/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2007,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "columnsview.h"

// lib
#include "columnrenderer.h"
// Qt
#include <QtCore/QListIterator>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QScrollBar>


namespace KHEUI {

static const int DefaultSingleStep = 20;

class ColumnsViewPrivate
{
  public:
    ColumnsViewPrivate( /*bool R,*/ );
    ~ColumnsViewPrivate();
  public:
    void updateWidths();
  public: // calculated
    /** collection of all the columns. All columns will be autodeleted. */
    QList<ColumnRenderer*> Columns;
    /** the number of lines which the column view has */
    int NoOfLines;
    /** the height of each line in pixels */
    KPixelY LineHeight;
    /** the width of all visible columns together */
    KPixelX ColumnsWidth;

  public:
//    bool Reversed;
};


ColumnsViewPrivate::ColumnsViewPrivate( /*bool R,*/)
 : NoOfLines( 0 ),
   LineHeight( 0 ),
   ColumnsWidth( 0 )
//    Reversed( R )
{}

void ColumnsViewPrivate::updateWidths()
{
    ColumnsWidth = 0;
    QListIterator<ColumnRenderer*> it( Columns );
    while( it.hasNext() )
    {
        ColumnRenderer *column = it.next();
        column->setX( ColumnsWidth );
        ColumnsWidth += column->visibleWidth();
    }
}

ColumnsViewPrivate::~ColumnsViewPrivate()
{
    while( !Columns.isEmpty() )
        delete Columns.takeFirst();
}

ColumnsView::ColumnsView( /*bool R,*/ QWidget *parent )
 : QAbstractScrollArea( parent ),
   d( new ColumnsViewPrivate() )
{
    viewport()->setAttribute( Qt::WA_StaticContents );
    viewport()->setBackgroundRole ( QPalette::Base );
    horizontalScrollBar()->setSingleStep( DefaultSingleStep );
    verticalScrollBar()->setSingleStep( DefaultSingleStep );

    viewport()->setFocusProxy( this );
    viewport()->setFocusPolicy( Qt::WheelFocus );
}


int ColumnsView::noOfLines()          const { return d->NoOfLines; }
KPixelY ColumnsView::lineHeight()     const { return d->LineHeight; }
uint ColumnsView::lineAt( KPixelY y ) const { return (d->LineHeight!=0) ? y / d->LineHeight : 0; }
KHE::KSection ColumnsView::visibleLines() const
{
    const KPixelYs ySpan = KPixelYs::fromWidth( yOffset(), visibleHeight() );
    return KHE::KSection( lineAt(ySpan.start()), lineAt(ySpan.end()) );
}
KHE::KSection ColumnsView::visibleLines( const KPixelYs &YPixels ) const
{ return KHE::KSection( lineAt(YPixels.start()), lineAt(YPixels.end()) ); }

KPixelX ColumnsView::visibleWidth()  const { return viewport()->width(); }
KPixelY ColumnsView::visibleHeight() const { return viewport()->height(); }

KPixelY ColumnsView::columnsHeight() const { return d->NoOfLines*d->LineHeight; }
KPixelX ColumnsView::columnsWidth()  const { return d->ColumnsWidth; }

QPoint ColumnsView::viewportToColumns( const QPoint &P ) const
{ return QPoint(xOffset(),yOffset()) + P; }


KPixelX ColumnsView::xOffset() const { return horizontalScrollBar()->value(); }
KPixelY ColumnsView::yOffset() const { return verticalScrollBar()->value(); }


void ColumnsView::setColumnsPos( KPixelX x, KPixelY y )
{
    horizontalScrollBar()->setValue( x );
    verticalScrollBar()->setValue( y );
}


void ColumnsView::setNoOfLines( int newNoOfLines )
{
    if( d->NoOfLines == newNoOfLines )
        return;

    d->NoOfLines = newNoOfLines;

    updateScrollBars();
}


void ColumnsView::setLineHeight( KPixelY newLineHeight )
{
    if( newLineHeight == d->LineHeight )
        return;

    d->LineHeight = newLineHeight;

    QListIterator<ColumnRenderer*> it( d->Columns );
    while( it.hasNext() )
        it.next()->setLineHeight( d->LineHeight );

    verticalScrollBar()->setSingleStep( d->LineHeight );
    updateScrollBars();
}


void ColumnsView::updateWidths()
{
    d->updateWidths();

    updateScrollBars();
}


void ColumnsView::updateScrollBars()
{
    QSize viewSize = maximumViewportSize();

    const bool needsVerticalBar = ( columnsHeight() > viewSize.height() );
    const bool needsHorizontalBar = ( columnsWidth() > viewSize.width() );
    const int scrollBarWidth = style()->pixelMetric( QStyle::PM_ScrollBarExtent );

    if( needsVerticalBar )
        viewSize.rwidth() -= scrollBarWidth;
    if( needsHorizontalBar )
        viewSize.rheight() -= scrollBarWidth;

    verticalScrollBar()->setRange( 0, columnsHeight()-viewSize.height() );
    verticalScrollBar()->setPageStep( viewSize.height() );
    horizontalScrollBar()->setRange( 0, columnsWidth()-viewSize.width() );
    horizontalScrollBar()->setPageStep( viewSize.width() );
}


void ColumnsView::updateColumn( ColumnRenderer& columnRenderer )
{
    if( columnRenderer.isVisible() )
        viewport()->update( columnRenderer.x()-xOffset(), 0, columnRenderer.width(), visibleHeight() );
}


int ColumnsView::noOfLinesPerPage() const
{
    if( d->LineHeight < 1 )
        return 1;

    int result = (visibleHeight()-1) / d->LineHeight; // -1 ensures to get always the last visible line

    if( result < 1 )
        // ensure to move down at least one line
        result = 1;
    return result;
}


void ColumnsView::addColumn( ColumnRenderer *columnRenderer )
{
//   if( Reversed )
//     Columns.prepend( C );
//   else
    d->Columns.append( columnRenderer );

    updateWidths();
}


void ColumnsView::removeColumn( ColumnRenderer *columnRenderer )
{
    const int columnRendererIndex = d->Columns.indexOf( columnRenderer );
    if( columnRendererIndex != -1 )
        d->Columns.removeAt( columnRendererIndex );

    delete columnRenderer;

    updateWidths();
}


void ColumnsView::scrollContentsBy( int dx, int dy )
{
    viewport()->scroll( dx, dy );
}

bool ColumnsView::event( QEvent *event )
{
    if( event->type() == QEvent::StyleChange || event->type() == QEvent::LayoutRequest )
        updateScrollBars();

    return QAbstractScrollArea::event( event );
}


void ColumnsView::resizeEvent( QResizeEvent *event )
{
    updateScrollBars();

    QAbstractScrollArea::resizeEvent( event );
}

void ColumnsView::paintEvent( QPaintEvent *paintEvent )
{
    QAbstractScrollArea::paintEvent( paintEvent );

    const int x = xOffset();
    const int y = yOffset();

    QRect dirtyRect = paintEvent->rect();
    dirtyRect.translate( x, y );

    QPainter painter( viewport() );
    painter.translate( -x, -y );

    renderColumns( &painter, dirtyRect.x(),dirtyRect.y(), dirtyRect.width(), dirtyRect.height() );
}

//#include <kdebug.h>
void ColumnsView::renderColumns( QPainter *painter, int cx, int cy, int cw, int ch )
{
    KPixelXs dirtyXs = KPixelXs::fromWidth( cx, cw );

    // content to be shown?
    if( dirtyXs.startsBefore(d->ColumnsWidth) )
    {
        KPixelYs dirtyYs = KPixelYs::fromWidth( cy, ch );

        // collect affected columns
        QList<ColumnRenderer*> dirtyColumns;
        QListIterator<ColumnRenderer*> it( d->Columns );
        while( it.hasNext() )
        {
            ColumnRenderer *column = it.next();
            if( column->isVisible() && column->overlaps(dirtyXs) )
                dirtyColumns.append( column );
        }

        // any lines of any columns to be drawn?
        if( d->NoOfLines > 0 )
        {
            // calculate affected lines
            KHE::KSection dirtyLines = visibleLines( dirtyYs );
            dirtyLines.restrictEndTo( d->NoOfLines - 1 );

            if( dirtyLines.isValid() )
            {
                // paint full columns
                QListIterator<ColumnRenderer*> fit( d->Columns ); // TODO: reuse later, see some lines below
                while( fit.hasNext() )
                    fit.next()->renderColumn( painter, dirtyXs, dirtyYs );

                KPixelY cy = dirtyLines.start() * d->LineHeight;
        //kDebug()<<"Dirty lines: "<<dirtyLines.start()<<"-"<<dirtyLines.end();
                // starting painting with the first line
                int line = dirtyLines.start();
                QListIterator<ColumnRenderer*> it( dirtyColumns );
                ColumnRenderer *column = it.next();
                painter->translate( column->x(), cy );

                while( true )
                {
                    column->renderFirstLine( painter, dirtyXs, line );
                    if( !it.hasNext() )
                        break;
                    painter->translate( column->width(), 0 );
                    column = it.next();
                }
                painter->translate( -column->x(), 0 );

                // Go through the other lines
                while( true )
                {
                    ++line;

                    if( line > dirtyLines.end() )
                        break;

                    QListIterator<ColumnRenderer*> it( dirtyColumns );
                    column = it.next();
                    painter->translate( column->x(), d->LineHeight );

                    while( true )
                    {
                        column->renderNextLine( painter );
                        if( !it.hasNext() )
                            break;
                        painter->translate( column->width(), 0 );
                        column = it.next();
                    }
                    painter->translate( -column->x(), 0 );
                }
                cy = dirtyLines.end() * d->LineHeight;

                painter->translate( 0, -cy );
            }
        }

        // draw empty columns?
        dirtyYs.setStart( columnsHeight() );
        if( dirtyYs.isValid() )
        {
            QListIterator<ColumnRenderer*> it( dirtyColumns );
            while( it.hasNext() )
                it.next()->renderEmptyColumn( painter, dirtyXs, dirtyYs );
        }
    }

    // painter empty rects
    dirtyXs.setStart( d->ColumnsWidth );
    if( dirtyXs.isValid() )
        renderEmptyArea( painter, dirtyXs.start(), cy, dirtyXs.width(), ch );
}


void ColumnsView::renderEmptyArea( QPainter *painter, int cx ,int cy, int cw, int ch)
{
    painter->fillRect( cx,cy, cw,ch, viewport()->palette().brush(viewport()->backgroundRole()) );
}


ColumnsView::~ColumnsView()
{
    delete d;
}

}

#include "columnsview.moc"
