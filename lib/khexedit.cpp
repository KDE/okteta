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

 
// c specific
#include <stdlib.h>
#include <limits.h>
// c++ specific
//#include <limits>
#include <iostream>
// qt specific
#include <qstyle.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qcursor.h>
#include <qapplication.h>
// kde specific
#ifndef QT_ONLY
#include <kglobalsettings.h>
#endif
//#include <kapplication.h>
// app specific
#include "kdatabuffer.h"
#include "koffsetcolumn.h"
#include "kbuffercolumn.h"
#include "kbordercolumn.h"
#include "kbuffercursor.h"
#include "kbufferlayout.h"
#include "kbufferranges.h"
#include "kbufferdrag.h"
#include "kcursor.h"
#include "kbytecodec.h"
#include "khexedit.h"

using namespace KHE;

static const int NoOfColumns = 3;

/** zooming is done in steps of font size points */
static const int DefaultZoomStep = 1;
static const int DefaultStartOffset = 0;//5;
static const int DefaultFirstLineOffset = 0;
static const int DefaultNoOfBytesPerLine =  16;
static const KHexEdit::KResizeStyle DefaultResizeStyle = KHexEdit::FullSizeUsage;
static const int DefaultScrollTimerPeriod = 100;



KHexEdit::KHexEdit( KDataBuffer *Buffer, QWidget *Parent, const char *Name, WFlags Flags )
 : KColumnsView( /*kapp->reverseLayout(),*/ Parent, Name, Flags ),
   DataBuffer( Buffer ),
   BufferLayout( new KBufferLayout(DefaultNoOfBytesPerLine,DefaultStartOffset,0) ),
   BufferCursor( new KBufferCursor(BufferLayout) ),
   BufferRanges( new KBufferRanges(BufferLayout) ),
   CursorBlinkTimer( new QTimer(this) ),
   ScrollTimer( new QTimer(this) ),
   DragStartTimer( new QTimer(this) ),
   TrippleClickTimer( new QTimer(this) ),
   CursorPixmaps( new KCursor() ),
   ByteBuffer( new char[KByteCodec::MaxCodingWidth] ),
   ClipboardMode( QClipboard::Clipboard ),
   ResizeStyle( DefaultResizeStyle ),
   TabChangesFocus( false ),
   ReadOnly( true ),
//    Modified( false ),
   OverWriteOnly( false ),
   OverWrite( true ),
   InDnD( false ),
   DragStartPossible( false ),
   CursorHidden( false ),
   BlinkCursorVisible( false ),
   Zooming( false ),
   d( 0 )
{
  if( DataBuffer )
    BufferLayout->setLength( DataBuffer->size() );

  // creating the columns in the needed order
  OffsetColumn = new KOffsetColumn( this, DefaultFirstLineOffset, DefaultNoOfBytesPerLine, KOffsetFormat::Hexadecimal );
  BorderColumn[0] = new KBorderColumn( this );
  BufferColumn[0] = new KBufferColumn( this, DataBuffer, BufferLayout, BufferRanges );
  BorderColumn[1] = new KBorderColumn( this );
  BufferColumn[1] = new KBufferColumn( this, DataBuffer, BufferLayout, BufferRanges );

  textColumn().setCoding( KHE::ASCIICoding );
  textColumn().setSpacing( 0,0,0 );

  // select the active column
  ActiveColumn = &textColumn();
  InactiveColumn = &hexColumn();

#ifdef QT_ONLY
  QFont FixedFont( "fixed", 10 );
  FixedFont.setFixedPitch( true );
  setFont( FixedFont );
#else
  setFont( KGlobalSettings::fixedFont() );
#endif

  //
  viewport()->setFocusProxy( this );
  viewport()->setFocusPolicy( WheelFocus );

  viewport()->installEventFilter( this );
  installEventFilter( this );

  connect( CursorBlinkTimer, SIGNAL(timeout()), this, SLOT(blinkCursor()));
  connect( ScrollTimer,      SIGNAL(timeout()), this, SLOT(autoScrollTimerDone()) );
  connect( DragStartTimer,   SIGNAL(timeout()), this, SLOT(startDrag()) );
}


KHexEdit::~KHexEdit()
{
  delete [] ByteBuffer;
}


int KHexEdit::noOfBytesPerLine()               const { return BufferLayout->noOfBytesPerLine(); }
int KHexEdit::firstLineOffset()                const { return OffsetColumn->firstLineOffset(); }
int KHexEdit::startOffset()                    const { return BufferLayout->startOffset(); }
KHexEdit::KResizeStyle KHexEdit::resizeStyle() const { return ResizeStyle; }
KHexEdit::KCoding KHexEdit::coding()           const { return (KHexEdit::KCoding)hexColumn().coding(); }
KPixelX KHexEdit::byteSpacingWidth()           const { return hexColumn().byteSpacingWidth(); }
int KHexEdit::noOfGroupedBytes()               const { return hexColumn().noOfGroupedBytes(); }
KPixelX KHexEdit::groupSpacingWidth()          const { return hexColumn().groupSpacingWidth(); }
KPixelX KHexEdit::binaryGapWidth()             const { return hexColumn().binaryGapWidth(); }
bool KHexEdit::isOverwriteMode()               const { return OverWrite; }
bool KHexEdit::isOverwriteOnly()               const { return OverWriteOnly; }
bool KHexEdit::isReadOnly()                    const { return ReadOnly; }
bool KHexEdit::isModified()                    const { return DataBuffer->isModified(); }
bool KHexEdit::tabChangesFocus()               const { return TabChangesFocus; }

void KHexEdit::setOverwriteOnly( bool OO )    { OverWriteOnly = OO; if( OverWriteOnly ) setOverwriteMode( true ); }
void KHexEdit::setModified( bool M )          { DataBuffer->setModified(M); }
void KHexEdit::setTabChangesFocus( bool TCF ) { TabChangesFocus = TCF; }
void KHexEdit::setFirstLineOffset( int FLO )  { OffsetColumn->setFirstLineOffset( FLO ); }


void KHexEdit::setOverwriteMode( bool OM )
{
  if( OverWriteOnly && !OM )
    return;

  OverWrite = OM;

  bool NCH = !CursorHidden;
  if( NCH )
    pauseCursorBlinking();
  BufferCursor->setNewPosAllowed( !OverWrite );
  if( NCH )
    unpauseCursorBlinking();

  emit cutAvailable( !OverWrite && BufferRanges->hasSelection() );
}


void KHexEdit::setDataBuffer( KDataBuffer *B )
{
  DataBuffer = B;

  BufferLayout->setLength( DataBuffer->size() );

  hexColumn().set( DataBuffer );
  textColumn().set( DataBuffer);

  fitInLine();
  updateWidths();

  BufferLayout->setNoOfLinesPerPage( noOfLinesPerPage() );

  // ensure that the widget is readonly if the buffer is TODO
//   if( DataBuffer->isReadOnly() && !isReadOnly() )
//     setReadOnly( true );

  repaintView();
}


void KHexEdit::setStartOffset( int SO )
{
  if( !BufferLayout->setStartOffset(SO) )
    return;

  fitInLine();
  updateWidths();

  repaintView();
}


void KHexEdit::setReadOnly( bool RO )
{
  ReadOnly = RO;
}


void KHexEdit::setBufferSpacing( KPixelX ByteSpacing, int NoOfGroupedBytes, KPixelX GroupSpacing )
{
  if( !hexColumn().setSpacing(ByteSpacing,NoOfGroupedBytes,GroupSpacing) )
    return;
  updateViewByWidth();
}


void KHexEdit::setCoding( KCoding C )
{
  if( !hexColumn().setCoding((KHE::KCoding)C) )
    return;

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
  if( !hexColumn().setByteSpacingWidth(BSW) )
    return;
  updateViewByWidth();
}

void KHexEdit::setNoOfGroupedBytes( int NoGB )
{
  if( !hexColumn().setNoOfGroupedBytes(NoGB) )
    return;
  updateViewByWidth();
}


