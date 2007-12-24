/***************************************************************************
                          abstractframerenderer.h  -  description
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


#ifndef ABSTRACTFRAMERENDERER_H
#define ABSTRACTFRAMERENDERER_H

class QSize;
class QPoint;
class QRect;
class QPainter;

// frame only horizontal duplicated or also vertical?
class AbstractFrameRenderer
{
  public:
    AbstractFrameRenderer();
    virtual ~AbstractFrameRenderer();

  public: // API to be implemented
    // make this flags?
//     virtual bool hasFixedWidth() const = 0;
//     virtual bool hasFixedHeight() const = 0;
    virtual int height() const = 0;
    virtual int width() const = 0;
//     virtual QSize sizeHint( const QSize &maxSize ) const = 0;
    // only vertical for now...
//     virtual int framesCount() const = 0;

    /** painting will start, fix all things like Time and Data */
    virtual void prepare() = 0;
    virtual void renderFrame( QPainter *painter, int frameIndex ) = 0;

  public:
    void setPos( int x, int y );
    void setPos( const QPoint &point );

  public:
    QPoint pos () const;
    QRect rect () const;
    QSize size () const;
    int x() const;
    int y() const;

  protected:
    class Private;
    Private * const d;
};

#endif
