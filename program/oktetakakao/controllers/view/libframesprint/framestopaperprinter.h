/***************************************************************************
                          framestopaperprinter.h  -  description
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


#ifndef FRAMESTOPAGESPRINTER_H
#define FRAMESTOPAGESPRINTER_H


// Qt
#include <QtCore/QList>
#include <QtCore/QRect>

class QPrinter;
class AbstractFrameRenderer;

class FramesToPaperPrinter
{
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

  protected:
    QList<AbstractFrameRenderer*> mFrameRendererList;
    QRect mPaperRect;
    QRect mPageRect;
};

#endif
