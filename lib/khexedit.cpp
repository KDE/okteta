/***************************************************************************
                          khexedit.cpp  -  description
                             -------------------
    begin                : Die Mai 13 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include <kdebug.h>

// c specific
#include <stdlib.h>
//#include <limits.h>
// c++ specific
//#include <limits>
// qt specific
#include <QStyle>
#include <QPainter>
#include <QTimer>
#include <QCursor>
#include <QApplication>
#include <QListIterator>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QShowEvent>
#include <QResizeEvent>
// kde specific
#ifndef QT_ONLY
#include <kglobalsettings.h>
#endif
// lib specific
#include "kabstractbytearraymodel.h"
#include "kbytearraymodel.h"
#include "koffsetcolumn.h"
#include "kvaluecolumn.h"
#include "kcharcolumn.h"
#include "kbordercolumn.h"
#include "kbuffercursor.h"
#include "kbufferlayout.h"
#include "kbufferranges.h"
#include "controller/ktabcontroller.h"
#include "controller/knavigator.h"
#include "controller/kvalueeditor.h"
#include "controller/kchareditor.h"
#include "kbufferdrag.h"
#include "kcursor.h"
#include "kbytecodec.h"
#include "kcharcodec.h"
#include "kwordbufferservice.h"
#include "khexedit.h"

using namespace KHE;

// zooming is done in steps of font size points
static const int DefaultZoomStep = 1;
static const int DefaultStartOffset = 0;//5;
static const int DefaultFirstLineOffset = 0;
static const int DefaultNoOfBytesPerLine =  16;
static const KHexEdit::KResizeStyle DefaultResizeStyle = KHexEdit::FullSizeUsage;
static const KHexEdit::KEncoding DefaultEncoding = KHexEdit::LocalEncoding;
static const int DefaultScrollTimerPeriod = 100;
static const int InsertCursorWidth = 2;



KHexEdit::KHexEdit( KAbstractByteArrayModel *Buffer, QWidget *Parent )
 : KColumnsView( Parent ),
   ByteArrayModel( Buffer ),
   BufferLayout( new KBufferLayout(DefaultNoOfBytesPerLine,DefaultStartOffset,0) ),
   BufferCursor( new KBufferCursor(BufferLayout) ),
   BufferRanges( new KBufferRanges(BufferLayout) ),
   CursorBlinkTimer( new QTimer(this) ),
   ScrollTimer( new QTimer(this) ),
   DragStartTimer( new QTimer(this) ),
   TrippleClickTimer( new QTimer(this) ),
   CursorPixmaps( new KCursor() ),
   Codec( 0 ),
   ClipboardMode( QClipboard::Clipboard ),
   ResizeStyle( DefaultResizeStyle ),
   Encoding( MaxEncodingId ), // forces update
   ReadOnly( false ),
//    Modified( false ),
   OverWriteOnly( false ),
   OverWrite( true ),
   MousePressed( false ),
   InDoubleClick( false ),
   InDnD( false ),
   DragStartPossible( false ),
   CursorPaused( false ),
   BlinkCursorVisible( false ),
   InZooming( false ),
   d( 0 )
{
  // initialize layout
  if( !ByteArrayModel )
    ByteArrayModel = new KByteArrayModel; // TODO: leaking, make it shared
  connect( ByteArrayModel, SIGNAL(contentsChanged(int,int)), SLOT(updateRange(int,int)) );
  connect( ByteArrayModel, SIGNAL(contentsReplaced(int,int,int)), SLOT(onContentsReplaced(int,int,int)) );
  connect( ByteArrayModel, SIGNAL(contentsMoved(int,int,int)), SLOT(onContentsMoved(int,int,int)) );

  BufferLayout->setLength( ByteArrayModel->size() );
  BufferLayout->setNoOfLinesPerPage( noOfLinesPerPage() );

  // creating the columns in the needed order
  OffsetColumn =       new KOffsetColumn( this, DefaultFirstLineOffset, DefaultNoOfBytesPerLine, KOffsetFormat::Hexadecimal );
  FirstBorderColumn =  new KBorderColumn( this, false );
  ValueColumn =        new KValueColumn( this, ByteArrayModel, BufferLayout, BufferRanges );
  SecondBorderColumn = new KBorderColumn( this, true );
  CharColumn =         new KCharColumn( this, ByteArrayModel, BufferLayout, BufferRanges );

  // select the active column
  ActiveColumn = &charColumn();
  InactiveColumn = &valueColumn();

  // set encoding
  Codec = KCharCodec::createCodec( (KHE::KEncoding)DefaultEncoding );
  valueColumn().setCodec( Codec );
  charColumn().setCodec( Codec );
  Encoding = DefaultEncoding;

  TabController = new KTabController( this, 0 );
  Navigator = new KNavigator( this, TabController );
  ValueEditor = new KValueEditor( ValueColumn, BufferCursor, this, Navigator );
  CharEditor = new KCharEditor( CharColumn, BufferCursor, this, Navigator );

  Controller = Navigator;

#ifdef QT_ONLY
  QFont FixedFont( "fixed", 10 );
  FixedFont.setFixedPitch( true );
  setFont( FixedFont );
#else
  setFont( KGlobalSettings::fixedFont() );
#endif

  // get the full control
  viewport()->installEventFilter( this );
  installEventFilter( this );

  connect( CursorBlinkTimer, SIGNAL(timeout()), this, SLOT(blinkCursor()) );
  connect( ScrollTimer,      SIGNAL(timeout()), this, SLOT(autoScrollTimerDone()) );
  connect( DragStartTimer,   SIGNAL(timeout()), this, SLOT(startDrag()) );
  DragStartTimer->setSingleShot( true );
  TrippleClickTimer->setSingleShot( true );

  viewport()->setAcceptDrops( true );
}


KHexEdit::~KHexEdit()
{
  delete TabController;
  delete Navigator;
  delete ValueEditor;
  delete CharEditor;
}


int KHexEdit::noOfBytesPerLine()               const { return BufferLayout->noOfBytesPerLine(); }
int KHexEdit::firstLineOffset()                const { return OffsetColumn->firstLineOffset(); }
int KHexEdit::startOffset()                    const { return BufferLayout->startOffset(); }
KHexEdit::KResizeStyle KHexEdit::resizeStyle() const { return ResizeStyle; }
KHexEdit::KCoding KHexEdit::coding()           const { return (KHexEdit::KCoding)valueColumn().coding(); }
KPixelX KHexEdit::byteSpacingWidth()           const { return valueColumn().byteSpacingWidth(); }
int KHexEdit::noOfGroupedBytes()               const { return valueColumn().noOfGroupedBytes(); }
KPixelX KHexEdit::groupSpacingWidth()          const { return valueColumn().groupSpacingWidth(); }
KPixelX KHexEdit::binaryGapWidth()             const { return valueColumn().binaryGapWidth(); }
bool KHexEdit::isOverwriteMode()               const { return OverWrite; }
bool KHexEdit::isOverwriteOnly()               const { return OverWriteOnly; }
bool KHexEdit::isReadOnly()                    const { return ReadOnly; }
bool KHexEdit::isModified()                    const { return ByteArrayModel->isModified(); }
bool KHexEdit::tabChangesFocus()               const { return TabController->tabChangesFocus(); }
bool KHexEdit::showUnprintable()               const { return charColumn().showUnprintable(); }
QChar KHexEdit::substituteChar()               const { return charColumn().substituteChar(); }
QChar KHexEdit::undefinedChar()                const { return charColumn().undefinedChar(); }
KHexEdit::KEncoding KHexEdit::encoding()       const { return (KHexEdit::KEncoding)Encoding; }
const QString &KHexEdit::encodingName()        const { return Codec->name(); }

int KHexEdit::cursorPosition() const { return BufferCursor->index(); }
bool KHexEdit::isCursorBehind() const { return BufferCursor->isBehind(); }
KHexEdit::KBufferColumnId KHexEdit::cursorColumn() const
{ return static_cast<KHE::KValueColumn *>( ActiveColumn ) == &valueColumn()? ValueColumnId : CharColumnId; }

void KHexEdit::setOverwriteOnly( bool OO )    { OverWriteOnly = OO; if( OverWriteOnly ) setOverwriteMode( true ); }
void KHexEdit::setModified( bool M )          { ByteArrayModel->setModified(M); }
void KHexEdit::setTabChangesFocus( bool TCF ) { TabController->setTabChangesFocus(TCF); }
void KHexEdit::setFirstLineOffset( int FLO )  { OffsetColumn->setFirstLineOffset( FLO ); }

bool KHexEdit::offsetColumnVisible() const { return OffsetColumn->isVisible(); }
int KHexEdit::visibleBufferColumns() const
{ return (valueColumn().isVisible() ? ValueColumnId : 0) | (charColumn().isVisible() ? CharColumnId : 0); }


void KHexEdit::setOverwriteMode( bool OM )
{
  if( (OverWriteOnly && !OM) || (OverWrite == OM) )
    return;

  OverWrite = OM;

  // affected:
  // cursor shape
  bool ChangeCursor = !( CursorPaused || ValueEditor->isInEditMode() );
  if( ChangeCursor )
    pauseCursor();

  BufferCursor->setAppendPosEnabled( !OverWrite );

  if( ChangeCursor )
    unpauseCursor();

  emit cutAvailable( !OverWrite && BufferRanges->hasSelection() );
}


void KHexEdit::setDataBuffer( KAbstractByteArrayModel *B )
{
  disconnect( ByteArrayModel, SIGNAL(contentsChanged(int,int)),
              this, SLOT(updateRange(int,int)) );
  disconnect( ByteArrayModel, SIGNAL(contentsReplaced(int,int,int)),
              this, SLOT(onContentsReplaced(int,int,int)) );
  disconnect( ByteArrayModel, SIGNAL(contentsMoved(int,int,int)),
              this, SLOT(onContentsMoved(int,int,int)) );

  ValueEditor->reset();
  CursorPaused = true;

  ByteArrayModel = B;
  valueColumn().set( ByteArrayModel );
  charColumn().set( ByteArrayModel);

  // affected:
  // length -> no of lines -> width
  BufferLayout->setLength( ByteArrayModel->size() );
  adjustLayoutToSize();

  // ensure that the widget is readonly if the buffer is
  if( ByteArrayModel->isReadOnly() )
    setReadOnly( true );

  viewport()->update();
  connect( ByteArrayModel, SIGNAL(contentsChanged(int,int)), SLOT(updateRange(int,int)) );
  connect( ByteArrayModel, SIGNAL(contentsReplaced(int,int,int)), SLOT(onContentsReplaced(int,int,int)) );
  connect( ByteArrayModel, SIGNAL(contentsMoved(int,int,int)), SLOT(onContentsMoved(int,int,int)) );

  BufferCursor->gotoStart();
  ensureCursorVisible();
  unpauseCursor();
}


void KHexEdit::setStartOffset( int SO )
{
  if( !BufferLayout->setStartOffset(SO) )
    return;

  pauseCursor();
  // affects:
  // the no of lines -> width
  adjustLayoutToSize();

  viewport()->update();

  BufferCursor->updateCoord();
  ensureCursorVisible();
  unpauseCursor();
}


void KHexEdit::setReadOnly( bool RO )
{
  // don't set editor readwrite if databuffer is readonly
  ReadOnly = (ByteArrayModel && ByteArrayModel->isReadOnly()) ? true : RO;

  Controller = ReadOnly ? (KController*)Navigator :
      cursorColumn() == CharColumnId ? (KController*)CharEditor : (KController*)ValueEditor;
}


void KHexEdit::setBufferSpacing( KPixelX ByteSpacing, int NoOfGroupedBytes, KPixelX GroupSpacing )
{
  if( !valueColumn().setSpacing(ByteSpacing,NoOfGroupedBytes,GroupSpacing) )
    return;

  updateViewByWidth();
}


void KHexEdit::setCoding( KCoding C )
{
  uint OldCodingWidth = valueColumn().byteCodec()->encodingWidth();

  if( !valueColumn().setCoding((KHE::KCoding)C) )
    return;

  uint NewCodingWidth = valueColumn().byteCodec()->encodingWidth();
  ValueEditor->ByteBuffer.resize( NewCodingWidth ); //hack for now

  // no change in the width?
  if( NewCodingWidth == OldCodingWidth )
    updateColumn( valueColumn() );
  else
    updateViewByWidth();
}


void KHexEdit::setResizeStyle( KResizeStyle NewStyle )
{
  if( ResizeStyle == NewStyle )
    return;

  ResizeStyle = NewStyle;

  updateViewByWidth();
}


void KHexEdit::setNoOfBytesPerLine( int NoBpL )
{
  // if the number is explicitly set we expect a wish for no automatic resize
  ResizeStyle = NoResize;

  if( !BufferLayout->setNoOfBytesPerLine(NoBpL) )
    return;
  updateViewByWidth();
}


void KHexEdit::setByteSpacingWidth( int/*KPixelX*/ BSW )
{
  if( !valueColumn().setByteSpacingWidth(BSW) )
    return;
  updateViewByWidth();
}

