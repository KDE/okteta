/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractcolumnrenderer.hpp"
#include "abstractcolumnrenderer_p.hpp"

// lib
#include "columnsview.hpp"
// Qt
#include <QPainter>

namespace Okteta {

AbstractColumnRenderer::AbstractColumnRenderer(AbstractColumnRendererPrivate* d)
    : d_ptr(d)
{
}

AbstractColumnRenderer::AbstractColumnRenderer(AbstractColumnStylist* stylist)
    : d_ptr(new AbstractColumnRendererPrivate(stylist))
{
}

AbstractColumnRenderer::~AbstractColumnRenderer() = default;

AbstractColumnStylist* AbstractColumnRenderer::stylist() const
{
    Q_D(const AbstractColumnRenderer);

    return d->mStylist;
}

PixelX AbstractColumnRenderer::x() const
{
    Q_D(const AbstractColumnRenderer);

    return d->mXSpan.start();
}

PixelX AbstractColumnRenderer::rightX() const
{
    Q_D(const AbstractColumnRenderer);

    return d->mXSpan.end();
}

PixelX AbstractColumnRenderer::width() const
{
    Q_D(const AbstractColumnRenderer);

    return d->mXSpan.width();
}

bool AbstractColumnRenderer::isVisible() const
{
    Q_D(const AbstractColumnRenderer);

    return d->mIsVisible;
}

PixelX AbstractColumnRenderer::visibleWidth() const
{
    Q_D(const AbstractColumnRenderer);

    return d->mIsVisible ? d->mXSpan.width() : 0;
}

PixelY AbstractColumnRenderer::lineHeight() const
{
    Q_D(const AbstractColumnRenderer);

    return d->mLineHeight;
}

void AbstractColumnRenderer::setX(PixelX x)
{
    Q_D(AbstractColumnRenderer);

    d->mXSpan.moveToStart(x);
}

void AbstractColumnRenderer::setWidth(PixelX width)
{
    Q_D(AbstractColumnRenderer);

    d->mXSpan.setEndByWidth(width);
}

void AbstractColumnRenderer::setVisible(bool isVisible)
{
    Q_D(AbstractColumnRenderer);

    d->mIsVisible = isVisible;
}

void AbstractColumnRenderer::setLineHeight(PixelY lineHeight)
{
    Q_D(AbstractColumnRenderer);

    d->mLineHeight = lineHeight;
}

void AbstractColumnRenderer::restrictToXSpan(PixelXRange* xSpan) const
{
    Q_D(const AbstractColumnRenderer);

    xSpan->restrictTo(d->mXSpan);
}

bool AbstractColumnRenderer::overlaps(const PixelXRange& xSpan) const
{
    Q_D(const AbstractColumnRenderer);

    return d->mXSpan.overlaps(xSpan);
}

void AbstractColumnRenderer::renderFirstLine(QPainter* painter, const PixelXRange& xSpan, int firstLineIndex)
{
    Q_UNUSED(xSpan)
    Q_UNUSED(firstLineIndex)
    Q_UNUSED(painter)
//     d->renderBlankLine( painter );
}

void AbstractColumnRenderer::renderNextLine(QPainter* painter)
{
    Q_UNUSED(painter)
//     d->renderBlankLine( painter );
}

void AbstractColumnRenderer::renderBlankLine(QPainter* painter) const
{
    Q_D(const AbstractColumnRenderer);

    d->renderBlankLine(painter);
}

void AbstractColumnRenderer::renderColumn(QPainter* painter, const PixelXRange& xSpan, const PixelYRange& ySpan)
{
    Q_D(AbstractColumnRenderer);

    d->renderEmptyColumn(painter, xSpan, ySpan);
}

void AbstractColumnRenderer::renderEmptyColumn(QPainter* painter, const PixelXRange& xSpan, const PixelYRange& ySpan)
{
    Q_D(AbstractColumnRenderer);

    d->renderEmptyColumn(painter, xSpan, ySpan);
}

}
