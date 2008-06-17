/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractcolumnframerenderer.h"

// lib
#include "abstractcolumnrenderer.h"
// Qt
#include <QtCore/QListIterator>
#include <QtGui/QPainter>


static const int DefaultSingleStep = 20;

class AbstractColumnFrameRendererPrivate
{
  public:
    AbstractColumnFrameRendererPrivate();
    ~AbstractColumnFrameRendererPrivate();
  public:
    void updateWidths();
  public: // calculated
    /** collection of all the columns. All columns will be autodeleted. */
    QList<KHEPrint::AbstractColumnRenderer*> mColumns;
    /** the number of lines which the columnRenderer view has */
    int mNoOfLines;
    /** the height of each line in pixels */
    KPixelY mLineHeight;
    /** the width of all visible columns together */
    KPixelX mColumnsWidth;
};


AbstractColumnFrameRendererPrivate::AbstractColumnFrameRendererPrivate()
 : mNoOfLines( 0 ),
   mLineHeight( 1 ),
   mColumnsWidth( 0 )
{}

void AbstractColumnFrameRendererPrivate::updateWidths()
{
    mColumnsWidth = 0;
    QListIterator<KHEPrint::AbstractColumnRenderer*> it( mColumns );
    while( it.hasNext() )
    {
        KHEPrint::AbstractColumnRenderer *columnRenderer = it.next();
        columnRenderer->setX( mColumnsWidth );
        mColumnsWidth += columnRenderer->visibleWidth();
    }
}

AbstractColumnFrameRendererPrivate::~AbstractColumnFrameRendererPrivate()
{
   qDeleteAll( mColumns );
}

AbstractColumnFrameRenderer::AbstractColumnFrameRenderer()
 : d( new AbstractColumnFrameRendererPrivate() )
{
}


int AbstractColumnFrameRenderer::noOfLines()          const { return d->mNoOfLines; }
KPixelY AbstractColumnFrameRenderer::lineHeight()     const { return d->mLineHeight; }

KPixelY AbstractColumnFrameRenderer::columnsHeight() const { return d->mNoOfLines*d->mLineHeight; }
KPixelX AbstractColumnFrameRenderer::columnsWidth()  const { return d->mColumnsWidth; }

void AbstractColumnFrameRenderer::setNoOfLines( int noOfLines )
{
    if( d->mNoOfLines == noOfLines )
        return;

    d->mNoOfLines = noOfLines;
}


void AbstractColumnFrameRenderer::setLineHeight( KPixelY newLineHeight )
{
    if( newLineHeight == d->mLineHeight )
        return;

    if( newLineHeight < 1 )
        newLineHeight = 1;
    d->mLineHeight = newLineHeight;

    QListIterator<KHEPrint::AbstractColumnRenderer*> it( d->mColumns );
    while( it.hasNext() )
        it.next()->setLineHeight( d->mLineHeight );
}


void AbstractColumnFrameRenderer::updateWidths()
{
    d->updateWidths();
}


int AbstractColumnFrameRenderer::noOfLinesPerFrame() const
{
    // TODO: the right reaction?
    if( d->mLineHeight < 1 )
        return 1;

    int noOfLinesPerFrame = height() / d->mLineHeight;

    if( noOfLinesPerFrame < 1 )
        // ensure at least one line, so there aren't endless frames TODO: think about
        noOfLinesPerFrame = 1;
    return noOfLinesPerFrame;
}


void AbstractColumnFrameRenderer::addColumn( KHEPrint::AbstractColumnRenderer *columnRenderer )
{
    d->mColumns.append( columnRenderer );

    updateWidths();
}


void AbstractColumnFrameRenderer::removeColumn( KHEPrint::AbstractColumnRenderer *columnRenderer )
{
    const int columnPos = d->mColumns.indexOf( columnRenderer );
    if( columnPos != -1 )
        d->mColumns.removeAt( columnPos );

    delete columnRenderer;

    updateWidths();
}


void AbstractColumnFrameRenderer::renderFrame( QPainter *painter, int frameIndex )
{
    KPixelXs renderedXs = KPixelXs::fromWidth( 0, width() );

    // content to be shown?
    if( renderedXs.startsBefore(d->mColumnsWidth) )
    {
        // collect affected columns
        QList<KHEPrint::AbstractColumnRenderer*> columnRenderers;
        QListIterator<KHEPrint::AbstractColumnRenderer*> it( d->mColumns );
        while( it.hasNext() )
        {
            KHEPrint::AbstractColumnRenderer *columnRenderer = it.next();
            if( columnRenderer->isVisible() && columnRenderer->overlaps(renderedXs) )
                columnRenderers.append( columnRenderer );
        }

        // calculate affected lines
        const int baseLine = frameIndex * noOfLinesPerFrame();
        KHE::KSection renderedLines = KHE::KSection::fromWidth( baseLine, noOfLinesPerFrame() );
        renderedLines.restrictEndTo( noOfLines()-1 );

        KPixelYs renderedYs = KPixelYs::fromWidth( 0, renderedLines.width()*d->mLineHeight );

        // any lines of any columns to be drawn?
        if( renderedLines.isValid() )
        {
            // paint full columns
            QListIterator<KHEPrint::AbstractColumnRenderer*> fit( columnRenderers ); // TODO: reuse later, see some lines below
            while( fit.hasNext() )
                fit.next()->renderColumn( painter, renderedXs, renderedYs );

            KPixelY cy = 0;
            // starting painting with the first line
            int line = renderedLines.start();
            QListIterator<KHEPrint::AbstractColumnRenderer*> it( columnRenderers );
            KHEPrint::AbstractColumnRenderer *columnRenderer = it.next();
            painter->translate( columnRenderer->x(), cy );

            while( true )
            {
                columnRenderer->renderFirstLine( painter, renderedXs, line );
                if( !it.hasNext() )
                    break;
                painter->translate( columnRenderer->width(), 0 );
                columnRenderer = it.next();
            }
            painter->translate( -columnRenderer->x(), 0 );

            // Go through the other lines
            while( true )
            {
                ++line;

                if( line > renderedLines.end() )
                    break;

                QListIterator<KHEPrint::AbstractColumnRenderer*> it( columnRenderers );
                columnRenderer = it.next();
                painter->translate( columnRenderer->x(), d->mLineHeight );

                while( true )
                {
                    columnRenderer->renderNextLine( painter );
                    if( !it.hasNext() )
                        break;
                    painter->translate( columnRenderer->width(), 0 );
                    columnRenderer = it.next();
                }
                painter->translate( -columnRenderer->x(), 0 );
            }
            cy = (renderedLines.width()-1)*d->mLineHeight;

            painter->translate( 0, -cy );
        }

        // draw empty columns?
       renderedYs.set( renderedYs.nextBehindEnd(), height()-1 );
        if( renderedYs.isValid() )
        {
            QListIterator<KHEPrint::AbstractColumnRenderer*> it( columnRenderers );
            while( it.hasNext() )
                it.next()->renderEmptyColumn( painter, renderedXs, renderedYs );
        }
    }

    // painter empty rects
    renderedXs.setStart( d->mColumnsWidth );
    if( renderedXs.isValid() )
        drawEmptyArea( painter, renderedXs.start(), 0, renderedXs.width(), height() );
}


void AbstractColumnFrameRenderer::drawEmptyArea( QPainter *painter, int cx ,int cy, int cw, int ch )
{
    painter->fillRect( cx,cy, cw,ch, Qt::white );
}


AbstractColumnFrameRenderer::~AbstractColumnFrameRenderer()
{
    delete d;
}
