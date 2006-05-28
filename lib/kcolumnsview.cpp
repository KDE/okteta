/***************************************************************************
                          kcolumnsview.cpp  -  description
                             -------------------
    begin                : Mit Mai 21 2003
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


// qt specific
#include <QListIterator>
#include <QPaintEvent>
#include <QPainter>
#include <QStyle>
#include <QScrollBar>
// lib specific
#include "kcolumn.h"
#include "kcolumnsview.h"


using namespace KHE;

static const int DefaultSingleStep = 20;

KColumnsView::KColumnsView( /*bool R,*/ QWidget *Parent )
 : QAbstractScrollArea( Parent ),
   NoOfLines( 0 ),
   LineHeight( 0 ),
   ColumnsWidth( 0 ),
//    Reversed( R ),
   d( 0 )
{
  viewport()->setAttribute( Qt::WA_StaticContents );
  viewport()->setBackgroundRole ( QPalette::Base );
  horizontalScrollBar()->setSingleStep( DefaultSingleStep );
  verticalScrollBar()->setSingleStep( DefaultSingleStep );

  viewport()->setFocusProxy( this );
  viewport()->setFocusPolicy( Qt::WheelFocus );
}

KColumnsView::~KColumnsView()
{
  while( !Columns.isEmpty() )
    delete Columns.takeFirst();
}


KPixelX KColumnsView::xOffset() const { return horizontalScrollBar()->value(); }
KPixelY KColumnsView::yOffset() const { return verticalScrollBar()->value(); }


void KColumnsView::setColumnsPos( KPixelX x, KPixelY y )
{
  horizontalScrollBar()->setValue( x );
  verticalScrollBar()->setValue( y );
}


void KColumnsView::setNoOfLines( int NewNoOfLines )
{
  if( NoOfLines == NewNoOfLines )
    return;

  NoOfLines = NewNoOfLines;

  updateScrollBars();
}


void KColumnsView::setLineHeight( KPixelY LH )
{
  if( LH == LineHeight )
    return;

  LineHeight = LH;

  QListIterator<KColumn*> it( Columns );
  while( it.hasNext() )
    it.next()->setLineHeight( LineHeight );

  verticalScrollBar()->setSingleStep( LineHeight );
  updateScrollBars();
}


void KColumnsView::updateWidths()
{
  ColumnsWidth = 0;
  QListIterator<KColumn*> it( Columns );
  while( it.hasNext() )
  {
    KColumn *Column = it.next();
    Column->setX( ColumnsWidth );
    ColumnsWidth += Column->visibleWidth();
  }

  updateScrollBars();
}


void KColumnsView::updateScrollBars()
{
  const int ScrollBarWidth = style()->pixelMetric( QStyle::PM_ScrollBarExtent );
  QSize ViewSize = maximumViewportSize();

  bool NeedsVerticalBar = columnsHeight() > ViewSize.height();
  bool NeedsHorizontalBar = columnsWidth() > ViewSize.width();

  if( NeedsVerticalBar )
    ViewSize.rwidth() -= ScrollBarWidth;
  if( NeedsHorizontalBar )
    ViewSize.rheight() -= ScrollBarWidth;

  verticalScrollBar()->setRange( 0, columnsHeight()-ViewSize.height() );
  verticalScrollBar()->setPageStep( ViewSize.height() );
  horizontalScrollBar()->setRange( 0, columnsWidth()-ViewSize.width() );
  horizontalScrollBar()->setPageStep( ViewSize.width() );
}


int KColumnsView::noOfLinesPerPage() const
{
  if( LineHeight == 0 )
    return 1;

  int NoOfLinesPerPage = (visibleHeight()-1) / LineHeight; // -1 ensures to get always the last visible line

  if( NoOfLinesPerPage == 0 )
    // ensure to move down at least one line
    NoOfLinesPerPage = 1;
  return NoOfLinesPerPage;
}


void KColumnsView::addColumn( KColumn *C )
{
//   if( Reversed )
//     Columns.prepend( C );
//   else
    Columns.append( C );

  updateWidths();
}


