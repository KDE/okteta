/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "kbytearrayview.h"

// lib
#include "koffsetcolumn.h"
#include "kvaluecolumn.h"
#include "kcharcolumn.h"
#include "kbordercolumn.h"
#include "kdatacursor.h"
#include "kdatalayout.h"
#include "kdataranges.h"
#include "controller/ktabcontroller.h"
#include "controller/knavigator.h"
#include "controller/kvalueeditor.h"
#include "controller/kchareditor.h"
#include "kbytearraydrag.h"
#include "kcursor.h"
// Okteta core
#include <kabstractbytearraymodel.h>
#include <kbytearraymodel.h>
#include <valuecodec.h>
#include <kcharcodec.h>
#include <kwordbufferservice.h>
// KDE
#include <KGlobalSettings>
// Qt
#include <QtCore/QListIterator>
#include <QtCore/QTimer>
#include <QtGui/QStyle>
#include <QtGui/QPainter>
#include <QtGui/QCursor>
#include <QtGui/QApplication>
#include <QtGui/QScrollBar>
#include <QtGui/QShowEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDropEvent>
// C
#include <stdlib.h>
//#include <limits.h>
// c++ specific
//#include <limits>


namespace KHEUI {

// zooming is done in steps of font size points
static const int DefaultZoomStep = 1;
static const int DefaultStartOffset = 0;//5;
static const int DefaultFirstLineOffset = 0;
static const int DefaultNoOfBytesPerLine =  16;
static const KByteArrayView::KResizeStyle DefaultResizeStyle = KByteArrayView::FullSizeUsage;
static const KByteArrayView::KEncoding DefaultEncoding = KByteArrayView::LocalEncoding;
static const int DefaultScrollTimerPeriod = 100;
static const int InsertCursorWidth = 2;



KByteArrayView::KByteArrayView( KHECore::KAbstractByteArrayModel *Buffer, QWidget *Parent )
 : KColumnsView( Parent ),
   ByteArrayModel( Buffer ),
   BufferLayout( new KDataLayout(DefaultNoOfBytesPerLine,DefaultStartOffset,0) ),
   BufferCursor( new KDataCursor(BufferLayout) ),
   BufferRanges( new KDataRanges(BufferLayout) ),
   CursorBlinkTimer( new QTimer(this) ),
   ScrollTimer( new QTimer(this) ),
   DragStartTimer( new QTimer(this) ),
   TrippleClickTimer( new QTimer(this) ),
   CursorPixmaps( new KCursor() ),
   Codec( 0 ),
   ClipboardMode( QClipboard::Clipboard ),
   ResizeStyle( DefaultResizeStyle ),
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
    ByteArrayModel = new KHECore::KByteArrayModel; // TODO: leaking, make it shared
  connect( ByteArrayModel, SIGNAL(readOnlyChanged( bool )), SLOT(adaptController()) );
  connect( ByteArrayModel, SIGNAL(contentsChanged(int,int)), SLOT(updateRange(int,int)) );
  connect( ByteArrayModel, SIGNAL(contentsChanged(const KHE::KSectionList&)), SLOT(updateRange(const KHE::KSectionList&)) );
  connect( ByteArrayModel, SIGNAL(contentsReplaced(int,int,int)), SLOT(onContentsReplaced(int,int,int)) );
  connect( ByteArrayModel, SIGNAL(contentsReplaced( const QList<KHE::ReplacementScope> & )),
           SLOT(onContentsReplaced( const QList<KHE::ReplacementScope> & )) );
  connect( ByteArrayModel, SIGNAL(contentsSwapped(int,int,int)), SLOT(onContentsSwapped(int,int,int)) );

  KHECore::Bookmarkable *bookmarks = qobject_cast<KHECore::Bookmarkable*>( ByteArrayModel );
  if( bookmarks )
  {
      connect( ByteArrayModel, SIGNAL(bookmarksAdded( const QList<KHECore::KBookmark>& )),
               SLOT(onBookmarksChange(const QList<KHECore::KBookmark>&)) );
      connect( ByteArrayModel, SIGNAL(bookmarksRemoved( const QList<KHECore::KBookmark>& )),
               SLOT(onBookmarksChange(const QList<KHECore::KBookmark>&)) );
  }

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
  Codec = KHECore::KCharCodec::createCodec( (KHECore::KEncoding)DefaultEncoding );
  valueColumn().setCodec( Codec );
  charColumn().setCodec( Codec );
  Encoding = DefaultEncoding;

  TabController = new KTabController( this, 0 );
  Navigator = new KNavigator( this, TabController );
  ValueEditor = new KValueEditor( ValueColumn, BufferCursor, this, Navigator );
  CharEditor = new KCharEditor( CharColumn, BufferCursor, this, Navigator );

  adaptController();

  setFont( KGlobalSettings::fixedFont() );

  connect( CursorBlinkTimer, SIGNAL(timeout()), this, SLOT(blinkCursor()) );
  connect( ScrollTimer,      SIGNAL(timeout()), this, SLOT(autoScrollTimerDone()) );
  connect( DragStartTimer,   SIGNAL(timeout()), this, SLOT(startDrag()) );
  DragStartTimer->setSingleShot( true );
  TrippleClickTimer->setSingleShot( true );

  viewport()->setAcceptDrops( true );
}