void KHexEdit::setGroupSpacingWidth( int/*KPixelX*/ GSW )
{
  if( !hexColumn().setGroupSpacingWidth(GSW) )
    return;
  updateViewByWidth();
}


void KHexEdit::setBinaryGapWidth( int/*KPixelX*/ BGW )
{
  if( !hexColumn().setBinaryGapWidth(BGW) )
    return;
  updateViewByWidth();
}


void KHexEdit::fontChange( const QFont &/*OldFont*/ )
{
  if( !Zooming )
    DefaultFontSize = font().pointSize();

  QFontMetrics FM( fontMetrics() );
  KPixelX DigitWidth = FM.maxWidth();
  KPixelY DigitBaseLine = FM.ascent();

  setLineHeight( FM.height() );
  BufferLayout->setNoOfLinesPerPage( noOfLinesPerPage() ); // needed here if content is smaller then a page

  offsetColumn().setMetrics( DigitWidth, DigitBaseLine );
  hexColumn().setMetrics( DigitWidth, DigitBaseLine );
  textColumn().setMetrics( DigitWidth, DigitBaseLine );

  updateViewByWidth();
}


void KHexEdit::updateViewByWidth()
{
  pauseCursorBlinking();
  fitToNoOfBytesPerLine();
  updateWidths();

  repaintView();

  BufferCursor->updateCoord();
  ensureCursorVisible();
  unpauseCursorBlinking();
}


void KHexEdit::zoomIn()         { zoomIn( DefaultZoomStep ); }
void KHexEdit::zoomOut()        { zoomOut( DefaultZoomStep ); }

void KHexEdit::zoomIn( int PointInc )
{
  Zooming = true;
  QFont F( font() );
  F.setPointSize( QFontInfo(F).pointSize() + PointInc );
  setFont( F );
  Zooming = false;
}

void KHexEdit::zoomOut( int PointDec )
{
  Zooming = true;
  QFont F( font() );
  F.setPointSize( QMAX( 1, QFontInfo(F).pointSize() - PointDec ) );
  setFont( F );
  Zooming = false;
}


void KHexEdit::zoomTo( int PointSize )
{
  Zooming = true;
  QFont F( font() );
  F.setPointSize( PointSize );
  setFont( F );
  Zooming = false;
}


void KHexEdit::unZoom()
{
  zoomTo( DefaultFontSize );
}


void KHexEdit::fitInLine()
{
  if( ResizeStyle != NoResize )
  {
    int FittingBytesPerLine = fittingBytesPerLine( size() );

//     std::cout<<"FitBpL"<<FittingBytesPerLine<<std::endl;

    // changes?
    if( BufferLayout->setNoOfBytesPerLine(FittingBytesPerLine) )
      fitToNoOfBytesPerLine();
  }
}


void KHexEdit::fitToNoOfBytesPerLine()
{
  offsetColumn().setDelta( BufferLayout->noOfBytesPerLine() );
  hexColumn().resetXBuffer();
  textColumn().resetXBuffer();

  setNoOfLines( BufferLayout->noOfLines() );
}


QSize KHexEdit::sizeHint() const
{
  return QSize( totalWidth(), totalHeight() );
}


QSize KHexEdit::minimumSizeHint() const
{
  // TODO: better minimal width (visibility!)
  return QSize( offsetColumn().visibleWidth()+BorderColumn[0]->visibleWidth()+BorderColumn[1]->visibleWidth()+hexColumn().byteWidth()+textColumn().byteWidth(),
                lineHeight() + noOfLines()>1? style().pixelMetric(QStyle::PM_ScrollBarExtent):0 );
}


void KHexEdit::resizeEvent( QResizeEvent *ResizeEvent )
{
  BufferLayout->setNoOfLinesPerPage( noOfLinesPerPage() );

  if( ResizeStyle != NoResize )
  {
    int FittingBytesPerLine = fittingBytesPerLine( ResizeEvent->size() );

    // changes?
    if( BufferLayout->setNoOfBytesPerLine(FittingBytesPerLine) )
    {
      setNoOfLines( BufferLayout->noOfLines() );
      updateViewByWidth();
    }
  }

  QScrollView::resizeEvent( ResizeEvent );
}


int KHexEdit::fittingBytesPerLine( const QSize &NewSize ) const
{
  KPixelX ReservedWidth = OffsetColumn->visibleWidth() + BorderColumn[0]->visibleWidth() + BorderColumn[1]->visibleWidth();

  // abstract framewidth as well as offset and border columns width
  int UsedbyFrameWidth = 2 * frameWidth();
  KPixelX FullWidth = NewSize.width() - UsedbyFrameWidth - ReservedWidth;

//  // no width left for resizeable columns? TODO: put this in resizeEvent
//  if( FullWidth < 0 )
//    return;

  KPixelY FullHeight = NewSize.height() - UsedbyFrameWidth;

  // check influence of dis-/appearing of the vertical scrollbar
  bool VerticalScrollbarIsVisible = verticalScrollBar()->isVisible();
  KPixelX ScrollbarExtent = style().pixelMetric( QStyle::PM_ScrollBarExtent );//verticalScrollBar()->width();

  KPixelX AvailableWidth = FullWidth;
  if( VerticalScrollbarIsVisible )
    AvailableWidth -= ScrollbarExtent;

  enum KMatchTrial { FirstRun, RerunWithScrollbarOn, TestWithoutScrollbar };
  KMatchTrial MatchRun = FirstRun;

  // prepare needed values
  KPixelX DigitWidth = hexColumn().digitWidth();
  KPixelX TextByteWidth = textColumn().isVisible() ? DigitWidth : 0;
  KPixelX HexByteWidth = hexColumn().byteWidth();
  KPixelX ByteSpacingWidth = hexColumn().byteSpacingWidth();
  KPixelX GroupSpacingWidth;
  int NoOfGroupedBytes = hexColumn().noOfGroupedBytes();
  // no grouping?
  if( NoOfGroupedBytes == 0 )
  {
    // faking grouping by 1
    NoOfGroupedBytes = 1;
    GroupSpacingWidth = 0;
  }
  else
    GroupSpacingWidth = hexColumn().groupSpacingWidth();

  KPixelX HexByteGroupWidth =  NoOfGroupedBytes * HexByteWidth + (NoOfGroupedBytes-1)*ByteSpacingWidth;
  KPixelX TextByteGroupWidth = NoOfGroupedBytes * TextByteWidth;
  KPixelX TotalGroupWidth = HexByteGroupWidth + GroupSpacingWidth + TextByteGroupWidth;

  int FittingBytesPerLine;
  int WithScrollbarFittingBytesPerLine = 0;
  for(;;)
  {
//    std::cout << "matchWidth: " << FullWidth
//              << " (v:" << visibleWidth()
//              << ", f:" << frameWidth()
//              << ", A:" << AvailableWidth
//              << ", S:" << ScrollbarExtent
//              << ", R:" << ReservedWidth << ")" << std::endl;

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

//        std::cout << "Left: " << AvailableWidth << "("<<HexByteWidth<<", "<<TextByteWidth<<")" << std::endl;

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

//    std::cout << "meantime: " << FittingGroupsPerLine << " (T:" << TotalGroupWidth
//              << ", h:" << HexByteGroupWidth
//              << ", t:" << TextByteGroupWidth
//              << ", s:" << GroupSpacingWidth << ") " <<FittingBytesPerLine<< std::endl;

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
//          std::cout << "tested without scrollbar..." << std::endl;
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
//          std::cout << "rerun with scrollbar on..." << std::endl;
        continue;
      }
    }

    break;
  }

  return FittingBytesPerLine;
}