void KHexEdit::setNoOfGroupedBytes( int NoGB )
{
  if( !valueColumn().setNoOfGroupedBytes(NoGB) )
    return;
  updateViewByWidth();
}


void KHexEdit::setGroupSpacingWidth( int/*KPixelX*/ GSW )
{
  if( !valueColumn().setGroupSpacingWidth(GSW) )
    return;
  updateViewByWidth();
}


void KHexEdit::setBinaryGapWidth( int/*KPixelX*/ BGW )
{
  if( !valueColumn().setBinaryGapWidth(BGW) )
    return;
  updateViewByWidth();
}


void KHexEdit::setSubstituteChar( QChar SC )
{
  if( !charColumn().setSubstituteChar(SC) )
    return;
  pauseCursor();
  updateColumn( charColumn() );
  unpauseCursor();
}

void KHexEdit::setUndefinedChar( QChar UC )
{
  if( !charColumn().setUndefinedChar(UC) )
    return;
  pauseCursor();
  updateColumn( charColumn() );
  unpauseCursor();
}

void KHexEdit::setShowUnprintable( bool SU )
{
  if( !charColumn().setShowUnprintable(SU) )
    return;
  pauseCursor();
  updateColumn( charColumn() );
  unpauseCursor();
}


void KHexEdit::setEncoding( KEncoding C )
{
  if( Encoding == C )
    return;

  KCharCodec *NC = KCharCodec::createCodec( (KHE::KEncoding)C );
  if( NC == 0 )
    return;

  valueColumn().setCodec( NC );
  charColumn().setCodec( NC );

  delete Codec;
  Codec = NC;
  Encoding = C;

  pauseCursor();
  updateColumn( valueColumn() );
  updateColumn( charColumn() );
  unpauseCursor();
}