KByteArrayView::~KByteArrayView()
{
  delete TabController;
  delete Navigator;
  delete ValueEditor;
  delete CharEditor;

  delete BufferCursor;
  delete BufferRanges;
  delete BufferLayout;

  delete Codec;
  delete CursorPixmaps;
}



const KValueColumn& KByteArrayView::valueColumn()     const { return *ValueColumn; }
const KCharColumn& KByteArrayView::charColumn()       const { return *CharColumn; }
const KDataColumn& KByteArrayView::activeColumn()   const { return *ActiveColumn; }
const KDataColumn& KByteArrayView::inactiveColumn() const { return *InactiveColumn; }

KValueColumn& KByteArrayView::valueColumn()     { return *ValueColumn; }
KCharColumn& KByteArrayView::charColumn()       { return *CharColumn; }
KDataColumn& KByteArrayView::activeColumn()     { return *ActiveColumn; }
KDataColumn& KByteArrayView::inactiveColumn()   { return *InactiveColumn; }

KHECore::KAbstractByteArrayModel *KByteArrayView::byteArrayModel() const { return ByteArrayModel; }
int KByteArrayView::noOfBytesPerLine()               const { return BufferLayout->noOfBytesPerLine(); }
int KByteArrayView::firstLineOffset()                const { return OffsetColumn->firstLineOffset(); }
int KByteArrayView::startOffset()                    const { return BufferLayout->startOffset(); }
KByteArrayView::KResizeStyle KByteArrayView::resizeStyle() const { return ResizeStyle; }
KByteArrayView::KCoding KByteArrayView::coding()           const { return (KByteArrayView::KCoding)valueColumn().coding(); }
KPixelX KByteArrayView::byteSpacingWidth()           const { return valueColumn().byteSpacingWidth(); }
int KByteArrayView::noOfGroupedBytes()               const { return valueColumn().noOfGroupedBytes(); }
KPixelX KByteArrayView::groupSpacingWidth()          const { return valueColumn().groupSpacingWidth(); }
KPixelX KByteArrayView::binaryGapWidth()             const { return valueColumn().binaryGapWidth(); }
bool KByteArrayView::isOverwriteMode()               const { return OverWrite; }
bool KByteArrayView::isOverwriteOnly()               const { return OverWriteOnly; }
bool KByteArrayView::isReadOnly()                    const { return ReadOnly || ByteArrayModel->isReadOnly(); }
bool KByteArrayView::isModified()                    const { return ByteArrayModel->isModified(); }
bool KByteArrayView::tabChangesFocus()               const { return TabController->tabChangesFocus(); }
bool KByteArrayView::showsNonprinting()              const { return charColumn().showsNonprinting(); }
QChar KByteArrayView::substituteChar()               const { return charColumn().substituteChar(); }
QChar KByteArrayView::undefinedChar()                const { return charColumn().undefinedChar(); }
KByteArrayView::KEncoding KByteArrayView::encoding()       const { return (KByteArrayView::KEncoding)Encoding; }
const QString &KByteArrayView::encodingName()        const { return Codec->name(); }
double KByteArrayView::zoomLevel()                    const { return (double)font().pointSize()/DefaultFontSize; }

int KByteArrayView::cursorPosition() const { return BufferCursor->realIndex(); }
bool KByteArrayView::isCursorBehind() const { return BufferCursor->isBehind(); }
KByteArrayView::KDataColumnId KByteArrayView::cursorColumn() const
{ return static_cast<KHEUI::KValueColumn *>( ActiveColumn ) == &valueColumn()? ValueColumnId : CharColumnId; }
KHE::KSection KByteArrayView::selection() const { return BufferRanges->selection(); }

void KByteArrayView::setOverwriteOnly( bool OO )    { OverWriteOnly = OO; if( OverWriteOnly ) setOverwriteMode( true ); }
void KByteArrayView::setModified( bool M )          { ByteArrayModel->setModified(M); }
void KByteArrayView::setTabChangesFocus( bool TCF ) { TabController->setTabChangesFocus(TCF); }
void KByteArrayView::setFirstLineOffset( int FLO )  { OffsetColumn->setFirstLineOffset( FLO ); }

bool KByteArrayView::offsetColumnVisible() const { return OffsetColumn->isVisible(); }
int KByteArrayView::visibleBufferColumns() const
{ return (valueColumn().isVisible() ? ValueColumnId : 0) | (charColumn().isVisible() ? CharColumnId : 0); }


void KByteArrayView::setOverwriteMode( bool OM )
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