void KHexEdit::selectAll( bool Select )
{
  pauseCursorBlinking();
  if( !Select )
    BufferRanges->removeSelection();
  else
  {
    BufferRanges->setSelection( KSection(0,BufferLayout->length()-1) );
    BufferCursor->gotoEnd();
  }
  unpauseCursorBlinking();

  repaintChanged();

  if( !OverWrite ) emit cutAvailable( BufferRanges->hasSelection() );
  emit copyAvailable( BufferRanges->hasSelection() );
  emit selectionChanged();
  viewport()->setCursor( isReadOnly() ? arrowCursor : ibeamCursor );
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
  QByteArray SD( Selection.width() );
  DataBuffer->copyTo( SD.data(), Selection.start(), Selection.width() );
  return SD;
}


KBufferDrag *KHexEdit::dragObject( QWidget *Parent ) const
{
  if( !BufferRanges->hasSelection() )
    return 0L;

  return new KBufferDrag( selectedData(), Parent );
}


void KHexEdit::cut()
{
  if( isReadOnly() || OverWrite )
    return;

  KBufferDrag *Drag = dragObject();
  if( !Drag )
    return;

  QApplication::clipboard()->setData( Drag, ClipboardMode );

  removeSelectedData();
}


void KHexEdit::copy()
{
  KBufferDrag *Drag = dragObject();
  if( !Drag )
    return;

  QApplication::clipboard()->setData( Drag, ClipboardMode );
}


void KHexEdit::paste()
{
  if( isReadOnly() )
    return;

  QMimeSource *Source = QApplication::clipboard()->data( ClipboardMode );
  pasteFromSource( Source );
}


void KHexEdit::pasteFromSource( QMimeSource *Source )
{
  if( !Source || !Source->provides(KBufferDrag::OctetStream) )
    return;

  QByteArray Data;
  if( !KBufferDrag::decode(Source,Data) )
    return;

  if( !Data.isEmpty() )
    insert( Data );
}


void KHexEdit::insert( const QByteArray &D )
{
  pauseCursorBlinking();

  if( OverWrite )
  {
    if( BufferRanges->hasSelection() )
    {
      // replacing the selection:
      // we restrict the replacement to the minimum length of selection and input
      KSection Selection = BufferRanges->selection();
      Selection.restrictEndTo( Selection.start()+D.size()-1 );
      int W = DataBuffer->replace( Selection, D.data(), Selection.width() );
      BufferCursor->gotoCIndex( Selection.start()+W );
      BufferRanges->addChangedRange( Selection );
      BufferRanges->removeSelection();
    }
    else
    {
      // replacing the normal data, at least until the end
      KSection Section( BufferCursor->trueIndex(), D.size(), false );
      Section.restrictEndTo( BufferLayout->length()-1 );
      if( Section.isValid() && !BufferCursor->isBehind() )
      {
        int W = DataBuffer->replace( Section, D.data(), Section.width() );
        BufferCursor->gotoNextByte( W );
        BufferRanges->addChangedRange( Section );
      }
      else
      {
        unpauseCursorBlinking();
        emit inputFailed();
        return;
      }
    }
  }
  else
  {
    if( BufferRanges->hasSelection() )
    {
      // replacing the selection
      KSection Selection = BufferRanges->selection();
      int OldLastIndex = BufferLayout->length() - 1;
      int W = DataBuffer->replace( Selection, D.data(), D.size() );
      updateLength();
      BufferCursor->gotoIndex( Selection.start() + W );
      if( W > 0 )
      {
        if( Selection.width() == (int)D.size() )
          BufferRanges->addChangedRange( Selection );
        else
        {
          int NewLastIndex = DataBuffer->size() - 1;
          BufferRanges->addChangedRange( Selection.start(), NewLastIndex>OldLastIndex?NewLastIndex:OldLastIndex );
        }
      }
      BufferRanges->removeSelection();
    }
    else
    {
      int InputAfterEnd = BufferCursor->isBehindEnd();
      int OldIndex = BufferCursor->trueIndex();
      int W = DataBuffer->insert( OldIndex, D.data(), D.size() );
      updateLength();
      std::cout << "End"<<InputAfterEnd<<" OldIndex"<<OldIndex<<"Pos"<<BufferCursor->pos()<<std::endl;
      if( W > 0 )
      {
        if( InputAfterEnd )
          BufferCursor->gotoEnd();
        else
          BufferCursor->gotoNextByte( W );
        BufferRanges->addChangedRange( KSection(OldIndex,DataBuffer->size()-1) );
      std::cout << "Behind" << BufferCursor->isBehind()<<"Index"<<BufferCursor->index()<<"Pos"<<BufferCursor->pos()<<std::endl;
      }
      else
      {
        unpauseCursorBlinking();
        emit inputFailed();
        return;
      }
    }
  }
  repaintChanged();

  unpauseCursorBlinking();

  emit bufferChanged();
  emit selectionChanged();
}


void KHexEdit::removeSelectedData()
{
  pauseCursorBlinking();

  KSection Selection = BufferRanges->selection();

  BufferRanges->removeFurtherSelections();

  if( !OverWrite )
    removeData( Selection );
  BufferRanges->removeSelection();

  repaintChanged();

  if( BufferCursor->isValid() )
  {
    BufferCursor->gotoCIndex( Selection.start() );

    ensureCursorVisible();
//     clearUndoRedo();
    viewport()->setCursor( isReadOnly() ? arrowCursor : ibeamCursor );
  }

  unpauseCursorBlinking();

  emit bufferChanged();
  emit selectionChanged();
}


void KHexEdit::removeData( KSection Indizes )
{
//   if( undoEnabled )
//   {
//     checkUndoRedoInfo( UndoRedoInfo::RemoveSelected );
//     if( !undoRedoInfo.valid() )
//     {
//       doc->selectionStart( selNum, undoRedoInfo.id, undoRedoInfo.index );
//       undoRedoInfo.d->text = QString::null;
//     }
//     readFormats( c1, c2, undoRedoInfo.d->text, TRUE );
//   }

  int OldLastIndex = BufferLayout->length()-1;
  // do it!
  DataBuffer->remove( Indizes );
  updateLength();
  BufferRanges->addChangedRange( Indizes.start(), OldLastIndex );
}


void KHexEdit::updateLength()
{
  BufferLayout->setLength( DataBuffer->size() );
  setNoOfLines( BufferLayout->noOfLines() );
}


bool KHexEdit::goInsideByte()
{
  int ValidIndex = BufferCursor->validIndex();
  if( ValidIndex == -1 || !OverWrite || isReadOnly() || BufferCursor->isBehind() )
  {
    return false;
  }
  if( ActiveColumn == &textColumn() )
  {
    pauseCursorBlinking();
    ActiveColumn = &hexColumn();
    InactiveColumn = &textColumn();
    unpauseCursorBlinking();
  }
  BufferCursor->setInsideByte( true );
  OldValue = EditValue = (unsigned char)DataBuffer->datum( ValidIndex );

  hexColumn().codingFunction()( ByteBuffer, EditValue );
  drawEditedByte( true );

  std::cout << "going inside byte" << std::endl;

  // put Cursor at the end (implemented later)
  return true;
}


bool KHexEdit::incByte()
{
  if( !BufferCursor->isInsideByte() && !goInsideByte() )
    return false;
  if( EditValue < 255 )
  {
    ++EditValue;
    hexColumn().codingFunction()( ByteBuffer, EditValue );
    drawEditedByte( true );
    return true;
  }
  else
    return false;
}

bool KHexEdit::decByte()
{
  if( !BufferCursor->isInsideByte() && !goInsideByte() )
    return false;
  if( EditValue > 0 )
  {
    --EditValue;
    hexColumn().codingFunction()( ByteBuffer, EditValue );
    drawEditedByte( true );
    return true;
  }
  else
    return false;
}


void KHexEdit::clipboardChanged()
{
  // don't listen to selection changes
  disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()), this, 0 );
  selectAll( false );
}


void KHexEdit::setCursorPosition( int Index )
{
  pauseCursorBlinking();
  BufferCursor->gotoCIndex( Index );

  BufferRanges->removeSelection();
  if( BufferRanges->isModified() )
  {
    viewport()->setCursor( isReadOnly() ? arrowCursor : ibeamCursor );

    if( !OverWrite ) emit cutAvailable( BufferRanges->hasSelection() );
    repaintChanged();
    emit copyAvailable( BufferRanges->hasSelection() );
    emit selectionChanged();
  }
  ensureCursorVisible();
  unpauseCursorBlinking();
}