// TODO: join with function above!
void KHexEdit::setEncoding( const QString& EncodingName )
{
  if( EncodingName == Codec->name() )
    return;

  KCharCodec *NC = KCharCodec::createCodec( EncodingName );
  if( NC == 0 )
    return;

  valueColumn().setCodec( NC );
  charColumn().setCodec( NC );

  delete Codec;
  Codec = NC;
  Encoding = LocalEncoding; // TODO: add encoding no to every known codec

  pauseCursor();
  updateColumn( valueColumn() );
  updateColumn( charColumn() );
  unpauseCursor();
}


void KHexEdit::fontChange( const QFont &OldFont )
{
  KColumnsView::fontChange( OldFont );

  if( !InZooming )
    DefaultFontSize = font().pointSize();

  // get new values
  QFontMetrics FM( fontMetrics() );
  KPixelX DigitWidth = FM.maxWidth();
  KPixelY DigitBaseLine = FM.ascent();

  setLineHeight( FM.height() );

  // update all dependant structures
  BufferLayout->setNoOfLinesPerPage( noOfLinesPerPage() );

  OffsetColumn->setMetrics( DigitWidth, DigitBaseLine );
  valueColumn().setMetrics( DigitWidth, DigitBaseLine );
  charColumn().setMetrics( DigitWidth, DigitBaseLine );

  updateViewByWidth();
}


void KHexEdit::updateViewByWidth()
{
  pauseCursor();

  adjustToLayoutNoOfBytesPerLine();
  adjustLayoutToSize();

  viewport()->update();

  BufferCursor->updateCoord();
  ensureCursorVisible();

  unpauseCursor();
}


void KHexEdit::zoomIn()         { zoomIn( DefaultZoomStep ); }
void KHexEdit::zoomOut()        { zoomOut( DefaultZoomStep ); }

void KHexEdit::zoomIn( int PointInc )
{
  InZooming = true;
  QFont F( font() );
  F.setPointSize( QFontInfo(F).pointSize() + PointInc );
  setFont( F );
  InZooming = false;
}

void KHexEdit::zoomOut( int PointDec )
{
  InZooming = true;
  QFont F( font() );
  F.setPointSize( qMax( 1, QFontInfo(F).pointSize() - PointDec ) );
  setFont( F );
  InZooming = false;
}


void KHexEdit::zoomTo( int PointSize )
{
  InZooming = true;
  QFont F( font() );
  F.setPointSize( PointSize );
  setFont( F );
  InZooming = false;
}


void KHexEdit::unZoom()
{
  zoomTo( DefaultFontSize );
}


void KHexEdit::adjustLayoutToSize()
{
  // check whether there is a change with the numbers of fitting bytes per line
  if( ResizeStyle != NoResize )
  {
    int FittingBytesPerLine = fittingBytesPerLine();

//     std::cout<<"FitBpL"<<FittingBytesPerLine<<std::endl;

    // changes?
    if( BufferLayout->setNoOfBytesPerLine(FittingBytesPerLine) )
      adjustToLayoutNoOfBytesPerLine();
  }

  setNoOfLines( BufferLayout->noOfLines() );
}


void KHexEdit::adjustToLayoutNoOfBytesPerLine()
{
  OffsetColumn->setDelta( BufferLayout->noOfBytesPerLine() );
  valueColumn().resetXBuffer();
  charColumn().resetXBuffer();

  updateWidths();
}


void KHexEdit::setNoOfLines( int NewNoOfLines )
{
  KColumnsView::setNoOfLines( NewNoOfLines>1?NewNoOfLines:1 );
}


void KHexEdit::toggleOffsetColumn( bool Visible )
{
  bool OCVisible = OffsetColumn->isVisible();
  // no change?
  if( OCVisible == Visible )
    return;

  OffsetColumn->setVisible( Visible );
  FirstBorderColumn->setVisible( Visible );

  updateViewByWidth();
}


QSize KHexEdit::sizeHint() const
{
  return QSize( columnsWidth(), columnsHeight() );
}


QSize KHexEdit::minimumSizeHint() const
{
  // TODO: better minimal width (visibility!)
  return QSize( OffsetColumn->visibleWidth()+FirstBorderColumn->visibleWidth()+SecondBorderColumn->visibleWidth()+valueColumn().byteWidth()+charColumn().byteWidth(),
                lineHeight() + noOfLines()>1? style()->pixelMetric(QStyle::PM_ScrollBarExtent):0 );
}


void KHexEdit::resizeEvent( QResizeEvent *ResizeEvent )
{
  if( ResizeStyle != NoResize )
  {
    int FittingBytesPerLine = fittingBytesPerLine();

    // changes?
    if( BufferLayout->setNoOfBytesPerLine(FittingBytesPerLine) )
    {
      setNoOfLines( BufferLayout->noOfLines() );
      updateViewByWidth();
    }
  }

  KColumnsView::resizeEvent( ResizeEvent );

  BufferLayout->setNoOfLinesPerPage( noOfLinesPerPage() ); // TODO: doesn't work with the new size!!!
}