void KByteArrayView::setByteArrayModel( KHECore::KAbstractByteArrayModel *B )
{
  ByteArrayModel->disconnect( this );

  ValueEditor->reset();
  CursorPaused = true;

  ByteArrayModel = B;
  valueColumn().set( ByteArrayModel );
  charColumn().set( ByteArrayModel);

  // affected:
  // length -> no of lines -> width
  BufferLayout->setLength( ByteArrayModel->size() );
  adjustLayoutToSize();

  // if the model is readonly make the view too, per default
  if( ByteArrayModel->isReadOnly() )
    setReadOnly( true );

  connect( ByteArrayModel, SIGNAL(readOnlyChanged( bool )), SLOT(adaptController()) );
  connect( ByteArrayModel, SIGNAL(contentsChanged(int,int)), SLOT(updateRange(int,int)) );
  connect( ByteArrayModel, SIGNAL(contentsChanged(const KHE::KSectionList&)), SLOT(updateRange(const KHE::KSectionList&)) );
  connect( ByteArrayModel, SIGNAL(contentsReplaced(int,int,int)), SLOT(onContentsReplaced(int,int,int)) );
  connect( ByteArrayModel, SIGNAL(contentsReplaced( const QList<KHE::ReplacementScope> & )),
           SLOT(onContentsReplaced( const QList<KHE::ReplacementScope> & )) );
  connect( ByteArrayModel, SIGNAL(contentsSwapped(int,int,int)), SLOT(onContentsSwapped(int,int,int)) );

  KHECore::Bookmarkable *bookmarks = qobject_cast<KHECore::Bookmarkable*>( ByteArrayModel );
  if( bookmarks )
  {
      connect( ByteArrayModel, SIGNAL(bookmarksAdded( const QList<KHECore::KBookmark>& )),
               SLOT(onBookmarksChange(const QList<KHECore::KBookmark>&)) );
      connect( ByteArrayModel, SIGNAL(bookmarksRemoved( const QList<KHECore::KBookmark>& )),
               SLOT(onBookmarksChange(const QList<KHECore::KBookmark>&)) );
  }

  viewport()->update();

  BufferCursor->gotoStart();
  ensureCursorVisible();
  unpauseCursor();

  emit cursorPositionChanged( BufferCursor->realIndex() );
}


void KByteArrayView::setStartOffset( int SO )
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
  emit cursorPositionChanged( BufferCursor->realIndex() );
}


void KByteArrayView::setReadOnly( bool RO )
{
    if( ReadOnly == RO )
        return;

    ReadOnly = RO;

    adaptController();
}

void KByteArrayView::adaptController()
{
    const bool isEffectiveReadOnly = ByteArrayModel->isReadOnly() || ReadOnly;

    Controller = isEffectiveReadOnly ?            (KController*)Navigator :
                 cursorColumn() == CharColumnId ? (KController*)CharEditor : (KController*)ValueEditor;
}


void KByteArrayView::setBufferSpacing( KPixelX ByteSpacing, int NoOfGroupedBytes, KPixelX GroupSpacing )
{
  if( !valueColumn().setSpacing(ByteSpacing,NoOfGroupedBytes,GroupSpacing) )
    return;

  updateViewByWidth();
}


void KByteArrayView::setCoding( KCoding C )
{
  uint OldCodingWidth = valueColumn().byteCodec()->encodingWidth();

  if( !valueColumn().setCoding((KHECore::KCoding)C) )
    return;

  uint NewCodingWidth = valueColumn().byteCodec()->encodingWidth();
  ValueEditor->ByteBuffer.resize( NewCodingWidth ); //hack for now

  // no change in the width?
  if( NewCodingWidth == OldCodingWidth )
    updateColumn( valueColumn() );
  else
    updateViewByWidth();

  emit valueCodingChanged( C );
}


void KByteArrayView::setResizeStyle( KResizeStyle NewStyle )
{
  if( ResizeStyle == NewStyle )
    return;

  ResizeStyle = NewStyle;

  updateViewByWidth();
}


void KByteArrayView::setNoOfBytesPerLine( int NoBpL )
{
  // if the number is explicitly set we expect a wish for no automatic resize
  ResizeStyle = NoResize;

  if( !BufferLayout->setNoOfBytesPerLine(NoBpL) )
    return;
  updateViewByWidth();
}


void KByteArrayView::setByteSpacingWidth( int/*KPixelX*/ BSW )
{
  if( !valueColumn().setByteSpacingWidth(BSW) )
    return;
  updateViewByWidth();
}

void KByteArrayView::setNoOfGroupedBytes( int NoGB )
{
  if( !valueColumn().setNoOfGroupedBytes(NoGB) )
    return;
  updateViewByWidth();
}


void KByteArrayView::setGroupSpacingWidth( int/*KPixelX*/ GSW )
{
  if( !valueColumn().setGroupSpacingWidth(GSW) )
    return;
  updateViewByWidth();
}


void KByteArrayView::setBinaryGapWidth( int/*KPixelX*/ BGW )
{
  if( !valueColumn().setBinaryGapWidth(BGW) )
    return;
  updateViewByWidth();
}


void KByteArrayView::setSubstituteChar( QChar SC )
{
  if( !charColumn().setSubstituteChar(SC) )
    return;
  pauseCursor();
  updateColumn( charColumn() );
  unpauseCursor();
}

void KByteArrayView::setUndefinedChar( QChar UC )
{
  if( !charColumn().setUndefinedChar(UC) )
    return;
  pauseCursor();
  updateColumn( charColumn() );
  unpauseCursor();
}

void KByteArrayView::setShowsNonprinting( bool SU )
{
  if( !charColumn().setShowsNonprinting(SU) )
    return;
  pauseCursor();
  updateColumn( charColumn() );
  unpauseCursor();
}


void KByteArrayView::setEncoding( KEncoding C )
{
  if( Encoding == C )
    return;

  KHECore::KCharCodec *NC = KHECore::KCharCodec::createCodec( (KHECore::KEncoding)C );
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

  emit charCodecChanged( NC->name() );
}

// TODO: join with function above!
void KByteArrayView::setEncoding( const QString& EncodingName )
{
  if( EncodingName == Codec->name() )
    return;

  KHECore::KCharCodec *NC = KHECore::KCharCodec::createCodec( EncodingName );
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

  emit charCodecChanged( NC->name() );
}


void KByteArrayView::fontChange( const QFont &OldFont )
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


