/***************************************************************************
                          kcolumnsview.cpp  -  description
                             -------------------
    begin                : Mit Mai 21 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// qt specific
#include <QtCore/QListIterator>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QScrollBar>
// lib specific
#include "kcolumn.h"
#include "kcolumnsview.h"


namespace KHEUI {

static const int DefaultSingleStep = 20;

class KColumnsViewPrivate
{
  public:
    KColumnsViewPrivate( /*bool R,*/ );
    ~KColumnsViewPrivate();
  public:
    void updateWidths();
  public: // calculated
    /** collection of all the columns. All columns will be autodeleted. */
    QList<KColumn*> Columns;
    /** the number of lines which the column view has */
    int NoOfLines;
    /** the height of each line in pixels */
    KPixelY LineHeight;
    /** the width of all visible columns together */
    KPixelX ColumnsWidth;

  public:
//    bool Reversed;
};


KColumnsViewPrivate::KColumnsViewPrivate( /*bool R,*/)
 : NoOfLines( 0 ),
   LineHeight( 0 ),
   ColumnsWidth( 0 )
//    Reversed( R )
{}

void KColumnsViewPrivate::updateWidths()
{
  ColumnsWidth = 0;
  QListIterator<KColumn*> it( Columns );
  while( it.hasNext() )
  {
    KColumn *Column = it.next();
    Column->setX( ColumnsWidth );
    ColumnsWidth += Column->visibleWidth();
  }
}

KColumnsViewPrivate::~KColumnsViewPrivate()
{
    while( !Columns.isEmpty() )
        delete Columns.takeFirst();
}

KColumnsView::KColumnsView( /*bool R,*/ QWidget *Parent )
 : QAbstractScrollArea( Parent ),
   d( new KColumnsViewPrivate() )
{
  viewport()->setAttribute( Qt::WA_StaticContents );
  viewport()->setBackgroundRole ( QPalette::Base );
  horizontalScrollBar()->setSingleStep( DefaultSingleStep );
  verticalScrollBar()->setSingleStep( DefaultSingleStep );

  viewport()->setFocusProxy( this );
  viewport()->setFocusPolicy( Qt::WheelFocus );
}


int KColumnsView::noOfLines()          const { return d->NoOfLines; }
KPixelY KColumnsView::lineHeight()     const { return d->LineHeight; }
uint KColumnsView::lineAt( KPixelY y ) const { return (d->LineHeight!=0) ? y / d->LineHeight : 0; }
KHE::KSection KColumnsView::visibleLines() const
{
  KPixelYs YSpan = KPixelYs::fromWidth( yOffset(), visibleHeight() );
  return KHE::KSection( lineAt(YSpan.start()), lineAt(YSpan.end()) );
}
KHE::KSection KColumnsView::visibleLines( const KPixelYs &YPixels ) const
{ return KHE::KSection( lineAt(YPixels.start()), lineAt(YPixels.end()) ); }

KPixelX KColumnsView::visibleWidth()  const { return viewport()->width(); }
KPixelY KColumnsView::visibleHeight() const { return viewport()->height(); }

KPixelY KColumnsView::columnsHeight() const { return d->NoOfLines*d->LineHeight; }
KPixelX KColumnsView::columnsWidth()  const { return d->ColumnsWidth; }

QPoint KColumnsView::viewportToColumns( const QPoint &P ) const
{ return QPoint(xOffset(),yOffset()) + P; }


KPixelX KColumnsView::xOffset() const { return horizontalScrollBar()->value(); }
KPixelY KColumnsView::yOffset() const { return verticalScrollBar()->value(); }


void KColumnsView::setColumnsPos( KPixelX x, KPixelY y )
{
  horizontalScrollBar()->setValue( x );
  verticalScrollBar()->setValue( y );
}


void KColumnsView::setNoOfLines( int NewNoOfLines )
{
  if( d->NoOfLines == NewNoOfLines )
    return;

  d->NoOfLines = NewNoOfLines;

  updateScrollBars();
}


void KColumnsView::setLineHeight( KPixelY LH )
{
  if( LH == d->LineHeight )
    return;

  d->LineHeight = LH;

  QListIterator<KColumn*> it( d->Columns );
  while( it.hasNext() )
    it.next()->setLineHeight( d->LineHeight );

  verticalScrollBar()->setSingleStep( d->LineHeight );
  updateScrollBars();
}


void KColumnsView::updateWidths()
{
  d->updateWidths();

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
    if( d->LineHeight < 1 )
        return 1;

    int NoOfLinesPerPage = (visibleHeight()-1) / d->LineHeight; // -1 ensures to get always the last visible line

    if( NoOfLinesPerPage < 1 )
        // ensure to move down at least one line
        NoOfLinesPerPage = 1;
    return NoOfLinesPerPage;
}


void KColumnsView::addColumn( KColumn *C )
{
//   if( Reversed )
//     Columns.prepend( C );
//   else
    d->Columns.append( C );

    updateWidths();
}


void KColumnsView::removeColumn( KColumn *C )
{
  int Pos = d->Columns.indexOf( C );
  if( Pos != -1 )
    d->Columns.removeAt( Pos );

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
  if( DirtyXs.startsBefore(d->ColumnsWidth) )
  {
    KPixelYs DirtyYs = KPixelYs::fromWidth( cy, ch );

    // collect affected columns
    QList<KColumn*> DirtyColumns;
    QListIterator<KColumn*> it( d->Columns );
    while( it.hasNext() )
    {
      KColumn *Column = it.next();
      if( Column->isVisible() && Column->overlaps(DirtyXs) )
        DirtyColumns.append( Column );
    }

    // any lines of any columns to be drawn?
    if( d->NoOfLines > 0 )
    {
      // calculate affected lines
      KHE::KSection DirtyLines = visibleLines( DirtyYs );
      DirtyLines.restrictEndTo( d->NoOfLines - 1 );

      if( DirtyLines.isValid() )
      {
        KPixelY cy = DirtyLines.start() * d->LineHeight;
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
          Painter->translate( Column->x(), d->LineHeight );

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
        cy = DirtyLines.end() * d->LineHeight;

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
  DirtyXs.setStart( d->ColumnsWidth );
  if( DirtyXs.isValid() )
    drawEmptyArea( Painter, DirtyXs.start(), cy, DirtyXs.width(), ch );
}


void KColumnsView::drawEmptyArea( QPainter *Painter, int cx ,int cy, int cw, int ch)
{
    Painter->fillRect( cx,cy, cw,ch, viewport()->palette().brush(viewport()->backgroundRole()) );
}


KColumnsView::~KColumnsView()
{
    delete d;
}

}

#include "kcolumnsview.moc"