int KHexEdit::fittingBytesPerLine() const
{
   const QSize &NewSize = maximumViewportSize();
  KPixelX ReservedWidth = OffsetColumn->visibleWidth() + FirstBorderColumn->visibleWidth() + SecondBorderColumn->visibleWidth();

  // abstract offset and border columns width
  KPixelX FullWidth = NewSize.width() - ReservedWidth;

//  // no width left for resizeable columns? TODO: put this in resizeEvent
//  if( FullWidth < 0 )
//    return;

  KPixelY FullHeight = NewSize.height();

  // check influence of dis-/appearing of the vertical scrollbar
  bool VerticalScrollbarIsVisible = verticalScrollBar()->isVisible();
  KPixelX ScrollbarExtent = style()->pixelMetric( QStyle::PM_ScrollBarExtent );

  KPixelX AvailableWidth = FullWidth;
  if( VerticalScrollbarIsVisible )
    AvailableWidth -= ScrollbarExtent;

  enum KMatchTrial { FirstRun, RerunWithScrollbarOn, TestWithoutScrollbar };
  KMatchTrial MatchRun = FirstRun;

  // prepare needed values
  KPixelX DigitWidth = valueColumn().digitWidth();
  KPixelX TextByteWidth = charColumn().isVisible() ? DigitWidth : 0;
  KPixelX HexByteWidth = valueColumn().isVisible() ? valueColumn().byteWidth() : 0;
  KPixelX ByteSpacingWidth = valueColumn().isVisible() ? valueColumn().byteSpacingWidth() : 0;
  KPixelX GroupSpacingWidth;
  int NoOfGroupedBytes = valueColumn().noOfGroupedBytes();
  // no grouping?
  if( NoOfGroupedBytes == 0 )
  {
    // faking grouping by 1
    NoOfGroupedBytes = 1;
    GroupSpacingWidth = 0;
  }
  else
    GroupSpacingWidth = valueColumn().isVisible() ? valueColumn().groupSpacingWidth() : 0;

  KPixelX HexByteGroupWidth =  NoOfGroupedBytes * HexByteWidth + (NoOfGroupedBytes-1)*ByteSpacingWidth;
  KPixelX TextByteGroupWidth = NoOfGroupedBytes * TextByteWidth;
  KPixelX TotalGroupWidth = HexByteGroupWidth + GroupSpacingWidth + TextByteGroupWidth;

  int FittingBytesPerLine;
  int WithScrollbarFittingBytesPerLine = 0;
  for(;;)
  {
//    kDebug() << "matchWidth: " << FullWidth
//              << " (v:" << visibleWidth()
//              << ", f:" << frameWidth()
//              << ", A:" << AvailableWidth
//              << ", S:" << ScrollbarExtent
//              << ", R:" << ReservedWidth << ")" << endl;

    // calculate fitting groups per line
    int FittingGroupsPerLine = (AvailableWidth+GroupSpacingWidth) // fake spacing after last group
                               / TotalGroupWidth;

    // calculate the fitting bytes per line by groups
    FittingBytesPerLine = NoOfGroupedBytes * FittingGroupsPerLine;

    // not only full groups?
    if( ResizeStyle == FullSizeUsage && NoOfGroupedBytes > 1 )
    {
      if( FittingGroupsPerLine > 0 )
        AvailableWidth -= FittingGroupsPerLine*TotalGroupWidth; // includes additional spacing after last group

//         kDebug() << "Left: " << AvailableWidth << "("<<HexByteWidth<<", "<<TextByteWidth<<")" << endl;

      if( AvailableWidth > 0 )
        FittingBytesPerLine += (AvailableWidth+ByteSpacingWidth) / (HexByteWidth+ByteSpacingWidth+TextByteWidth);

      // is there not even the space for a single byte?
      if( FittingBytesPerLine == 0 )
      {
        // ensure at least one byte per line
        FittingBytesPerLine = 1;
        // and
        break;
      }
    }
    // is there not the space for a single group?
    else if( FittingBytesPerLine == 0 )
    {
      // ensures at least one group
      FittingBytesPerLine = NoOfGroupedBytes;
      break;
    }

//    kDebug() << "meantime: " << FittingGroupsPerLine << " (T:" << TotalGroupWidth
//              << ", h:" << HexByteGroupWidth
//              << ", t:" << TextByteGroupWidth
//              << ", s:" << GroupSpacingWidth << ") " <<FittingBytesPerLine<< endl;

    int NewNoOfLines = (BufferLayout->length()+BufferLayout->startOffset()+FittingBytesPerLine-1)
                       / FittingBytesPerLine;
    KPixelY NewHeight =  NewNoOfLines * LineHeight;

    if( VerticalScrollbarIsVisible )
    {
      if( MatchRun == TestWithoutScrollbar )
      {
        // did the test without the scrollbar fail, don't the data fit into the view?
        if( NewHeight>FullHeight )
          // reset to old calculated value
          FittingBytesPerLine =  WithScrollbarFittingBytesPerLine;
        break;
      }

      // a chance for to perhaps fit in height?
      if( FittingBytesPerLine <= BufferLayout->noOfBytesPerLine() )
      {
        // remember this trial's result and calc number of bytes with vertical scrollbar on
        WithScrollbarFittingBytesPerLine = FittingBytesPerLine;
        AvailableWidth = FullWidth;
        MatchRun = TestWithoutScrollbar;
//          kDebug() << "tested without scrollbar..." << endl;
        continue;
      }
    }
    else
    {
      // doesn't it fit into the height anymore?
      if( NewHeight>FullHeight && MatchRun==FirstRun )
      {
        // need for a scrollbar has risen... ->less width, new calculation
        AvailableWidth = FullWidth - ScrollbarExtent;
        MatchRun = RerunWithScrollbarOn;
//          kDebug() << "rerun with scrollbar on..." << endl;
        continue;
      }
    }

    break;
  }

  return FittingBytesPerLine;
}


bool KHexEdit::selectWord( /*unsigned TODO:change all unneeded signed into unsigned!*/ int Index )
{
  if( Index >= 0 && Index < BufferLayout->length()  )
  {
    KWordBufferService WBS( ByteArrayModel, Codec );
    KSection WordSection = WBS.wordSection( Index );
    if( WordSection.isValid() )
    {
      pauseCursor();

      BufferRanges->setFirstWordSelection( WordSection );
      BufferCursor->gotoIndex( WordSection.behindEnd() );
      updateChanged();

      unpauseCursor();
      return true;
    }
  }
  return false;
}


void KHexEdit::selectAll( bool Select )
{
  pauseCursor( true );

  if( !Select )
    BufferRanges->removeSelection();
  else
  {
    BufferRanges->setSelection( KSection(0,BufferLayout->length()-1) );
    BufferCursor->gotoEnd();
  }

  updateChanged();

  unpauseCursor();

  if( !OverWrite ) emit cutAvailable( BufferRanges->hasSelection() );
  emit copyAvailable( BufferRanges->hasSelection() );
  emit selectionChanged();
  viewport()->setCursor( isReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor );
}


bool KHexEdit::hasSelectedData() const
{
  return BufferRanges->hasSelection();
}


QByteArray KHexEdit::selectedData() const
{
  if( !BufferRanges->hasSelection() )
    return QByteArray();

  KSection Selection = BufferRanges->selection();
  QByteArray SD;
  SD.resize( Selection.width() );
  ByteArrayModel->copyTo( SD.data(), Selection.start(), Selection.width() );
  return SD;
}


KBufferDrag *KHexEdit::dragObject() const
{
  if( !BufferRanges->hasSelection() )
    return 0;

  const KOffsetColumn *OC;
  const KValueColumn *HC;
  const KCharColumn *TC;
  KCoordRange Range;

  if( static_cast<KHE::KCharColumn *>( ActiveColumn ) == &charColumn() )
  {
    OC = 0;
    HC = 0;
    TC = 0;
  }
  else
  {
    OC = OffsetColumn->isVisible() ? OffsetColumn : 0;
    HC = valueColumn().isVisible() ? &valueColumn() : 0;
    TC = charColumn().isVisible() ? &charColumn() : 0;
    Range.set( BufferLayout->coordRangeOfIndizes(BufferRanges->selection()) );
  }

  return new KBufferDrag( selectedData(), Range, OC, HC, TC,
                          charColumn().substituteChar(), charColumn().undefinedChar(),
                          Codec->name() );
}


void KHexEdit::cut()
{
  if( isReadOnly() || OverWrite )
    return;

  KBufferDrag *Drag = dragObject();
  if( !Drag )
    return;

  QApplication::clipboard()->setMimeData( Drag, ClipboardMode );

  removeSelectedData();
}


void KHexEdit::copy()
{
  KBufferDrag *Drag = dragObject();
  if( !Drag )
    return;

  QApplication::clipboard()->setMimeData( Drag, ClipboardMode );
}


void KHexEdit::paste()
{
  if( isReadOnly() )
    return;

  const QMimeData *Data = QApplication::clipboard()->mimeData( ClipboardMode );
  pasteFromSource( Data );
}


void KHexEdit::pasteFromSource( const QMimeData *Data )
{
  if( !Data )
    return;

  QByteArray Bytes = Data->data( "application/octet-stream" );

  if( !Bytes.isEmpty() )
    insert( Bytes );
}


void KHexEdit::insert( const QByteArray &Data )
{
  pauseCursor( true );

  if( OverWrite )
  {
    if( BufferRanges->hasSelection() )
    {
      // replacing the selection:
      // we restrict the replacement to the minimum length of selection and input
      KSection Selection = BufferRanges->removeSelection();
      Selection.restrictEndByWidth( Data.size() );
      ByteArrayModel->replace( Selection, Data.data(), Selection.width() );
    }
    else
    {
      if( !BufferCursor->isBehind() )
      {
        // replacing the normal data, at least until the end
        KSection InsertRange = KSection::fromWidth( BufferCursor->realIndex(), Data.size() );
        InsertRange.restrictEndTo( BufferLayout->length()-1 );
        ByteArrayModel->replace( InsertRange, Data.data(), InsertRange.width() );
      }
    }
  }
  else
  {
    if( BufferRanges->hasSelection() )
    {
      // replacing the selection
      KSection Selection = BufferRanges->removeSelection();
      ByteArrayModel->replace( Selection, Data );
    }
    else
      ByteArrayModel->insert( BufferCursor->realIndex(), Data );
  }

  emit selectionChanged();
}