void KByteArrayView::updateViewByWidth()
{
  pauseCursor();

  adjustToLayoutNoOfBytesPerLine();
  adjustLayoutToSize();

  viewport()->update();

  BufferCursor->updateCoord();
  ensureCursorVisible();

  unpauseCursor();
  emit cursorPositionChanged( BufferCursor->realIndex() );
}


void KByteArrayView::zoomIn()         { zoomIn( DefaultZoomStep ); }
void KByteArrayView::zoomOut()        { zoomOut( DefaultZoomStep ); }

void KByteArrayView::zoomIn( int PointInc )
{
  InZooming = true;
  QFont F( font() );
  F.setPointSize( QFontInfo(F).pointSize() + PointInc );
  setFont( F );
  InZooming = false;
}

void KByteArrayView::zoomOut( int PointDec )
{
  InZooming = true;
  QFont F( font() );
  F.setPointSize( qMax( 1, QFontInfo(F).pointSize() - PointDec ) );
  setFont( F );
  InZooming = false;
}


void KByteArrayView::zoomTo( int PointSize )
{
  InZooming = true;
  QFont F( font() );
  F.setPointSize( PointSize );
  setFont( F );
  InZooming = false;
}


void KByteArrayView::unZoom()
{
  zoomTo( DefaultFontSize );
}

void KByteArrayView::setZoomLevel( double Level )
{
  zoomTo( (int)(Level*DefaultFontSize) );
}


void KByteArrayView::adjustLayoutToSize()
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


void KByteArrayView::adjustToLayoutNoOfBytesPerLine()
{
  OffsetColumn->setDelta( BufferLayout->noOfBytesPerLine() );
  valueColumn().resetXBuffer();
  charColumn().resetXBuffer();

  updateWidths();
}


void KByteArrayView::setNoOfLines( int NewNoOfLines )
{
  KColumnsView::setNoOfLines( NewNoOfLines>1?NewNoOfLines:1 );
}


void KByteArrayView::toggleOffsetColumn( bool Visible )
{
  bool OCVisible = OffsetColumn->isVisible();
  // no change?
  if( OCVisible == Visible )
    return;

  OffsetColumn->setVisible( Visible );
  FirstBorderColumn->setVisible( Visible );

  updateViewByWidth();
}


QSize KByteArrayView::sizeHint() const
{
  return QSize( columnsWidth(), columnsHeight() );
}


QSize KByteArrayView::minimumSizeHint() const
{
  // TODO: better minimal width (visibility!)
  return QSize( OffsetColumn->visibleWidth()+FirstBorderColumn->visibleWidth()+SecondBorderColumn->visibleWidth()+valueColumn().byteWidth()+charColumn().byteWidth(),
                lineHeight() + noOfLines()>1? style()->pixelMetric(QStyle::PM_ScrollBarExtent):0 );
}


void KByteArrayView::resizeEvent( QResizeEvent *ResizeEvent )
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


int KByteArrayView::fittingBytesPerLine() const
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

//         kDebug() << "Left: " << AvailableWidth << "("<<HexByteWidth<<", "<<TextByteWidth<<")" ;

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
    KPixelY NewHeight =  NewNoOfLines * lineHeight();

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
//          kDebug() << "tested without scrollbar..." ;
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
//          kDebug() << "rerun with scrollbar on..." ;
        continue;
      }
    }

    break;
  }

  return FittingBytesPerLine;
}


bool KByteArrayView::selectWord( /*unsigned TODO:change all unneeded signed into unsigned!*/ int Index )
{
  if( Index >= 0 && Index < BufferLayout->length()  )
  {
    KHECore::KWordBufferService WBS( ByteArrayModel, Codec );
    KHE::KSection WordSection = WBS.wordSection( Index );
    if( WordSection.isValid() )
    {
      pauseCursor();

      BufferRanges->setFirstWordSelection( WordSection );
      BufferCursor->gotoIndex( WordSection.behindEnd() );
      updateChanged();

      unpauseCursor();
      emit cursorPositionChanged( BufferCursor->realIndex() );
      return true;
    }
  }
  return false;
}


void KByteArrayView::selectAll( bool Select )
{
  pauseCursor( true );

  if( !Select )
    BufferRanges->removeSelection();
  else
  {
    BufferRanges->setSelection( KHE::KSection(0,BufferLayout->length()-1) );
    BufferCursor->gotoEnd();
  }

  updateChanged();

  unpauseCursor();

  if( !OverWrite ) emit cutAvailable( BufferRanges->hasSelection() );
  emit copyAvailable( BufferRanges->hasSelection() );
  emit selectionChanged( BufferRanges->hasSelection() );
  emit cursorPositionChanged( BufferCursor->realIndex() );
  viewport()->setCursor( isReadOnly() ? Qt::ArrowCursor : Qt::IBeamCursor );
}


bool KByteArrayView::hasSelectedData() const
{
  return BufferRanges->hasSelection();
}


QByteArray KByteArrayView::selectedData() const
{
  if( !BufferRanges->hasSelection() )
    return QByteArray();

  KHE::KSection Selection = BufferRanges->selection();
  QByteArray SD;
  SD.resize( Selection.width() );
  ByteArrayModel->copyTo( SD.data(), Selection.start(), Selection.width() );
  return SD;
}


