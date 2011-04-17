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

#ifndef FRAMESTOPAPERPRINTER_H
#define FRAMESTOPAPERPRINTER_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QRect>

class QPrinter;
class AbstractFrameRenderer;

class FramesToPaperPrinter : public QObject
{
  Q_OBJECT

  public:
    FramesToPaperPrinter();
    ~FramesToPaperPrinter();

  public:
    void setPaperRect( const QRect &paperRect );
    void setPageRect( const QRect &pageRect );

    void addFrameRenderer( AbstractFrameRenderer *frameRenderer );

    bool print( QPrinter *printer, int firstPageIndex, int lastPageIndex );

  public:
    QRect pageRect() const;

  Q_SIGNALS:
    void printedPage( int pageIndex );

  protected:
    QList<AbstractFrameRenderer*> mFrameRendererList;
    QRect mPaperRect;
    QRect mPageRect;
};

#endif