void KHexEdit::removeSelectedData()
{
  // Can't we do this?
  if( isReadOnly() || OverWrite || ValueEditor->isInEditMode() )
    return;

  KSection Selection = BufferRanges->removeSelection();

  ByteArrayModel->remove( Selection );

//     clearUndoRedo();

  //emit selectionChanged( -1, -1 );
}


void KHexEdit::updateRange( int Start, int End )
{
  BufferRanges->addChangedRange( Start, End );
kDebug() << "update: "<<Start<<","<<End<<endl;

  unpauseCursor();
  updateChanged();
}

void KHexEdit::onContentsReplaced( int Pos, int RemovedLength, int InsertedLength )
{
  pauseCursor();

  bool Appending = BufferCursor->atAppendPos();
  // update lengths
  int OldNoOfLines = noOfLines();
  BufferLayout->setLength( ByteArrayModel->size() );
  int NewNoOfLines = BufferLayout->noOfLines();
  if( OldNoOfLines != NewNoOfLines )
  {
    setNoOfLines( NewNoOfLines );
    updateColumn( *OffsetColumn );
  }

kDebug()<< "Pos:"<<Pos<<", RemovedLength:"<<RemovedLength<<", InsertedLength:"<<InsertedLength<<endl;
kDebug() << "Cursor:"<<BufferCursor->index()<<", "<<BufferCursor->isBehind()<<endl;
  // adapt cursor(s)
  if( Appending )
    BufferCursor->gotoEnd();
  else
    BufferCursor->adaptToChange( Pos, RemovedLength, InsertedLength );

  BufferRanges->adaptSelectionToChange( Pos, RemovedLength, InsertedLength );
kDebug() << "Cursor:"<<BufferCursor->index()<<", Selection:"<<BufferRanges->selectionStart()<<"-"<<BufferRanges->selectionEnd()
         <<", BytesPerLine: "<<BufferLayout->noOfBytesPerLine()<<endl;
}

void KHexEdit::onContentsMoved( int Destination, int Source, int MovedLength )
{
  pauseCursor();

}

void KHexEdit::clipboardChanged()
{
  // don't listen to selection changes
  disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()), this, 0 );
  selectAll( false );
}


void KHexEdit::setCursorPosition( int Index, bool Behind )
{
  pauseCursor( true );

  BufferCursor->gotoCIndex( Index );
  if( Behind )
    BufferCursor->stepBehind();

  BufferRanges->removeSelection();
  if( BufferRanges->isModified() )
  {
    updateChanged();

    viewport()->setCursor( isReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor );

    if( !OverWrite ) emit cutAvailable( BufferRanges->hasSelection() );
    emit copyAvailable( BufferRanges->hasSelection() );
    emit selectionChanged();
  }
  ensureCursorVisible();

  unpauseCursor();
}


void KHexEdit::showBufferColumns( int CCs )
{
  int Columns = visibleBufferColumns();

  // no changes or no column selected?
  if( CCs == Columns || !(CCs&( ValueColumnId | CharColumnId )) )
    return;

  valueColumn().setVisible( ValueColumnId & CCs );
  charColumn().setVisible( CharColumnId & CCs );
  SecondBorderColumn->setVisible( CCs == (ValueColumnId|CharColumnId) );

  // active column not visible anymore?
  if( !activeColumn().isVisible() )
  {
    KBufferColumn *H = ActiveColumn;
    ActiveColumn = InactiveColumn;
    InactiveColumn = H;
    Controller = ReadOnly ? (KController*)Navigator :
      cursorColumn() == CharColumnId ? (KController*)CharEditor : (KController*)ValueEditor;
  }

  updateViewByWidth();
}


void KHexEdit::setCursorColumn( KBufferColumnId CC )
{
  // no changes or not visible?
  if( CC == cursorColumn()
      || (CC == ValueColumnId && !valueColumn().isVisible())
      || (CC == CharColumnId && !charColumn().isVisible()) )
    return;

  pauseCursor( true );

  if( CC == ValueColumnId )
  {
    ActiveColumn = &valueColumn();
    InactiveColumn = &charColumn();
  }
  else
  {
    ActiveColumn = &charColumn();
    InactiveColumn = &valueColumn();
  }
  Controller = ReadOnly ? (KController*)Navigator :
    cursorColumn() == CharColumnId ? (KController*)CharEditor : (KController*)ValueEditor;

  ensureCursorVisible();
  unpauseCursor();
}


void KHexEdit::placeCursor( const QPoint &Point )
{
  resetInputContext();

  // switch active column if needed
  if( charColumn().isVisible() && Point.x() >= charColumn().x() )
  {
    ActiveColumn = &charColumn();
    InactiveColumn = &valueColumn();
  }
  else
  {
    ActiveColumn = &valueColumn();
    InactiveColumn = &charColumn();
  }
  Controller = ReadOnly ? (KController*)Navigator :
      cursorColumn() == CharColumnId ? (KController*)CharEditor : (KController*)ValueEditor;

  // get coord of click and whether this click was closer to the end of the pos
  KBufferCoord C( activeColumn().magPosOfX(Point.x()), lineAt(Point.y()) );

  BufferCursor->gotoCCoord( C );
}


int KHexEdit::indexByPoint( const QPoint &Point ) const
{
  const KBufferColumn *C;
  if( charColumn().isVisible() && Point.x() >= charColumn().x() )
    C = &charColumn();
  else
    C = &valueColumn();

  KBufferCoord Coord( C->posOfX(Point.x()), lineAt(Point.y()) );

  return BufferLayout->indexAtCCoord( Coord );
}


void KHexEdit::showEvent( QShowEvent *Event )
{
    KColumnsView::showEvent( Event );
    BufferLayout->setNoOfLinesPerPage( noOfLinesPerPage() );
}


bool KHexEdit::eventFilter( QObject *Object, QEvent *E )
{
  if( Object == this || Object == viewport() )
  {
    if( E->type() == QEvent::FocusIn )
    {
      startCursor();
    }
    else if( E->type() == QEvent::FocusOut )
    {
      stopCursor();
    }
  }

//   if( O == this && E->type() == QEvent::PaletteChange )
//   {
//     QColor old( viewport()->colorGroup().color(QPalette::Text) );
//
//     if( old != colorGroup().color(QPalette::Text) )
//     {
//       QColor c( colorGroup().color(QPalette::Text) );
//       doc->setMinimumWidth( -1 );
//       doc->setDefaultFormat( doc->formatCollection()->defaultFormat()->font(), c );
//       lastFormatted = doc->firstParagraph();
//       formatMore();
//       updateChanged();
//     }
//   }

  return KColumnsView::eventFilter( Object, E );
}


void KHexEdit::blinkCursor()
{
  // skip the cursor drawing?
  if( CursorPaused || ValueEditor->isInEditMode() )
    return;

  // switch the cursor state
  BlinkCursorVisible = !BlinkCursorVisible;
  updateCursor( activeColumn() );
}