QMimeData *KByteArrayView::dragObject() const
{
  if( !BufferRanges->hasSelection() )
    return 0;

  const KOffsetColumn *OC;
  const KValueColumn *HC;
  const KCharColumn *TC;
  KCoordRange Range;

  if( static_cast<KHEUI::KCharColumn *>( ActiveColumn ) == &charColumn() )
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

  return new KByteArrayDrag( selectedData(), Range, OC, HC, TC,
                          charColumn().substituteChar(), charColumn().undefinedChar(),
                          Codec->name() );
}


void KByteArrayView::cut()
{
  if( isReadOnly() || OverWrite )
    return;

  QMimeData *Drag = dragObject();
  if( !Drag )
    return;

  QApplication::clipboard()->setMimeData( Drag, ClipboardMode );

  removeSelectedData();
}


void KByteArrayView::copy()
{
  QMimeData *Drag = dragObject();
  if( !Drag )
    return;

  QApplication::clipboard()->setMimeData( Drag, ClipboardMode );
}


void KByteArrayView::paste()
{
  if( isReadOnly() )
    return;

  const QMimeData *Data = QApplication::clipboard()->mimeData( ClipboardMode );
  pasteFromSource( Data );
}


void KByteArrayView::pasteFromSource( const QMimeData *Data )
{
  if( !Data )
    return;

  QByteArray Bytes = Data->data( "application/octet-stream" );

  if( !Bytes.isEmpty() )
    insert( Bytes );
}


void KByteArrayView::insert( const QByteArray &Data )
{
  pauseCursor( true );

  if( OverWrite )
  {
    if( BufferRanges->hasSelection() )
    {
      // replacing the selection:
      // we restrict the replacement to the minimum length of selection and input
      KHE::KSection Selection = BufferRanges->removeSelection();
      Selection.restrictEndByWidth( Data.size() );
      ByteArrayModel->replace( Selection, Data.data(), Selection.width() );
    }
    else
    {
      if( !BufferCursor->isBehind() )
      {
        // replacing the normal data, at least until the end
        KHE::KSection InsertRange = KHE::KSection::fromWidth( BufferCursor->realIndex(), Data.size() );
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
      KHE::KSection Selection = BufferRanges->removeSelection();
      ByteArrayModel->replace( Selection, Data );
    }
    else
      ByteArrayModel->insert( BufferCursor->realIndex(), Data );
  }

  emit selectionChanged( BufferRanges->hasSelection() );
}


void KByteArrayView::removeSelectedData()
{
  // Can't we do this?
  if( isReadOnly() || OverWrite || ValueEditor->isInEditMode() )
    return;

  KHE::KSection Selection = BufferRanges->removeSelection();

  ByteArrayModel->remove( Selection );

//     clearUndoRedo();

  //emit selectionChanged( -1, -1 );
}


void KByteArrayView::updateRange( int Start, int End )
{
  BufferRanges->addChangedRange( Start, End );
// kDebug() << "update: "<<Start<<","<<End;

  unpauseCursor();
  updateChanged();
}


void KByteArrayView::updateRange( const KHE::KSectionList &list )
{
    for( KHE::KSectionList::ConstIterator it=list.begin(); it!=list.end(); ++it )
        BufferRanges->addChangedRange( *it );

    unpauseCursor();
    updateChanged();
}


void KByteArrayView::onContentsReplaced( int Pos, int RemovedLength, int InsertedLength )
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

// kDebug()<< "Pos:"<<Pos<<", RemovedLength:"<<RemovedLength<<", InsertedLength:"<<InsertedLength;
// kDebug() << "Cursor:"<<BufferCursor->index()<<", "<<BufferCursor->isBehind();
  // adapt cursor(s)
  if( Appending )
    BufferCursor->gotoEnd();
  else
    BufferCursor->adaptToChange( Pos, RemovedLength, InsertedLength );

  BufferRanges->adaptSelectionToChange( Pos, RemovedLength, InsertedLength );
// kDebug() << "Cursor:"<<BufferCursor->index()<<", Selection:"<<BufferRanges->selectionStart()<<"-"<<BufferRanges->selectionEnd()
//          <<", BytesPerLine: "<<BufferLayout->noOfBytesPerLine()<<endl;
  emit cursorPositionChanged( BufferCursor->realIndex() );
}

void KByteArrayView::onContentsReplaced( const QList<KHE::ReplacementScope> &replacementList )
{
    pauseCursor();

    const bool Appending = BufferCursor->atAppendPos();
    const int oldLength = BufferLayout->length(); // TODO: hack for BufferCursor->adaptSelectionToChange
    // update lengths
    int OldNoOfLines = noOfLines();
    BufferLayout->setLength( ByteArrayModel->size() );
    int NewNoOfLines = BufferLayout->noOfLines();
    if( OldNoOfLines != NewNoOfLines )
    {
        setNoOfLines( NewNoOfLines );
        updateColumn( *OffsetColumn );
    }

    // adapt cursor(s)
    if( Appending )
        BufferCursor->gotoEnd();
    else
        BufferCursor->adaptToChange( replacementList, oldLength );

    BufferRanges->adaptSelectionToChange( replacementList );
    // kDebug() << "Cursor:"<<BufferCursor->index()<<", Selection:"<<BufferRanges->selectionStart()<<"-"<<BufferRanges->selectionEnd()
    //          <<", BytesPerLine: "<<BufferLayout->noOfBytesPerLine()<<endl;
    emit cursorPositionChanged( BufferCursor->realIndex() );
}

