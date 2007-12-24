/***************************************************************************
                          framestopaperprinter.cpp  -  description
                             -------------------
    begin                : Tue Dec 11 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/

#include "framestopaperprinter.h"


// lib
#include "abstractframerenderer.h"
// Qt
#include <QtGui/QPainter>
#include <QtGui/QPrinter>


FramesToPaperPrinter::FramesToPaperPrinter() {}

QRect FramesToPaperPrinter::pageRect() const { return mPageRect; }

void FramesToPaperPrinter::setPaperRect( const QRect &paperRect ) { mPaperRect = paperRect; }
void FramesToPaperPrinter::setPageRect( const QRect &pageRect ) { mPageRect = pageRect; }

void FramesToPaperPrinter::addFrameRenderer( AbstractFrameRenderer *frameRenderer )
{
    mFrameRendererList.append( frameRenderer );
}

bool FramesToPaperPrinter::print( QPrinter *printer, int firstPageIndex, int lastPageIndex )
{
    bool success = true;

    foreach( AbstractFrameRenderer *frameRenderer, mFrameRendererList )
    {
        frameRenderer->prepare();
    }

    QPainter painter( printer );
    int pageIndex = firstPageIndex;
    while( true )
    {
        foreach( AbstractFrameRenderer *frameRenderer, mFrameRendererList )
        {
            const int x = frameRenderer->x();
            const int y = frameRenderer->y();
            painter.translate( x, y );
            frameRenderer->renderFrame( &painter, pageIndex );
            painter.translate( -x, -y );
        }
        if( pageIndex < lastPageIndex )
        {
            printer->newPage();
            ++pageIndex;
        }
        else
            break;
    }

    return success;
}


FramesToPaperPrinter::~FramesToPaperPrinter()
{
    qDeleteAll( mFrameRendererList );
}