void KHexEdit::startCursor()
{
  CursorPaused = false;

  updateCursors();

  CursorBlinkTimer->start( QApplication::cursorFlashTime()/2 );
}


void KHexEdit::unpauseCursor()
{
  CursorPaused = false;

  if( CursorBlinkTimer->isActive() )
    updateCursors();
}


void KHexEdit::updateCursors()
{
  createCursorPixmaps();

  BlinkCursorVisible = true;
  updateCursor( activeColumn() );
  updateCursor( inactiveColumn() );
}


void KHexEdit::stopCursor()
{
  CursorBlinkTimer->stop();

  pauseCursor();
}


void KHexEdit::pauseCursor( bool LeaveEdit )
{
  CursorPaused = true;

  BlinkCursorVisible = false;
  updateCursor( activeColumn() );
  updateCursor( inactiveColumn() );

  if( LeaveEdit )
    ValueEditor->InEditMode = false;
}


void KHexEdit::updateCursor( const KBufferColumn &Column )
{
  int x = Column.xOfPos( BufferCursor->pos() ) - xOffset();
  int y = LineHeight * BufferCursor->line() - yOffset();
  int w = Column.byteWidth();

  viewport()->update( x,y, w,LineHeight );
}

void KHexEdit::createCursorPixmaps()
{
  // create CursorPixmaps
  CursorPixmaps->setSize( activeColumn().byteWidth(), LineHeight );

  int Index = BufferCursor->validIndex();

  QPainter Paint;
  Paint.begin( &CursorPixmaps->offPixmap() );
  Paint.initFrom( this );
  activeColumn().paintByte( &Paint, Index );
  Paint.end();

  Paint.begin( &CursorPixmaps->onPixmap() );
  Paint.initFrom( this );
  activeColumn().paintCursor( &Paint, Index );
  Paint.end();

  // calculat the shape
  KPixelX CursorX;
  KPixelX CursorW;
  if( BufferCursor->isBehind() )
  {
    CursorX = qMax( 0, CursorPixmaps->onPixmap().width()-InsertCursorWidth );
    CursorW = InsertCursorWidth;
  }
  else
  {
    CursorX = 0;
    CursorW = OverWrite ? -1 : InsertCursorWidth;
  }
  CursorPixmaps->setShape( CursorX, CursorW );
}


void KHexEdit::pointPainterToCursor( QPainter &Painter, const KBufferColumn &Column ) const
{
  int x = Column.xOfPos( BufferCursor->pos() ) - xOffset();
  int y = LineHeight * BufferCursor->line() - yOffset();

  Painter.begin( viewport() );
  Painter.translate( x, y );
}



void KHexEdit::drawActiveCursor()
{
  // any reason to skip the cursor drawing?
  if( BlinkCursorVisible && !hasFocus() && !viewport()->hasFocus() && !InDnD )
    return;

  QPainter Painter;
  pointPainterToCursor( Painter, activeColumn() );

  // paint edited byte?
  if( ValueEditor->isInEditMode() )
  {
    int Index = BufferCursor->index();

    if( BlinkCursorVisible )
      valueColumn().paintEditedByte( &Painter, ValueEditor->EditValue, ValueEditor->ByteBuffer );
    else
      valueColumn().paintByte( &Painter, Index );
  }
  else
    Painter.drawPixmap( CursorPixmaps->cursorX(), 0,
                        BlinkCursorVisible?CursorPixmaps->onPixmap():CursorPixmaps->offPixmap(),
                        CursorPixmaps->cursorX(),0,CursorPixmaps->cursorW(),-1 );
}


void KHexEdit::drawInactiveCursor()
{
  // any reason to skip the cursor drawing?
  if( !inactiveColumn().isVisible()
      || CursorPaused
      || (!CursorPaused && !hasFocus() && !viewport()->hasFocus() && !InDnD)  )
    return;

  int Index = BufferCursor->validIndex();

  QPainter Painter;
  pointPainterToCursor( Painter, inactiveColumn() );

  KBufferColumn::KFrameStyle Style =
    BufferCursor->isBehind() ? KBufferColumn::Right :
    (OverWrite||ValueEditor->isInEditMode()) ? KBufferColumn::Frame :
    KBufferColumn::Left;
  inactiveColumn().paintFramedByte( &Painter, Index, Style );
}


void KHexEdit::drawColumns( QPainter *P, int cx, int cy, int cw, int ch )
{
  KColumnsView::drawColumns( P, cx, cy, cw, ch );
  // TODO: update non blinking cursors. Should this perhaps be done in the buffercolumn?
  // Then it needs to know about inactive, insideByte and the like... well...
  // perhaps subclassing the buffer columns even more, to KCharColumn and KValueColumn?

  if( visibleLines(KPixelYs::fromWidth(cy,ch)).includes(BufferCursor->line()) )
  {
    drawActiveCursor();
    drawInactiveCursor();
  }
}

void KHexEdit::updateColumn( KColumn &Column )
{
  if( Column.isVisible() )
    viewport()->update( Column.x()-xOffset(), 0, Column.width(), visibleHeight() );
}

void KHexEdit::emitSelectionSignals()
{
  if( !OverWrite ) emit cutAvailable( BufferRanges->hasSelection() );
  emit copyAvailable( BufferRanges->hasSelection() );
  emit selectionChanged();
}


void KHexEdit::keyPressEvent( QKeyEvent *KeyEvent )
{
  if( !Controller->handleKeyPress( KeyEvent ) )
    KeyEvent->ignore();
}


void KHexEdit::updateChanged()
{
  KPixelXs Xs = KPixelXs::fromWidth( xOffset(), visibleWidth() );

  // collect affected buffer columns
  QList<KBufferColumn*> DirtyColumns;

  KBufferColumn *C = ValueColumn;
  while( true )
  {
    if( C->isVisible() && C->overlaps(Xs) )
    {
      DirtyColumns.append( C );
      C->preparePainting( Xs );
    }

    if( C == CharColumn )
      break;
    C = CharColumn;
  }

  // any colums to paint?
  if( DirtyColumns.size() > 0 )
  {
    KPixelYs Ys = KPixelYs::fromWidth( yOffset(), visibleHeight() );

    // calculate affected lines/indizes
    KSection FullPositions( 0, BufferLayout->noOfBytesPerLine()-1 );
    KCoordRange VisibleRange( FullPositions, visibleLines(Ys) );

    KCoordRange ChangedRange;
    // as there might be multiple selections on this line redo until no more is changed
    while( hasChanged(VisibleRange,&ChangedRange) )
    {
      KPixelY cy = ChangedRange.start().line() * LineHeight - yOffset();

      QListIterator<KBufferColumn*> it( DirtyColumns );
      // only one line?
      if( ChangedRange.start().line() == ChangedRange.end().line() )
      {
        while( it.hasNext() )
        {
          KPixelXs XPixels =
            it.next()->wideXPixelsOfPos( KSection(ChangedRange.start().pos(),ChangedRange.end().pos()) );

          viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), LineHeight );
        }
      }
      //
      else
      {
        // first line
        while( it.hasNext() )
        {
          KPixelXs XPixels =
            it.next()->wideXPixelsOfPos( KSection(ChangedRange.start().pos(),FullPositions.end()) );

          viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), LineHeight );
        }

        // at least one full line?
        for( int l = ChangedRange.start().line()+1; l < ChangedRange.end().line(); ++l )
        {
          cy += LineHeight;
          it.toFront();
          while( it.hasNext() )
          {
            KPixelXs XPixels =
              it.next()->wideXPixelsOfPos( FullPositions );

            viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), LineHeight );
          }
        }

        // last line
        cy += LineHeight;
        it.toFront();
        while( it.hasNext() )
        {
          KPixelXs XPixels =
            it.next()->wideXPixelsOfPos( KSection(FullPositions.start(),ChangedRange.end().pos()) );

          viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), LineHeight );
        }
      }

      // continue the search at the overnext index
      VisibleRange.setStart( ChangedRange.end()+2 );
      if( !VisibleRange.isValid() )
        break;
    }
  }

  BufferRanges->resetChangedRanges();
}