void KByteArrayView::onContentsSwapped( int firstStart, int secondStart, int secondLength )
{
Q_UNUSED( firstStart )
Q_UNUSED( secondStart )
Q_UNUSED( secondLength )
  // TODO: what should happen here? Well, relocate the cursor perhaps?
  pauseCursor();

}
void KByteArrayView::onBookmarksChange( const QList<KHECore::KBookmark> &bookmarks )
{

  foreach( KHECore::KBookmark bookmark, bookmarks )
  {
      const int position = bookmark.offset();
      BufferRanges->addChangedRange( position, position );
  }

  unpauseCursor();
  updateChanged();
}

void KByteArrayView::clipboardChanged()
{
  // don't listen to selection changes
  disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()) );
  selectAll( false );
}


void KByteArrayView::setCursorPosition( int Index, bool Behind )
{
  pauseCursor( true );

  if( Behind ) --Index;
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
    emit selectionChanged( BufferRanges->hasSelection() );
  }
  ensureCursorVisible();

  unpauseCursor();
  emit cursorPositionChanged( BufferCursor->realIndex() );
}

void KByteArrayView::setSelection( int Start, int End )
{
  if( Start >= 0 && End < BufferLayout->length()  )
  {
    KHE::KSection Selection( Start, End );
    if( Selection.isValid() )
    {
      pauseCursor();

      BufferRanges->setSelection( Selection );
      BufferCursor->gotoCIndex( Selection.behindEnd() );
      ensureVisible( activeColumn(), BufferLayout->coordOfIndex(Selection.start()) );
      ensureCursorVisible();
      updateChanged();

      unpauseCursor();
      emit cursorPositionChanged( BufferCursor->realIndex() );
    }
  }
}

void KByteArrayView::showBufferColumns( int CCs )
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
    KDataColumn *H = ActiveColumn;
    ActiveColumn = InactiveColumn;
    InactiveColumn = H;
    adaptController();
  }

  updateViewByWidth();
}


void KByteArrayView::setCursorColumn( KDataColumnId CC )
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
  adaptController();

  ensureCursorVisible();
  unpauseCursor();
}


void KByteArrayView::placeCursor( const QPoint &Point )
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
  adaptController();

  // get coord of click and whether this click was closer to the end of the pos
  KCoord C( activeColumn().magPosOfX(Point.x()), lineAt(Point.y()) );

  BufferCursor->gotoCCoord( C );
  emit cursorPositionChanged( BufferCursor->realIndex() );
}


int KByteArrayView::indexByPoint( const QPoint &Point ) const
{
  const KDataColumn *C;
  if( charColumn().isVisible() && Point.x() >= charColumn().x() )
    C = &charColumn();
  else
    C = &valueColumn();

  KCoord Coord( C->posOfX(Point.x()), lineAt(Point.y()) );

  return BufferLayout->indexAtCCoord( Coord );
}


void KByteArrayView::showEvent( QShowEvent *Event )
{
    KColumnsView::showEvent( Event );
    BufferLayout->setNoOfLinesPerPage( noOfLinesPerPage() );
}


void KByteArrayView::focusInEvent( QFocusEvent *focusEvent )
{
    KColumnsView::focusInEvent( focusEvent );
    startCursor();
}

void KByteArrayView::focusOutEvent( QFocusEvent *focusEvent )
{
    stopCursor();
    KColumnsView::focusOutEvent( focusEvent );
}


void KByteArrayView::blinkCursor()
{
  // skip the cursor drawing?
  if( CursorPaused || ValueEditor->isInEditMode() )
    return;

  // switch the cursor state
  BlinkCursorVisible = !BlinkCursorVisible;
  updateCursor( activeColumn() );
}


void KByteArrayView::startCursor()
{
  CursorPaused = false;

  updateCursors();

  CursorBlinkTimer->start( QApplication::cursorFlashTime()/2 );
}


void KByteArrayView::unpauseCursor()
{
  CursorPaused = false;

  if( CursorBlinkTimer->isActive() )
    updateCursors();
}


void KByteArrayView::updateCursors()
{
  createCursorPixmaps();

  BlinkCursorVisible = true;
  updateCursor( activeColumn() );
  updateCursor( inactiveColumn() );
}


void KByteArrayView::stopCursor()
{
  CursorBlinkTimer->stop();

  pauseCursor();
}


void KByteArrayView::pauseCursor( bool LeaveEdit )
{
  CursorPaused = true;

  BlinkCursorVisible = false;
  updateCursor( activeColumn() );
  updateCursor( inactiveColumn() );

  if( LeaveEdit )
    ValueEditor->InEditMode = false;
}


void KByteArrayView::updateCursor( const KDataColumn &Column )
{
  int x = Column.xOfPos( BufferCursor->pos() ) - xOffset();
  int y = lineHeight() * BufferCursor->line() - yOffset();
  int w = Column.byteWidth();

  viewport()->update( x,y, w,lineHeight() );
}

