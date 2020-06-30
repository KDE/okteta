/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef FRAMESPRINT_ABSTRACTFRAMERENDERER_HPP
#define FRAMESPRINT_ABSTRACTFRAMERENDERER_HPP

#include <QScopedPointer>

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
    virtual void renderFrame(QPainter* painter, int frameIndex) = 0;

public:
    void setPos(int x, int y);
    void setPos(QPoint point);

public:
    QPoint pos () const;
    QRect rect () const;
    QSize size () const;
    int x() const;
    int y() const;

private:
    QScopedPointer<class AbstractFrameRendererPrivate> const d;
};

#endif