bool KHexEdit::hasChanged( const KCoordRange &VisibleRange, KCoordRange *ChangedRange ) const
{
  if( !BufferRanges->overlapsChanges(VisibleRange,ChangedRange) )
    return false;

  ChangedRange->restrictTo( VisibleRange );
  return true;
}


void KHexEdit::ensureCursorVisible()
{
//   // Not visible or the user is dragging the window, so don't position to caret yet
//   if ( !isVisible() || isHorizontalSliderPressed() || isVerticalSliderPressed() )
//   {
//     d->ensureCursorVisibleInShowEvent = true;
//     return;
//   }
  //static const int Margin = 10;

  // TODO: add getCursorRect to BufferColumn
  const KPixelXs cursorXs = KPixelXs::fromWidth( activeColumn().xOfPos(BufferCursor->pos()),
                                                 activeColumn().byteWidth() );

  const KPixelYs cursorYs = KPixelYs::fromWidth( LineHeight*BufferCursor->line(), LineHeight );

  const KPixelXs visibleXs = KPixelXs::fromWidth( xOffset(), visibleWidth() );
  const KPixelYs visibleYs = KPixelXs::fromWidth( yOffset(), visibleHeight() );

  horizontalScrollBar()->setValue( visibleXs.startForInclude(cursorXs) );
  verticalScrollBar()->setValue( visibleYs.startForInclude(cursorYs) );
}


void KHexEdit::mousePressEvent( QMouseEvent *Event )
{
//   clearUndoRedo();
  pauseCursor( true );

  // care about a left button press?
  if( Event->button() == Qt::LeftButton )
  {
    MousePressed = true;

    // select whole line?
    if( TrippleClickTimer->isActive()
        && (Event->globalPos()-DoubleClickPoint).manhattanLength() < QApplication::startDragDistance() )
    {
      BufferRanges->setSelectionStart( BufferLayout->indexAtLineStart(DoubleClickLine) );
      BufferCursor->gotoLineEnd();
      BufferRanges->setSelectionEnd( BufferCursor->realIndex() );
      updateChanged();

      unpauseCursor();
      return;
    }

    const QPoint MousePoint = viewportToColumns( Event->pos() );
    placeCursor( MousePoint );
    ensureCursorVisible();

    // start of a drag perhaps?
    if( BufferRanges->selectionIncludes(BufferCursor->index()) )
    {
      DragStartPossible = true;
      DragStartTimer->start( QApplication::startDragTime() );
      DragStartPoint = MousePoint;

      unpauseCursor();
      return;
    }

    int RealIndex = BufferCursor->realIndex();
    if( BufferRanges->selectionStarted() )
    {
      if( Event->modifiers() & Qt::SHIFT )
        BufferRanges->setSelectionEnd( RealIndex );
      else
      {
        BufferRanges->removeSelection();
        BufferRanges->setSelectionStart( RealIndex );
      }
    }
    else // start of a new selection possible
    {
      BufferRanges->setSelectionStart( RealIndex );

      if( !isReadOnly() && (Event->modifiers()&Qt::SHIFT) ) // TODO: why only for readwrite?
        BufferRanges->setSelectionEnd( RealIndex );
    }

    BufferRanges->removeFurtherSelections();
  }
  else if( Event->button() == Qt::MidButton )
    BufferRanges->removeSelection();

  if( BufferRanges->isModified() )
  {
    updateChanged();
    viewport()->setCursor( isReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor );
  }

  unpauseCursor();
}


void KHexEdit::mouseMoveEvent( QMouseEvent *Event )
{
  QPoint MovePoint = viewportToColumns( Event->pos() );

  if( MousePressed )
  {
    if( DragStartPossible )
    {
      DragStartTimer->stop();
      // moved enough for a drag?
      if( (MovePoint-DragStartPoint).manhattanLength() > QApplication::startDragDistance() )
        startDrag();
      if( !isReadOnly() )
        viewport()->setCursor( Qt::IBeamCursor );
      return;
    }
    // selecting
    handleMouseMove( MovePoint );
  }
  else if( !isReadOnly() )
  {
    // visual feedback for possible dragging
    bool InSelection = BufferRanges->hasSelection() && BufferRanges->selectionIncludes( indexByPoint(MovePoint) );
    viewport()->setCursor( InSelection?Qt::ArrowCursor:Qt::IBeamCursor );
  }
}


void KHexEdit::mouseReleaseEvent( QMouseEvent *Event )
{
  const QPoint ReleasePoint = viewportToColumns( Event->pos() );

  // this is not the release of a doubleclick so we need to process it?
  if( !InDoubleClick )
  {
    int Line = lineAt( ReleasePoint.y() );
    int Pos = activeColumn().posOfX( ReleasePoint.x() ); // TODO: can we be sure here about the active column?
    int Index = BufferLayout->indexAtCCoord( KBufferCoord(Pos,Line) ); // TODO: can this be another index than the one of the cursor???
    emit clicked( Index );
  }

  if( MousePressed )
  {
    MousePressed = false;

    if( ScrollTimer->isActive() )
      ScrollTimer->stop();

    // was only click inside selection, nothing dragged?
    if( DragStartPossible )
    {
      selectAll( false );
      DragStartTimer->stop();
      DragStartPossible = false;

      unpauseCursor();
    }
    // was end of selection operation?
    else if( BufferRanges->hasSelection() )
    {
      if( QApplication::clipboard()->supportsSelection() )
      {
        ClipboardMode = QClipboard::Selection;
        disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()), this, 0);

        copy();

        connect( QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(clipboardChanged()) );
        ClipboardMode = QClipboard::Clipboard;
      }
    }
  }
  // middle mouse button paste?
  else if( Event->button() == Qt::MidButton && !isReadOnly() )
  {
    pauseCursor();

    placeCursor( ReleasePoint );

    // replace no selection?
    if( BufferRanges->hasSelection() && !BufferRanges->selectionIncludes(BufferCursor->index()) )
      BufferRanges->removeSelection();

    ClipboardMode = QClipboard::Selection;
    paste();
    ClipboardMode = QClipboard::Clipboard;

    // ensure selection changes to be drawn TODO: create a insert/pasteAtCursor that leaves out drawing
    updateChanged();

    ensureCursorVisible();
    unpauseCursor();
  }

  emit cursorPositionChanged( BufferCursor->index() );

  InDoubleClick = false;

  if( BufferRanges->selectionJustStarted() )
    BufferRanges->removeSelection();

  if( !OverWrite ) emit cutAvailable( BufferRanges->hasSelection() );
  emit copyAvailable( BufferRanges->hasSelection() );
  emit selectionChanged();
}