void KByteArrayView::createCursorPixmaps()
{
  // create CursorPixmaps
  CursorPixmaps->setSize( activeColumn().byteWidth(), lineHeight() );

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


void KByteArrayView::drawActiveCursor( QPainter *painter )
{
  // any reason to skip the cursor drawing?
  if( BlinkCursorVisible && !hasFocus() && !viewport()->hasFocus() && !InDnD )
    return;

  const int x = activeColumn().xOfPos( BufferCursor->pos() );
  const int y = lineHeight() * BufferCursor->line();

  painter->translate( x, y );

  // paint edited byte?
  if( ValueEditor->isInEditMode() )
  {
    int Index = BufferCursor->index();

    if( BlinkCursorVisible )
      valueColumn().paintEditedByte( painter, ValueEditor->EditValue, ValueEditor->ByteBuffer );
    else
      valueColumn().paintByte( painter, Index );
  }
  else
    painter->drawPixmap( CursorPixmaps->cursorX(), 0,
                         BlinkCursorVisible?CursorPixmaps->onPixmap():CursorPixmaps->offPixmap(),
                         CursorPixmaps->cursorX(),0,CursorPixmaps->cursorW(),-1 );

  painter->translate( -x, -y );
}


void KByteArrayView::drawInactiveCursor( QPainter *painter )
{
  // any reason to skip the cursor drawing?
  if( !inactiveColumn().isVisible()
      || CursorPaused
      || (!CursorPaused && !hasFocus() && !viewport()->hasFocus() && !InDnD)  )
    return;

  int Index = BufferCursor->validIndex();

  const int x = inactiveColumn().xOfPos( BufferCursor->pos() );
  const int y = lineHeight() * BufferCursor->line();

  painter->translate( x, y );

  KDataColumn::KFrameStyle Style =
    BufferCursor->isBehind() ? KDataColumn::Right :
    (OverWrite||ValueEditor->isInEditMode()) ? KDataColumn::Frame :
    KDataColumn::Left;
  inactiveColumn().paintFramedByte( painter, Index, Style );

  painter->translate( -x, -y );
}


void KByteArrayView::drawColumns( QPainter *painter, int cx, int cy, int cw, int ch )
{
  KColumnsView::drawColumns( painter, cx, cy, cw, ch );
  // TODO: update non blinking cursors. Should this perhaps be done in the buffercolumn?
  // Then it needs to know about inactive, insideByte and the like... well...
  // perhaps subclassing the buffer columns even more, to KCharColumn and KValueColumn?

  if( visibleLines(KPixelYs::fromWidth(cy,ch)).includes(BufferCursor->line()) )
  {
    drawActiveCursor( painter );
    drawInactiveCursor( painter );
  }
}

void KByteArrayView::updateColumn( KColumn &Column )
{
  if( Column.isVisible() )
    viewport()->update( Column.x()-xOffset(), 0, Column.width(), visibleHeight() );
}

void KByteArrayView::emitSelectionSignals()
{
  bool HasSelection = BufferRanges->hasSelection();
  if( !OverWrite ) emit cutAvailable( HasSelection );
  emit copyAvailable( HasSelection );
  emit selectionChanged( HasSelection );
}


void KByteArrayView::keyPressEvent( QKeyEvent *KeyEvent )
{
  if( !Controller->handleKeyPress( KeyEvent ) )
    KeyEvent->ignore();
}


void KByteArrayView::updateChanged()
{
  KPixelXs Xs = KPixelXs::fromWidth( xOffset(), visibleWidth() );

  // collect affected buffer columns
  QList<KDataColumn*> DirtyColumns;

  KDataColumn *C = ValueColumn;
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
    KHE::KSection FullPositions( 0, BufferLayout->noOfBytesPerLine()-1 );
    KCoordRange VisibleRange( FullPositions, visibleLines(Ys) );

    KCoordRange ChangedRange;
    // as there might be multiple selections on this line redo until no more is changed
    while( hasChanged(VisibleRange,&ChangedRange) )
    {
      KPixelY cy = ChangedRange.start().line() * lineHeight() - yOffset();

      QListIterator<KDataColumn*> it( DirtyColumns );
      // only one line?
      if( ChangedRange.start().line() == ChangedRange.end().line() )
      {
        while( it.hasNext() )
        {
          KPixelXs XPixels =
            it.next()->wideXPixelsOfPos( KHE::KSection(ChangedRange.start().pos(),ChangedRange.end().pos()) );

          viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), lineHeight() );
        }
      }
      //
      else
      {
        // first line
        while( it.hasNext() )
        {
          KPixelXs XPixels =
            it.next()->wideXPixelsOfPos( KHE::KSection(ChangedRange.start().pos(),FullPositions.end()) );

          viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), lineHeight() );
        }

        // at least one full line?
        for( int l = ChangedRange.start().line()+1; l < ChangedRange.end().line(); ++l )
        {
          cy += lineHeight();
          it.toFront();
          while( it.hasNext() )
          {
            KPixelXs XPixels =
              it.next()->wideXPixelsOfPos( FullPositions );

            viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), lineHeight() );
          }
        }

        // last line
        cy += lineHeight();
        it.toFront();
        while( it.hasNext() )
        {
          KPixelXs XPixels =
            it.next()->wideXPixelsOfPos( KHE::KSection(FullPositions.start(),ChangedRange.end().pos()) );

          viewport()->update( XPixels.start()-xOffset(), cy, XPixels.width(), lineHeight() );
        }
      }

      // continue the search at the overnext index
      VisibleRange.setStart( ChangedRange.end()+1 ); //+2 ); TODO: currently bounding ranges are not merged
      if( !VisibleRange.isValid() )
        break;
    }
  }

  BufferRanges->resetChangedRanges();
}


bool KByteArrayView::hasChanged( const KCoordRange &VisibleRange, KCoordRange *ChangedRange ) const
{
  if( !BufferRanges->overlapsChanges(VisibleRange,ChangedRange) )
    return false;

  ChangedRange->restrictTo( VisibleRange );
  return true;
}