void KColumnsView::removeColumn( KColumn *C )
{
  int Pos = Columns.indexOf( C );
  if( Pos != -1 )
    Columns.removeAt( Pos );

  delete C;

  updateWidths();
}


void KColumnsView::scrollContentsBy( int dx, int dy )
{
  viewport()->scroll( dx, dy );
}

bool KColumnsView::event( QEvent *Event )
{
  if( Event->type() == QEvent::StyleChange || Event->type() == QEvent::LayoutRequest )
    updateScrollBars();

  return QAbstractScrollArea::event( Event );
}


void KColumnsView::resizeEvent( QResizeEvent */*ResizeEvent*/ )
{
  updateScrollBars();
}

void KColumnsView::paintEvent( QPaintEvent *Event )
{
  const int x = xOffset();
  const int y = yOffset();

  QRect DirtyRect = Event->rect();
  DirtyRect.translate( x, y );

  QPainter Painter( viewport() );
  Painter.translate( -x, -y );

  drawColumns( &Painter, DirtyRect.x(),DirtyRect.y(), DirtyRect.width(), DirtyRect.height() );
}

//#include <kdebug.h>
void KColumnsView::drawColumns( QPainter *Painter, int cx, int cy, int cw, int ch )
{
  KPixelXs DirtyXs = KPixelXs::fromWidth( cx, cw );

  // content to be shown?
  if( DirtyXs.startsBefore(ColumnsWidth) )
  {
    KPixelYs DirtyYs = KPixelYs::fromWidth( cy, ch );

    // collect affected columns
    QList<KColumn*> DirtyColumns;
    QListIterator<KColumn*> it( Columns );
    while( it.hasNext() )
    {
      KColumn *Column = it.next();
      if( Column->isVisible() && Column->overlaps(DirtyXs) )
        DirtyColumns.append( Column );
    }

    // any lines of any columns to be drawn?
    if( NoOfLines > 0 )
    {
      // calculate affected lines
      KSection DirtyLines = visibleLines( DirtyYs );
      DirtyLines.restrictEndTo( NoOfLines - 1 );

      if( DirtyLines.isValid() )
      {
        KPixelY cy = DirtyLines.start() * LineHeight;
//kDebug()<<"Dirty lines: "<<DirtyLines.start()<<"-"<<DirtyLines.end()<<endl;
        // starting painting with the first line
        int Line = DirtyLines.start();
        QListIterator<KColumn*> it( DirtyColumns );
        KColumn *Column = it.next();
        Painter->translate( Column->x(), cy );

        while( true )
        {
          Column->paintFirstLine( Painter, DirtyXs, Line );
          if( !it.hasNext() )
            break;
          Painter->translate( Column->width(), 0 );
          Column = it.next();
        }
        Painter->translate( -Column->x(), 0 );

        // Go through the other lines
        while( true )
        {
          ++Line;

          if( Line > DirtyLines.end() )
            break;

          QListIterator<KColumn*> it( DirtyColumns );
          Column = it.next();
          Painter->translate( Column->x(), LineHeight );

          while( true )
          {
            Column->paintNextLine( Painter );
            if( !it.hasNext() )
              break;
            Painter->translate( Column->width(), 0 );
            Column = it.next();
          }
          Painter->translate( -Column->x(), 0 );
        }
        cy = DirtyLines.end() * LineHeight;

        Painter->translate( 0, -cy );
      }
    }

    // draw empty columns?
    DirtyYs.setStart( columnsHeight() );
    if( DirtyYs.isValid() )
    {
      QListIterator<KColumn*> it( DirtyColumns );
      while( it.hasNext() )
        it.next()->paintEmptyColumn( Painter, DirtyXs, DirtyYs );
    }
  }

  // Painter empty rects
  DirtyXs.setStart( ColumnsWidth );
  if( DirtyXs.isValid() )
    drawEmptyArea( Painter, DirtyXs.start(), cy, DirtyXs.width(), ch );
}


void KColumnsView::drawEmptyArea( QPainter *Painter, int cx ,int cy, int cw, int ch)
{
    Painter->fillRect( cx,cy, cw,ch, viewport()->palette().brush(viewport()->backgroundRole()) );
}

#include "kcolumnsview.moc"