// gets called after press and release instead of a plain press event (?)
void KHexEdit::mouseDoubleClickEvent( QMouseEvent *Event )
{
  // we are only interested in LMB doubleclicks
  if( Event->button() != Qt::LeftButton )
  {
    Event->ignore();
    return;
  }

  DoubleClickLine = BufferCursor->line();

  int Index = BufferCursor->validIndex();

  if( ActiveColumn == &charColumn() )
  {
    selectWord( Index );

    // as we already have a doubleclick maybe it is a tripple click
    TrippleClickTimer->start( qApp->doubleClickInterval() );
    DoubleClickPoint = Event->globalPos();
  }
//  else
//    ValueEditor->goInsideByte(); TODO: make this possible again

  InDoubleClick = true; //
  MousePressed = true;

  emit doubleClicked( Index );
}


void KHexEdit::autoScrollTimerDone()
{
  if( MousePressed )
    handleMouseMove( viewportToColumns(viewport()->mapFromGlobal( QCursor::pos() )) );
}


void KHexEdit::handleMouseMove( const QPoint& Point ) // handles the move of the mouse with pressed buttons
{
  // no scrolltimer and outside of viewport?
  if( !ScrollTimer->isActive() && Point.y() < yOffset() || Point.y() > yOffset() + visibleHeight() )
    ScrollTimer->start( DefaultScrollTimerPeriod );
  // scrolltimer but inside of viewport?
  else if( ScrollTimer->isActive() && Point.y() >= yOffset() && Point.y() <= yOffset() + visibleHeight() )
    ScrollTimer->stop();

  pauseCursor();

  placeCursor( Point );
  ensureCursorVisible();

  // do wordwise selection?
  if( InDoubleClick && BufferRanges->hasFirstWordSelection() )
  {
    int NewIndex = BufferCursor->realIndex();
    KSection FirstWordSelection = BufferRanges->firstWordSelection();
    KWordBufferService WBS( ByteArrayModel, Codec );
    // are we before the selection?
    if( NewIndex < FirstWordSelection.start() )
    {
      BufferRanges->ensureWordSelectionForward( false );
      NewIndex = WBS.indexOfLeftWordSelect( NewIndex );
    }
    // or behind?
    else if( NewIndex > FirstWordSelection.end() )
    {
      BufferRanges->ensureWordSelectionForward( true );
      NewIndex = WBS.indexOfRightWordSelect( NewIndex );
    }
    // or inside?
    else
    {
      BufferRanges->ensureWordSelectionForward( true );
      NewIndex = FirstWordSelection.behindEnd();
    }

    BufferCursor->gotoIndex( NewIndex );
  }

  if( BufferRanges->selectionStarted() )
    BufferRanges->setSelectionEnd( BufferCursor->realIndex() );

  updateChanged();

  unpauseCursor();
}


void KHexEdit::startDrag()
{
  // reset states
  MousePressed = false;
  InDoubleClick = false;
  DragStartPossible = false;

  // create data
  QMimeData *DragData = dragObject();
  if( !DragData )
    return;

  QDrag *drag = new QDrag( viewport() );
  drag->setMimeData( DragData );

  Qt::DropActions request = (isReadOnly()||OverWrite) ? Qt::CopyAction : Qt::CopyAction|Qt::MoveAction;
  Qt::DropAction dropAction = drag->start( request );

  if( dropAction == Qt::MoveAction )
    // Not inside this widget itself?
    if( drag->target() != this && drag->target() != viewport() )
      removeSelectedData();

  delete drag;
}


void KHexEdit::dragEnterEvent( QDragEnterEvent *event )
{
  // interesting for this widget?
  if( isReadOnly() || !event->mimeData()->hasFormat("application/octet-stream") )
  {
    event->ignore();
    return;
  }

  event->accept();
  InDnD = true;
}


void KHexEdit::dragMoveEvent( QDragMoveEvent *event )
{
  // is this content still interesting for us?
  if( isReadOnly() || !event->mimeData()->hasFormat("application/octet-stream") )
  {
    event->ignore();
    return;
  }

  // let text cursor follow mouse
  pauseCursor( true );
  placeCursor( event->pos() );
  unpauseCursor();

  event->accept();
}


void KHexEdit::dragLeaveEvent( QDragLeaveEvent * )
{
  // bye... and thanks for all the cursor movement...
  InDnD = false;
}



void KHexEdit::dropEvent( QDropEvent *Event )
{
  // after drag enter and move check one more time
  if( isReadOnly() )
    return;

  // leave state
  InDnD = false;
  Event->accept();

  // is this an internal dnd?
  if( Event->source() == this || Event->source() == viewport() )
    handleInternalDrag( Event );
  else
  {
   //BufferRanges->removeSelection();
    pasteFromSource( Event->mimeData() );
  }
}


void KHexEdit::handleInternalDrag( QDropEvent *Event )
{
  // get drag origin
  KSection Selection = BufferRanges->removeSelection();
  int InsertIndex = BufferCursor->realIndex();

  // is this a move?
  if( Event->proposedAction() == Qt::MoveAction )
  {
    // ignore the copy hold in the event but only move
    int NewIndex = ByteArrayModel->move( InsertIndex, Selection );
    if( NewIndex != Selection.start() )
    {
      BufferCursor->gotoCIndex( NewIndex+Selection.width() );
      BufferRanges->addChangedRange( KSection(qMin(InsertIndex,Selection.start()), qMax(InsertIndex,Selection.end())) );
    }
  }
  // is a copy
  else
  {
    // get data
    QByteArray Data = Event->mimeData()->data( "application/octet-stream" );

    if( !Data.isEmpty() )
    {
      if( OverWrite )
      {
        if( !BufferCursor->isBehind() )
        {
          KSection OverwriteRange = KSection::fromWidth( InsertIndex, Data.size() );
          OverwriteRange.restrictEndTo( BufferLayout->length()-1 );
          if( OverwriteRange.isValid() )
            ByteArrayModel->replace( OverwriteRange, Data.data(), OverwriteRange.width() );
        }
      }
      else
        ByteArrayModel->insert( InsertIndex, Data.data(), Data.size() );
    }
  }
}


void KHexEdit::wheelEvent( QWheelEvent *Event )
{
  if( isReadOnly() )
  {
    if( Event->modifiers() & Qt::CTRL )
    {
      if( Event->delta() > 0 )
        zoomOut();
      else if( Event->delta() < 0 )
        zoomIn();
      return;
    }
  }
  KColumnsView::wheelEvent( Event );
}


#if 0
void KHexEdit::contextMenuEvent( QContextMenuEvent *Event )
{
//   clearUndoRedo();
  MousePressed = false;

  Event->accept();

  QPopupMenu *PopupMenu = createPopupMenu( Event->pos() );
  if( !PopupMenu )
    PopupMenu = createPopupMenu();
  if( !PopupMenu )
    return;
  int r = PopupMenu->exec( Event->globalPos() );
  delete PopupMenu;

  if ( r == d->id[ IdClear ] )
    clear();
  else if ( r == d->id[ IdSelectAll ] )
  {
    selectAll();
    // if the clipboard support selections, put the newly selected text into the clipboard
    if( QApplication::clipboard()->supportsSelection() )
    {
      ClipboardMode = QClipboard::Selection;
      disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()), this, 0);

      copy();

      connect( QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(clipboardChanged()) );
      ClipboardMode = QClipboard::Clipboard;
    }
  }
  else if( r == d->id[IdUndo] )
    undo();
  else if( r == d->id[IdRedo] )
    redo();
  else if( r == d->id[IdCut] )
    cut();
  else if( r == d->id[IdCopy] )
    copy();
  else if( r == d->id[IdPaste] )
    paste();
}
#endif

#include "khexedit.moc"