void KByteArrayView::ensureCursorVisible()
{
//   // Not visible or the user is dragging the window, so don't position to caret yet
//   if ( !isVisible() || isHorizontalSliderPressed() || isVerticalSliderPressed() )
//   {
//     d->ensureCursorVisibleInShowEvent = true;
//     return;
//   }
  //static const int Margin = 10;
    ensureVisible( activeColumn(), BufferCursor->coord() );
}

void KByteArrayView::ensureVisible( const KDataColumn &Column, const KCoord &Coord )
{

  // TODO: add getCursorRect to BufferColumn
  const KPixelXs cursorXs = KPixelXs::fromWidth( Column.xOfPos(Coord.pos()),
                                                 Column.byteWidth() );

  const KPixelYs cursorYs = KPixelYs::fromWidth( lineHeight()*Coord.line(), lineHeight() );

  const KPixelXs visibleXs = KPixelXs::fromWidth( xOffset(), visibleWidth() );
  const KPixelYs visibleYs = KPixelXs::fromWidth( yOffset(), visibleHeight() );

  horizontalScrollBar()->setValue( visibleXs.startForInclude(cursorXs) );
  verticalScrollBar()->setValue( visibleYs.startForInclude(cursorYs) );
}


void KByteArrayView::mousePressEvent( QMouseEvent *Event )
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
      emit cursorPositionChanged( BufferCursor->realIndex() );
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


void KByteArrayView::mouseMoveEvent( QMouseEvent *Event )
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


void KByteArrayView::mouseReleaseEvent( QMouseEvent *Event )
{
  const QPoint ReleasePoint = viewportToColumns( Event->pos() );

  // this is not the release of a doubleclick so we need to process it?
  if( !InDoubleClick )
  {
    int Line = lineAt( ReleasePoint.y() );
    int Pos = activeColumn().posOfX( ReleasePoint.x() ); // TODO: can we be sure here about the active column?
    int Index = BufferLayout->indexAtCCoord( KCoord(Pos,Line) ); // TODO: can this be another index than the one of the cursor???
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
        disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()) );

        copy();

        //TODO: why did we do this? And why does the disconnect above not work?
        // got connected multiple times after a few selections by mouse
//         connect( QApplication::clipboard(), SIGNAL(selectionChanged()), SLOT(clipboardChanged()) );
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

  emit cursorPositionChanged( BufferCursor->realIndex() );

  InDoubleClick = false;

  if( BufferRanges->selectionJustStarted() )
    BufferRanges->removeSelection();

  if( !OverWrite ) emit cutAvailable( BufferRanges->hasSelection() );
  emit copyAvailable( BufferRanges->hasSelection() );
  emit selectionChanged( BufferRanges->hasSelection() );
}


// gets called after press and release instead of a plain press event (?)
void KByteArrayView::mouseDoubleClickEvent( QMouseEvent *Event )
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


void KByteArrayView::autoScrollTimerDone()
{
  if( MousePressed )
    handleMouseMove( viewportToColumns(viewport()->mapFromGlobal( QCursor::pos() )) );
}


void KByteArrayView::handleMouseMove( const QPoint& Point ) // handles the move of the mouse with pressed buttons
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
    KHE::KSection FirstWordSelection = BufferRanges->firstWordSelection();
    KHECore::KWordBufferService WBS( ByteArrayModel, Codec );
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
  emit cursorPositionChanged( BufferCursor->realIndex() );
}


void KByteArrayView::startDrag()
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
  Qt::DropAction dropAction = drag->exec( request );

  if( dropAction == Qt::MoveAction )
    // Not inside this widget itself?
    if( drag->target() != this && drag->target() != viewport() )
      removeSelectedData();

  delete drag;
}


void KByteArrayView::dragEnterEvent( QDragEnterEvent *event )
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


void KByteArrayView::dragMoveEvent( QDragMoveEvent *event )
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


void KByteArrayView::dragLeaveEvent( QDragLeaveEvent * )
{
  // bye... and thanks for all the cursor movement...
  InDnD = false;
}



void KByteArrayView::dropEvent( QDropEvent *Event )
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


void KByteArrayView::handleInternalDrag( QDropEvent *Event )
{
  // get drag origin
  KHE::KSection Selection = BufferRanges->removeSelection();
  int InsertIndex = BufferCursor->realIndex();

  // is this a move?
  if( Event->proposedAction() == Qt::MoveAction )
  {
    // ignore the copy hold in the event but only move
    int newCursorIndex;
    // need to swap?
    if( Selection.end() < InsertIndex )
    {
      newCursorIndex = InsertIndex;
      const int firstIndex = Selection.start();
      Selection.set( Selection.behindEnd(), InsertIndex-1 );
      InsertIndex = firstIndex;
    }
    else
      newCursorIndex = InsertIndex + Selection.width();

    const bool success = ByteArrayModel->swap( InsertIndex, Selection );
    if( success )
    {
      BufferCursor->gotoCIndex( newCursorIndex );
      BufferRanges->addChangedRange( KHE::KSection(InsertIndex,Selection.end()) );
      emit cursorPositionChanged( BufferCursor->realIndex() );
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
          KHE::KSection OverwriteRange = KHE::KSection::fromWidth( InsertIndex, Data.size() );
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


void KByteArrayView::wheelEvent( QWheelEvent *Event )
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
void KByteArrayView::contextMenuEvent( QContextMenuEvent *Event )
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
      disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()));

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

}