void KHexEdit::placeCursor( const QPoint &Point )
{
  resetInputContext();

  // switch active column if needed
  if( textColumn().isVisible() && Point.x() >= textColumn().x() )
  {
    ActiveColumn = &textColumn();
    InactiveColumn = &hexColumn();
  }
  else
  {
    ActiveColumn = &hexColumn();
    InactiveColumn = &textColumn();
  }

  // get coord of click and whether this click was closer to the end of the pos
  KBufferCoord C( activeColumn().magPosOfX(Point.x()), lineAt(Point.y()) );

  BufferCursor->gotoCCoord( C );
}


int KHexEdit::indexByPoint( const QPoint &Point ) const
{
  const KBufferColumn *C;
  if( textColumn().isVisible() && Point.x() >= textColumn().x() )
    C = &textColumn();
  else
    C = &hexColumn();

  KBufferCoord Coord( C->posOfX(Point.x()), lineAt(Point.y()) );

  return BufferLayout->indexAtCCoord( Coord );
}


void KHexEdit::showEvent( QShowEvent *e )
{
    KColumnsView::showEvent( e );
    BufferLayout->setNoOfLinesPerPage( noOfLinesPerPage() );
}


bool KHexEdit::eventFilter( QObject *O, QEvent *E )
{
  if( O == this || O == viewport() )
  {
    if( E->type() == QEvent::FocusIn )
      startCursorBlinking();
    else if( E->type() == QEvent::FocusOut )
      stopCursorBlinking();
  }

//   if( O == this && E->type() == QEvent::PaletteChange )
//   {
//     QColor old( viewport()->colorGroup().color(QColorGroup::Text) );
//
//     if( old != colorGroup().color(QColorGroup::Text) )
//     {
//       QColor c( colorGroup().color(QColorGroup::Text) );
//       doc->setMinimumWidth( -1 );
//       doc->setDefaultFormat( doc->formatCollection()->defaultFormat()->font(), c );
//       lastFormatted = doc->firstParagraph();
//       formatMore();
//       repaintChanged();
//     }
//   }

  return QScrollView::eventFilter( O, E );
}


void KHexEdit::blinkCursor()
{
  // skip the cursor drawing?
  if( CursorHidden )
    return;

  // switch the cursor state
  drawCursor( !BlinkCursorVisible );
}


void KHexEdit::startCursorBlinking()
{
  CursorHidden = false;
  if( !BufferCursor->isValid() )
    BufferCursor->gotoIndex( 0 );

  createCursorPixmaps();
  drawCursor( true );
  drawFrameCursor( true );

  CursorBlinkTimer->start( QApplication::cursorFlashTime()/2 );
}


void KHexEdit::stopCursorBlinking()
{
  CursorBlinkTimer->stop();

  // must Cursor be removed?
  if( BlinkCursorVisible )
    drawCursor( false );
  drawFrameCursor( false );
  CursorHidden = true;
}


void KHexEdit::pauseCursorBlinking()
{
  if( BufferCursor->isInsideByte() )
  {
    BufferCursor->setInsideByte( false );
    drawEditedByte( false );
  }
  else
  {
    // must Cursor be removed?
    if( BlinkCursorVisible )
      drawCursor( false );
  }
  drawFrameCursor( false );

  CursorHidden = true;
}


void KHexEdit::unpauseCursorBlinking()
{
  CursorHidden = false;

  if( CursorBlinkTimer->isActive() && BufferCursor->isValid() )
  {
    createCursorPixmaps();
    drawCursor( true );
    drawFrameCursor( true );
//     std::cout << "Index:" << BufferCursor->index() << "(" << BufferCursor->trueIndex() << ")" <<std::endl;
  }
}


void KHexEdit::createCursorPixmaps()
{
  // create CursorPixmaps
  CursorPixmaps->setSize( activeColumn().byteWidth(), LineHeight );

  int Index = BufferCursor->validIndex();

  QPainter Paint;
  Paint.begin( &CursorPixmaps->offPixmap(), this );
  activeColumn().paintByte( &Paint, Index );
  Paint.end();

  Paint.begin( &CursorPixmaps->onPixmap(), this );
  activeColumn().paintCursor( &Paint, Index );
  Paint.end();

  // calculat the shape
  KPixelX CursorX;
  KPixelX CursorW;
  if( BufferCursor->isBehind() )
  {
    CursorX = QMAX( 0, CursorPixmaps->onPixmap().width()-3 );
    CursorW = 2;
  }
  else
  {
    CursorX = 0;
    CursorW = OverWrite ? -1 : 2;
  }
  CursorPixmaps->setShape( CursorX, CursorW );
}


void KHexEdit::pointPainterToCursor( QPainter &Painter, const KBufferColumn &Column ) const
{
  int x = Column.xOfPos( BufferCursor->pos() ) - contentsX();
  int y = LineHeight * BufferCursor->line() - contentsY();

  Painter.begin( viewport() );
  Painter.translate( x, y );
}


void KHexEdit::drawCursor( bool CursorOn )
{
  // any reason to skip the cursor drawing?
  if( !isUpdatesEnabled() || !viewport()->isUpdatesEnabled()
//       || (!style().styleHint( QStyle::SH_BlinkCursorWhenTextSelected ) && !selectedText().isEmpty())
      || (CursorOn && !hasFocus() && !viewport()->hasFocus() && !InDnD )
      || BufferCursor->isInsideByte()
      /*|| isReadOnly()*/ )
    return;

  QPainter Painter;
  pointPainterToCursor( Painter, activeColumn() );
  Painter.drawPixmap( CursorPixmaps->cursorX(), 0,
                      CursorOn?CursorPixmaps->onPixmap():CursorPixmaps->offPixmap(),
                      CursorPixmaps->cursorX(),0,CursorPixmaps->cursorW(),-1 );

  // store state
  BlinkCursorVisible = CursorOn;
}


void KHexEdit::drawFrameCursor( bool FrameOn )
{
  // any reason to skip the cursor drawing?
  if( !isUpdatesEnabled() || !viewport()->isUpdatesEnabled()
//       || (!style().styleHint( QStyle::SH_BlinkCursorWhenTextSelected ) && !selectedText().isEmpty())
      || (FrameOn && !hasFocus() && !viewport()->hasFocus() && !InDnD )
       /*|| isReadOnly()*/ )
    return;

  QPainter Painter;
  pointPainterToCursor( Painter, inactiveColumn() );
  int Index = BufferCursor->validIndex();
  if( FrameOn )
    inactiveColumn().paintFramedByte( &Painter, Index );
  else
    inactiveColumn().paintByte( &Painter, Index );
}


void KHexEdit::drawEditedByte( bool Edited )
{
  // any reason to skip the cursor drawing?
  if( !isUpdatesEnabled() || !viewport()->isUpdatesEnabled()
//       || (!style().styleHint( QStyle::SH_BlinkCursorWhenTextSelected ) && !selectedText().isEmpty())
//       || (FrameOn && !hasFocus() && !viewport()->hasFocus() && !InDnD )
       /*|| isReadOnly()*/ )
    return;

  int Index = BufferCursor->index();
  DataBuffer->replace( Index, 1, (char*)&EditValue, 1 );

  QPainter Painter;
  pointPainterToCursor( Painter, activeColumn() );
  if( Edited )
  {
    activeColumn().paintEditedByte( &Painter, EditValue, ByteBuffer );
    Painter.end();
    pointPainterToCursor( Painter, inactiveColumn() );
    inactiveColumn().paintFramedByte( &Painter, Index );
  }
  else
    activeColumn().paintByte( &Painter, Index );
}

void KHexEdit::drawContents( QPainter *P, int cx, int cy, int cw, int ch )
{
  KColumnsView::drawContents( P, cx, cy, cw, ch );
  // TODO: update non blinking cursors. Should this perhaps be done in the buffercolumn?
  // Then it needs to know about inactive, insideByte and the like... well...
  // perhaps subclassing the buffer columns even more, to KTextColumn and KHexColumn?
}


