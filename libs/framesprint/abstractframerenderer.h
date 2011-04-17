/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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
