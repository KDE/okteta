/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef FRAMESPRINT_ABSTRACTFRAMERENDERER_HPP
#define FRAMESPRINT_ABSTRACTFRAMERENDERER_HPP

// Std
#include <memory>

class QSize;
class QPoint;
class QRect;
class QPainter;

namespace FramesPrint {

// frame only horizontal duplicated or also vertical?
class AbstractFrameRenderer
{
public:
    AbstractFrameRenderer();
    AbstractFrameRenderer(const AbstractFrameRenderer&) = delete;
    AbstractFrameRenderer(AbstractFrameRenderer&&) = delete;

    virtual ~AbstractFrameRenderer();

    AbstractFrameRenderer& operator=(const AbstractFrameRenderer&) = delete;
    AbstractFrameRenderer& operator=(AbstractFrameRenderer&&) = delete;

public: // API to be implemented
    // make this flags?
//     virtual bool hasFixedWidth() const = 0;
//     virtual bool hasFixedHeight() const = 0;
    [[nodiscard]]
    virtual int height() const = 0;
    [[nodiscard]]
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
    [[nodiscard]]
    QPoint pos () const;
    [[nodiscard]]
    QRect rect () const;
    [[nodiscard]]
    QSize size () const;
    [[nodiscard]]
    int x() const;
    [[nodiscard]]
    int y() const;

private:
    const std::unique_ptr<class AbstractFrameRendererPrivate> d;
};

}

#endif