bool KHexEdit::handleByteEditKey( QKeyEvent *KeyEvent )
{
  std::cout << "Input to inside Byte: ";
  bool KeyUsed = false;
  //
  switch( KeyEvent->key() )
  {
    case Key_Plus:
      incByte();
      KeyUsed = true;
      break;
    case Key_Minus:
      decByte();
      KeyUsed = true;
      break;
    case Key_Escape:
      EditValue = OldValue;
      drawEditedByte( false );
    case Key_Backspace:
      if( EditValue > 0 )
      {
        hexColumn().removingFunction()( &EditValue );
        hexColumn().codingFunction()( ByteBuffer, EditValue );
        drawEditedByte( true );
      }
      else
        emit inputFailed();
      KeyUsed = true;
      break;
    default:
      if( KeyEvent->text().length() > 0
          && ( !(KeyEvent->state()&( ControlButton | AltButton | MetaButton )) )
          && ( !KeyEvent->ascii() || KeyEvent->ascii() >= 32 ) )
      {
        QByteArray D( 1 );
        D[0] = KeyEvent->ascii();
//         std::cout << "Trying to add:" << D[0] << " to " << (unsigned int)EditValue <<"";
        if( hexColumn().addingFunction()(&EditValue,D[0]) )
        {
          hexColumn().codingFunction()( ByteBuffer, EditValue );
          drawEditedByte( true );
        }
        else
        {
//           std::cout << " failed";
          emit inputFailed();
        }
        KeyUsed = true;
      }
  }
  std::cout << std::endl;

  return KeyUsed;
}


void KHexEdit::keyPressEvent( QKeyEvent *KeyEvent )
{
  if( BufferCursor->isInsideByte() && handleByteEditKey(KeyEvent) )
    return;
//  changeIntervalTimer->stop();
//  interval = 10;
  bool KeyUnknown = false;

  // handle selection
  BufferRanges->removeFurtherSelections();
  if( BufferRanges->isModified() )
    repaintChanged();

  bool clearUndoRedoInfo = true;
  bool ShiftPressed =  KeyEvent->state() & ShiftButton;
  bool ControlPressed = KeyEvent->state() & ControlButton;
  bool AltPressed = KeyEvent->state() & AltButton;

  // we only care for cursor keys and the like, won't hardcode any other keys
  // we also don't check whether the commands are allowed
  // as the commands are also available as API so the check has to be done
  // in each command anyway
  switch( KeyEvent->key() )
  {
    case Key_Left:
      moveCursor( ControlPressed ? MoveWordBackward : MoveBackward, ShiftPressed );
      break;
    case Key_Right:
      moveCursor( ControlPressed ? MoveWordForward : MoveForward, ShiftPressed );
      break;
    case Key_Up:
      moveCursor( ControlPressed ? MovePgUp : MoveUp, ShiftPressed );
      break;
    case Key_Down:
      moveCursor( ControlPressed ? MovePgDown : MoveDown, ShiftPressed );
      break;
    case Key_Home:
      moveCursor( ControlPressed ? MoveHome : MoveLineStart, ShiftPressed );
      break;
    case Key_End:
      moveCursor( ControlPressed ? MoveEnd : MoveLineEnd, ShiftPressed );
      break;
    case Key_Prior:
      moveCursor( MovePgUp, ShiftPressed );
      break;
    case Key_Next:
      moveCursor( MovePgDown, ShiftPressed );
      break;
    case Key_Return:
    case Key_Enter:
      goInsideByte();
//       if( isReadOnly() )
//         viewport()->setCursor( arrowCursor );

//       doKeyboardAction( ActionReturn );
//       emit returnPressed();

//       KeyEvent->ignore();
      break;
    case Key_Tab:
      if( ActiveColumn == &textColumn() )
      {
        // in last column we care about tab changes focus
        if( TabChangesFocus && !ShiftPressed )
        {
          // leave tab for parent widget
          KeyEvent->ignore();
          break;
        }
        if( hexColumn().isVisible() )
        {
          pauseCursorBlinking();
          ActiveColumn = &hexColumn();
          InactiveColumn = &textColumn();
          unpauseCursorBlinking();
        }
        break;
      }

      if( textColumn().isVisible() )
      {
        // in last column we care about tab changes focus
        if( TabChangesFocus && ShiftPressed )
        {
          // leave tab for parent widget
          KeyEvent->ignore();
          break;
        }
        pauseCursorBlinking();
        ActiveColumn = &textColumn();
        InactiveColumn = &hexColumn();
        unpauseCursorBlinking();
      }
      break;

    case Key_Delete:
      if( ShiftPressed )
        cut();
      else if( BufferRanges->hasSelection() )
        removeSelectedData();
      else
      {
        doKeyboardAction( ControlPressed ? ActionWordDelete : ActionDelete );
        clearUndoRedoInfo = false;
      }
      break;

    case Key_Insert:
      if( ShiftPressed )
        paste();
      else if( ControlPressed )
        copy();
      else
        setOverwriteMode( !OverWrite );
      break;

    case Key_Backspace:
      if( AltPressed )
      {
        if( ControlPressed )
          break;
        else if( ShiftPressed )
        {
//           redo();
          break;
        }
        else
        {
//           undo();
          break;
        }
      }
      else if( BufferRanges->hasSelection() )
      {
        removeSelectedData();
        break;
      }

      doKeyboardAction( ControlPressed ? ActionWordBackspace : ActionBackspace );
      clearUndoRedoInfo = false;
      break;
    case Key_F16: // "Copy" key on Sun keyboards
      copy();
      break;
    case Key_F18: // "Paste" key on Sun keyboards
      paste();
      break;
    case Key_F20: // "Cut" key on Sun keyboards
      cut();
      break;

    default:
      if( KeyEvent->text().length() > 0
          && ( !(KeyEvent->state()&( ControlButton | AltButton | MetaButton )) )
          && ( !KeyEvent->ascii() || KeyEvent->ascii() >= 32 ) )
      {
        QByteArray D( 1 );
        D[0] = KeyEvent->ascii();
        if( ActiveColumn == &textColumn() )
        {
//         clearUndoRedoInfo = false;
          insert( D );
        }
        else
        {
          int ValidIndex = BufferCursor->validIndex();
          if( ValidIndex == -1 )
          {
            emit inputFailed();
            break;
          }
          // check for plus/minus
          if( OverWrite && !isReadOnly() )
          {
            bool KeyUsed = false;
            switch( KeyEvent->key() )
            {
              case Key_Plus:
                incByte();
                KeyUsed = true;
                break;
              case Key_Minus:
                decByte();
                KeyUsed = true;
                break;
            }
            if( KeyUsed )
              break;
          }
          OldValue = (unsigned char)DataBuffer->datum( ValidIndex );
          EditValue = 0;
          bool KeyValid = hexColumn().addingFunction()( &EditValue, D[0] );
          if( KeyValid )
          {
            hexColumn().codingFunction()( ByteBuffer, EditValue );
            BufferCursor->setInsideByte( true );

            if( !OverWrite )
            {
              // turn key into initial byte
              // if !succeded
              //   emit inputFailed();
              //   break;
              // insert inital byteSpacingWidth
              // turn to byteedit mode
            }
            drawEditedByte( true );
          }
          else
            emit inputFailed();
        }
        break;
      }
      else
        KeyUnknown = true;

        //QScrollView::keyPressEvent( KeyEvent ); useful where????
  }

  emit cursorPositionChanged( BufferCursor->index() );

//   if( clearUndoRedoInfo )
//     clearUndoRedo();
//   changeIntervalTimer->start( 100, true );

  if( KeyUnknown )
    KeyEvent->ignore();
}


