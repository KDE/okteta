/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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
        emit printedPage( pageIndex );
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

#include "framestopaperprinter.moc"