void KHexEdit::moveCursor( KMoveAction Action, bool Select )
{
  pauseCursorBlinking();

  if( Select )
  {
    if( !BufferRanges->selectionStarted() )
      BufferRanges->setSelectionStart( BufferCursor->trueIndex() );

    moveCursor( Action );
    BufferRanges->setSelectionEnd( BufferCursor->trueIndex() );

    if( !OverWrite ) emit cutAvailable( BufferRanges->hasSelection() );
    emit copyAvailable( BufferRanges->hasSelection() );
    emit selectionChanged();
  }
  else
  {
    moveCursor( Action );
    BufferRanges->removeSelection();

    if( BufferRanges->isModified() )
    {
      viewport()->setCursor( isReadOnly() ? arrowCursor : ibeamCursor );

      if( !OverWrite ) emit cutAvailable( BufferRanges->hasSelection() );
      emit copyAvailable( BufferRanges->hasSelection() );
      emit selectionChanged();
    }
  }

  if( BufferRanges->isModified() ) // TODO: would it be possible to move this behind ensureCursorVisible?
    repaintChanged();
  ensureCursorVisible();

  unpauseCursorBlinking();
}


// TODO: table of member functions, indexes by enum CursorAction (CharType would be stored at cursor)
void KHexEdit::moveCursor( KMoveAction Action )
{
  resetInputContext();
  switch( Action )
  {
    case MoveBackward:     BufferCursor->gotoPreviousByte(); break;
    case MoveWordBackward: {
                             int NewIndex = BufferCursor->trueIndex();
                             NewIndex = DataBuffer->indexOfPreviousWordStart( NewIndex, KDataBuffer::Readable );
                             BufferCursor->gotoIndex( NewIndex );
                           }
                           break;
    case MoveForward:      BufferCursor->gotoNextByte();     break;
    case MoveWordForward:  {
                             int NewIndex = BufferCursor->trueIndex();
                             NewIndex = DataBuffer->indexOfNextWordStart( NewIndex, KDataBuffer::Readable );
                             BufferCursor->gotoIndex( NewIndex );
                           }
                           break;
    case MoveUp:           BufferCursor->gotoUp();             break;
    case MovePgUp:         BufferCursor->gotoPageUp();         break;
    case MoveDown:         BufferCursor->gotoDown();           break;
    case MovePgDown:       BufferCursor->gotoPageDown();       break;
    case MoveLineStart:    BufferCursor->gotoLineStart();      break;
    case MoveHome:         BufferCursor->gotoStart();          break;
    case MoveLineEnd:      BufferCursor->gotoLineEnd();        break;
    case MoveEnd:          BufferCursor->gotoEnd();            break;
  }
}


void KHexEdit::doKeyboardAction( KKeyboardAction Action )
{
  if( isReadOnly() )//|| OverWrite )
    return;

  pauseCursorBlinking();

  switch( Action )
  {
    case ActionDelete:
//       if( BufferCursor->isBehind() )
//       {
//         if ( undoEnabled )
//         {
//           checkUndoRedoInfo( UndoRedoInfo::Delete );
//           if ( !undoRedoInfo.valid() )
//           {
//             undoRedoInfo.id = cursor->paragraph()->paragId();
//             undoRedoInfo.index = cursor->index();
//             undoRedoInfo.d->text = QString::null;
//           }
//           undoRedoInfo.d->text.insert( undoRedoInfo.d->text.length(), cursor->paragraph()->at( cursor->index() ), true );
//         }
//         cursor->remove();
//       }
//       else
//       {
//         clearUndoRedo();
//         doc->setSelectionStart( QTextDocument::Temp, *cursor );
//         if( Action == ActionWordDelete && !cursor->atParagEnd() )
//           Cursor->gotoNextWord();
//         else
//           Cursor->gotoNextLetter();
//
//         doc->setSelectionEnd( QTextDocument::Temp, *cursor );
//         removeSelectedData( QTextDocument::Temp );
//       }
      if( !OverWrite )
      {
        int Index = BufferCursor->trueIndex();
        if( Index < BufferLayout->length() )
        {
          removeData( KSection(Index,1,false) );
          if( Index == BufferLayout->length() )
            BufferCursor->gotoEnd();
        }
      }
      break;

    case ActionWordDelete: // kills data until the start of the next word
      if( !OverWrite )
      {
        int Index = BufferCursor->trueIndex();
        if( Index < BufferLayout->length() )
        {
          int End = DataBuffer->indexOfBeforeNextWordStart( Index );
          removeData( KSection(Index,End) );
          if( Index == BufferLayout->length() )
            BufferCursor->gotoEnd();
        }
      }
      break;

    case ActionBackspace:
      if( OverWrite )
        BufferCursor->gotoPreviousByte();
      else
      {
        int DeleteIndex = BufferCursor->trueIndex() - 1;
        if( DeleteIndex >= 0 )
        {
          removeData( KSection(DeleteIndex,1,false) );
          if( DeleteIndex == BufferLayout->length() )
            BufferCursor->gotoEnd();
          else
            BufferCursor->gotoPreviousByte();
        }
      }
      break;

//       if( Action == ActionBackspace && !cursor->atParagStart() )
//       {
//         if( undoEnabled )
//         {
//           checkUndoRedoInfo( UndoRedoInfo::Delete );
//           if( !undoRedoInfo.valid() )
//           {
//             undoRedoInfo.id = cursor->paragraph()->paragId();
//             undoRedoInfo.index = cursor->index();
//             undoRedoInfo.d->text = QString::null;
//           }
//         }
//         cursor->gotoPreviousLetter();
//         if ( undoEnabled )
//         {
//           undoRedoInfo.d->text.insert( 0, cursor->paragraph()->at( cursor->index() ), true );
//           undoRedoInfo.index = cursor->index();
//         }
//         cursor->remove();
//         lastFormatted = cursor->paragraph();
//       }
//       else if( cursor->paragraph()->prev()
//                || (Action == ActionWordBackspace && !cursor->atParagStart()) )
//       {
//         clearUndoRedo();
//         doc->setSelectionStart( QTextDocument::Temp, *cursor );
//
//         if ( Action == ActionWordBackspace && !cursor->atParagStart() )
//           cursor->gotoPreviousWord();
//         else
//           cursor->gotoPreviousLetter();
//
//         doc->setSelectionEnd( QTextDocument::Temp, *cursor );
//         removeSelectedData( QTextDocument::Temp );
//       }
    case ActionWordBackspace:
      {
        int LeftIndex = BufferCursor->trueIndex() - 1;
        if( LeftIndex >= 0 )
        {
          int WordStart = DataBuffer->indexOfPreviousWordStart( LeftIndex );
          if( !OverWrite )
            removeData( KSection(WordStart,LeftIndex) );
          if( WordStart == BufferLayout->length() )
            BufferCursor->gotoEnd();
          else
            BufferCursor->gotoIndex(WordStart);
        }
      }
  }

  repaintChanged();
  ensureCursorVisible();
  unpauseCursorBlinking();

  emit bufferChanged();
}


void KHexEdit::repaintChanged()
{
  if( !isUpdatesEnabled() || !viewport()->isUpdatesEnabled() || !BufferRanges->isModified() )
    return;

  resizeContents( totalWidth(), totalHeight() );

  KPixelX cx = contentsX();
  KPixelY cy = contentsY();
  KPixelX cw = visibleWidth();
  KPixelY ch = visibleHeight();

  QPainter Paint;
  Paint.begin( viewport() );
  Paint.translate( -cx, -cy );

  // calculate affected lines/indizes
  KSection VisibleLines( lineAt(cy), lineAt(cy+ch-1) );
  KSection FullPositions( 0, BufferLayout->noOfBytesPerLine() );
  KCoordRange VisibleRange( FullPositions, VisibleLines );
//   std::cout << "repaintChanged->"<<FirstIndex<<":"<<FirstLine<<","<<LastIndex<<":"<<LastLine<<std::endl;

  // collect affected buffer columns
  QPtrList<KBufferColumn> RepaintColumns;
  for( int i=0; i<NoOfBufferColumns; ++i )
    if( BufferColumn[i]->isVisible() && BufferColumn[i]->overlaps(cx,cx+cw-1) )
    {
      RepaintColumns.append( BufferColumn[i] );
      BufferColumn[i]->preparePainting( cx, cw );
    }

  if( RepaintColumns.count() > 0 )
  {
    KCoordRange ChangedRange;
    // as there might be multiple selections on this line redo until no more is changed
    while( hasChanged(VisibleRange,&ChangedRange) )
    {
//       std::cout << "  changed->"<<FirstChangedIndex<<","<<LastChangedIndex<<std::endl;

      // only one line?
      if( ChangedRange.start().line() == ChangedRange.end().line() )
        for( KBufferColumn *C=RepaintColumns.first(); C; C=RepaintColumns.next() )
          paintLine( &Paint, C, ChangedRange.start().line(),
                     KSection(ChangedRange.start().pos(),ChangedRange.end().pos()) );
      //
      else
      {
        // first line
        for( KBufferColumn *C=RepaintColumns.first(); C; C=RepaintColumns.next() )
          paintLine( &Paint, C, ChangedRange.start().line(),
                     KSection(ChangedRange.start().pos(),FullPositions.end()) );

        // at least one full line?
        for( int l = ChangedRange.start().line()+1; l < ChangedRange.end().line(); ++l )
          for( KBufferColumn *C=RepaintColumns.first(); C; C=RepaintColumns.next() )
            paintLine( &Paint, C, l, FullPositions );

        // last line
        for( KBufferColumn *C=RepaintColumns.first(); C; C=RepaintColumns.next() )
          paintLine( &Paint, C, ChangedRange.end().line(),
                     KSection(FullPositions.start(),ChangedRange.end().pos()) );
      }

      // continue the search at the overnext index
      VisibleRange.setStart( ChangedRange.end()+2 );
      if( !VisibleRange.isValid() )
        break;
    }
  }

  Paint.end();

  // Paint possible removed bytes at the end of the last line
  // Paint new/removed trailing lines
//   drawContents( P, cx, cy, cw, ch );
  // Paint empty rects
//  paintEmptyArea( P, cx, cy, cw, ch );
//   BufferLayout->noOfLines()

  BufferRanges->resetChangedRanges();
}


void KHexEdit::paintLine( QPainter *P, KBufferColumn *C, int Line, KSection Positions ) const
{
  Positions.restrictTo( C->visiblePositions() );

  // nothing to paint?
  if( !Positions.isValid() )
    return;
//   std::cout << "  paintLine->"<<Line<< ":"<<FirstPos<<","<<LastPos<<std::endl;

  // calculating pixel values
  KSection XPixels = C->wideXPixelsOfPos( Positions );

  KPixelY cy = Line * LineHeight;

  // to avoid flickers we first paint to the linebuffer
  QPainter Paint;
  Paint.begin( &LineBuffer, this );

  Paint.translate( C->x(), 0 );
  C->paintPositions( &Paint, Line, Positions );
  Paint.translate( -C->x(), 0 );

  if( HorizontalGrid && XPixels.start() < TotalWidth )
    Paint.drawLine( XPixels.start(), LineHeight-1, XPixels.width(), LineHeight-1 );  // TODO: use a additional TotalHeight?

  Paint.end();
  // copy to screen
  P->drawPixmap( XPixels.start(), cy, LineBuffer, XPixels.start(), 0, XPixels.width(), LineHeight ); // bitBlt directly impossible: lack of real coord
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
//   // Not visible or the user is draging the window, so don't position to caret yet
//   if ( !isVisible() || isHorizontalSliderPressed() || isVerticalSliderPressed() )
//   {
//     d->ensureCursorVisibleInShowEvent = true;
//     return;
//   }

  KPixelX x = activeColumn().x() + activeColumn().xOfPos( BufferCursor->pos() );
  KPixelY y = LineHeight * BufferCursor->line() + LineHeight/2;
  int xMargin = 1; // TODO: reasoning of the values 1 and 2 (below)
  int yMargin = LineHeight/2 + 2;
  ensureVisible( x, y, xMargin, yMargin );
}



void KHexEdit::contentsMousePressEvent( QMouseEvent *e )
{
//   clearUndoRedo();
  pauseCursorBlinking();

  // care about a left button press?
  if( e->button() == LeftButton )
  {
    MousePressed = true;

    // select whole line?
    if( TrippleClickTimer->isActive()
        && (e->globalPos()-DoubleClickPoint).manhattanLength() < QApplication::startDragDistance() )
    {
      BufferRanges->setSelectionStart( BufferLayout->indexAtLineStart(DoubleClickLine) );
      BufferCursor->gotoLineEnd();
      BufferRanges->setSelectionEnd( BufferCursor->trueIndex() );
      repaintChanged();
      unpauseCursorBlinking();
      return;
    }

    MousePoint = e->pos();
    placeCursor( MousePoint );
    ensureCursorVisible();

    // start of a drag perhaps?
    if( BufferRanges->selectionIncludes(indexByPoint( MousePoint )) )
    {
      DragStartPossible = true;
      unpauseCursorBlinking();
      DragStartTimer->start( QApplication::startDragTime(), true );
      DragStartPoint = MousePoint;
      return;
    }

    if( BufferRanges->selectionStarted() )
    {
      if( e->state() & ShiftButton )
        BufferRanges->setSelectionEnd( BufferCursor->trueIndex() );
      else
      {
        BufferRanges->removeSelection();
        BufferRanges->setSelectionStart( BufferCursor->trueIndex() );
      }
    }
    else
    {
      if( isReadOnly() || !(e->state()&ShiftButton) )
        BufferRanges->setSelectionStart( BufferCursor->index() );
      else
      {
        BufferRanges->setSelectionStart( BufferCursor->index() );
        BufferRanges->setSelectionEnd( BufferCursor->index() );
      }
    }

    BufferRanges->removeFurtherSelections();

    if( BufferRanges->isModified() )
    {
      repaintChanged();
      viewport()->setCursor( isReadOnly() ? arrowCursor : ibeamCursor );
    }
  }
  else if( e->button() == MidButton )
    BufferRanges->removeSelection();

  if( BufferRanges->isModified() )
  {
    repaintChanged();
    viewport()->setCursor( isReadOnly() ? arrowCursor : ibeamCursor );
  }
  unpauseCursorBlinking();
}


void KHexEdit::contentsMouseMoveEvent( QMouseEvent *e )
{
  if( MousePressed )
  {
    if( DragStartPossible )
    {
      DragStartTimer->stop();
      // moved enough for a drag?
      if( (e->pos()-DragStartPoint).manhattanLength() > QApplication::startDragDistance() )
        startDrag();
      if( !isReadOnly() )
        viewport()->setCursor( ibeamCursor );
      return;
    }
    // selecting
    MousePoint = e->pos();
    handleMouseMove( MousePoint );
    OldMousePoint = MousePoint;
  }
  else if( !isReadOnly() )
  {
    // visual feedback for possible dragging
    bool InSelection = BufferRanges->hasSelection() && BufferRanges->selectionIncludes( indexByPoint(e->pos()) );
    viewport()->setCursor( InSelection?arrowCursor:ibeamCursor );
  }

//  placeCursor( e->pos() );
}


void KHexEdit::contentsMouseReleaseEvent( QMouseEvent *e )
{
  // this is not the release of a doubleclick so we need to provess it?
  if( !InDoubleClick )
  {
    int Line = lineAt( e->pos().y() );
    int Pos = activeColumn().posOfX( e->pos().x() ); // TODO: can we be sure here about the active column?
    int Index = BufferLayout->indexAtCCoord( KBufferCoord(Pos,Line) ); // TODO: can this be another index than the one of the cursor???
    emit clicked( Index );
  }

  int OldIndex = BufferCursor->index();

  if( ScrollTimer->isActive() )
    ScrollTimer->stop();

  if( DragStartTimer->isActive() )
    DragStartTimer->stop();

  // still dreaming about a drag start?
  if( DragStartPossible )
  {
    selectAll( false );
    MousePressed = false;
    unpauseCursorBlinking();
  }

  if( MousePressed )
  {
    MousePressed = false;
    if( QApplication::clipboard()->supportsSelection() )
    {
      ClipboardMode = QClipboard::Selection;
      disconnect( QApplication::clipboard(), SIGNAL(selectionChanged()), this, 0);

      copy();

      connect( QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(clipboardChanged()) );
      ClipboardMode = QClipboard::Clipboard;
    }
  }
  // middle mouse button paste?
  else if( e->button() == MidButton && !isReadOnly() )
  {
    pauseCursorBlinking();

    placeCursor( e->pos() );
    ensureCursorVisible();
    if( BufferRanges->hasSelection() )
      BufferRanges->removeSelection();

    BufferRanges->setSelectionStart( OldIndex );
    BufferRanges->setSelectionStart( BufferCursor->index() );

    BufferRanges->removeFurtherSelections();

    if( BufferRanges->isModified() )
    {
      repaintChanged();
      viewport()->setCursor( ibeamCursor );
    }
    ClipboardMode = QClipboard::Selection;
    paste();
    ClipboardMode = QClipboard::Clipboard;

    unpauseCursorBlinking();
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
void KHexEdit::contentsMouseDoubleClickEvent( QMouseEvent *e )
{
  // we are only interested in LMB doubleclicks
  if( e->button() != Qt::LeftButton )
  {
    e->ignore();
    return;
  }

  DoubleClickLine = BufferCursor->line();

  int Index = BufferCursor->index();
  // for doubleclick we try to select the word that includes this index
  if( DataBuffer->isWordChar(Index) )
  {
    KSection Word = DataBuffer->wordSection( Index );
    if( Word.isValid() )
    {
      pauseCursorBlinking();
      BufferRanges->setSelection( Word );
      BufferCursor->gotoIndex( Word.end()+1 );
      repaintChanged();
      unpauseCursorBlinking();
    }
  }

  // as we already have a doubleclick maybe it is a tripple click
  TrippleClickTimer->start( qApp->doubleClickInterval(), true );
  DoubleClickPoint = e->globalPos();

  InDoubleClick = true; //
  MousePressed = true;

  emit doubleClicked( indexByPoint(e->pos()) );
}


void KHexEdit::autoScrollTimerDone()
{
  if( MousePressed )
    handleMouseMove( viewportToContents(viewport()->mapFromGlobal( QCursor::pos() )) );
}


void KHexEdit::handleMouseMove( const QPoint& Point ) // handles the move of the mouse with pressed buttons
{
  // no scrolltimer and outside of viewport?
  if( !ScrollTimer->isActive() && Point.y() < contentsY() || Point.y() > contentsY() + visibleHeight() )

    ScrollTimer->start( DefaultScrollTimerPeriod, false );
  // scrolltimer but inside of viewport?
  else if( ScrollTimer->isActive() && Point.y() >= contentsY() && Point.y() <= contentsY() + visibleHeight() )
    ScrollTimer->stop();

  pauseCursorBlinking();

  int OldIndex = BufferCursor->index();
  int OldLine = BufferCursor->line();

  placeCursor( Point );

  if( InDoubleClick )
  {
    // find out which is the closest: the last index, the one of the next word or the one of the previous one
    int NewIndex = BufferCursor->index();
    int IndexWordStart = DataBuffer->indexOfWordStart( NewIndex );
    int IndexWordEnd = DataBuffer->indexOfWordEnd( NewIndex );
    int XDistanceToOrigin = abs( activeColumn().xOfPos(OldIndex) - MousePoint.x() );
    int XDistanceToPrevWord = abs( activeColumn().xOfPos(IndexWordStart) - MousePoint.x() );
    int XDistanceToNextWord = abs( activeColumn().xOfPos(IndexWordEnd) - MousePoint.x() );

    // is not in the same line?
    if( BufferCursor->line() != OldLine )
      // make sure XDistanceToOrigin won't win
      XDistanceToOrigin = INT_MAX; //std::numeric_limits<int>::max();

    if( XDistanceToOrigin < XDistanceToPrevWord && XDistanceToOrigin < XDistanceToNextWord )
      BufferCursor->gotoIndex( OldIndex );
    else if( XDistanceToPrevWord < XDistanceToNextWord )
      BufferCursor->gotoIndex( IndexWordStart );
    else
      BufferCursor->gotoIndex( IndexWordEnd );
  }

  if( BufferRanges->selectionStarted() )
    BufferRanges->setSelectionEnd( BufferCursor->trueIndex() );

  if( BufferRanges->isModified() )
    repaintChanged();

  ensureCursorVisible();
  unpauseCursorBlinking();
}


void KHexEdit::startDrag()
{
  MousePressed = false;
  InDoubleClick = false;
  DragStartPossible = false;

  QDragObject *Drag = dragObject( viewport() );
  if( !Drag )
    return;

  if( isReadOnly() || OverWrite )
    Drag->dragCopy();
  else if( Drag->drag() && QDragObject::target() != this && QDragObject::target() != viewport() )
    removeSelectedData();
}


void KHexEdit::contentsDragEnterEvent( QDragEnterEvent *e )
{
  if( isReadOnly() || !KBufferDrag::canDecode(e) )
  {
    e->ignore();
    return;
  }
  e->acceptAction();
  InDnD = true;
}


void KHexEdit::contentsDragMoveEvent( QDragMoveEvent *e )
{
  // is this content for us?
  if( isReadOnly() || !KBufferDrag::canDecode(e) )
  {
    e->ignore();
    return;
  }
  // let text cursor follow mouse
  pauseCursorBlinking();
  placeCursor( e->pos() );
  unpauseCursorBlinking();
  e->acceptAction();
}


void KHexEdit::contentsDragLeaveEvent( QDragLeaveEvent * )
{
  InDnD = false;
}



void KHexEdit::contentsDropEvent( QDropEvent *e )
{
  if( isReadOnly() )
    return;

  InDnD = false;
  e->acceptAction();
  if( !KBufferDrag::canDecode(e) )
    return;

  bool IsInternalDrag = e->source() == this || e->source() == viewport();

  int InsertIndex = BufferCursor->index();
  if( IsInternalDrag && BufferRanges->hasSelection() )
  {
    KSection Selection = BufferRanges->selection();
    if( Selection.includes(InsertIndex) )
      InsertIndex = Selection.start();
    else if( Selection.endsBefore(InsertIndex) && e->action() == QDropEvent::Move )
      InsertIndex = InsertIndex - Selection.width() + 1;
  }

  if( IsInternalDrag && e->action() == QDropEvent::Move )
    removeSelectedData();
  else
    viewport()->setCursor( isReadOnly() ? arrowCursor : ibeamCursor );

  BufferRanges->removeSelection();
  pauseCursorBlinking();

  pasteFromSource( e );

  BufferCursor->gotoIndex( InsertIndex );
  unpauseCursorBlinking();

  // emit appropriate signals.
  emit selectionChanged();
  emit cursorPositionChanged( BufferCursor->index() );
}


void KHexEdit::contentsWheelEvent( QWheelEvent *e )
{
  if( isReadOnly() )
  {
    if( e->state() & ControlButton )
    {
      if( e->delta() > 0 )
        zoomOut();
      else if( e->delta() < 0 )
        zoomIn();
      return;
    }
  }
  QScrollView::contentsWheelEvent( e );
}


#if 0
void KHexEdit::contentsContextMenuEvent( QContextMenuEvent *e )
{
//   clearUndoRedo();
  MousePressed = false;

  e->accept();

  QPopupMenu *PopupMenu = createPopupMenu( e->pos() );
  if( !PopupMenu )
    PopupMenu = createPopupMenu();
  if( !PopupMenu )
    return;
  int r = PopupMenu->exec( e->globalPos() );
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

// editing
#if 0
void KBufferCursor::dumpEditState()
{
  Buffer->replace( ByteEdit->dump(), 1, Pos );
}
#endif

#include "khexedit.moc"
