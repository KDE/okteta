/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "offsetcolumnrenderer.hpp"
#include "offsetcolumnrenderer_p.hpp"

namespace Okteta {

OffsetColumnRenderer::OffsetColumnRenderer(AbstractColumnStylist* stylist,
                                           ByteArrayTableLayout* layout, OffsetFormat::Format format)
    : AbstractColumnRenderer(new OffsetColumnRendererPrivate(this, stylist, layout, format))
{
    Q_D(OffsetColumnRenderer);

    d->init();
}

OffsetColumnRenderer::~OffsetColumnRenderer() = default;

OffsetFormat::Format OffsetColumnRenderer::format() const
{
    Q_D(const OffsetColumnRenderer);

    return d->mFormat;
}

void OffsetColumnRenderer::renderFirstLine(QPainter* painter, const PixelXRange& Xs, Line firstLineIndex)
{
    Q_D(OffsetColumnRenderer);

    d->renderFirstLine(painter, Xs, firstLineIndex);
}

void OffsetColumnRenderer::renderNextLine(QPainter* painter)
{
    Q_D(OffsetColumnRenderer);

    d->renderNextLine(painter);
}

void OffsetColumnRenderer::renderColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys)
{
    Q_D(OffsetColumnRenderer);

    d->renderColumn(painter, Xs, Ys);
}

void OffsetColumnRenderer::renderEmptyColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys)
{
    Q_D(OffsetColumnRenderer);

    d->renderEmptyColumn(painter, Xs, Ys);
}

void OffsetColumnRenderer::setFormat(OffsetFormat::Format format, const QFontMetrics& fontMetrics)
{
    Q_D(OffsetColumnRenderer);

    d->setFormat(format, fontMetrics);
}

void OffsetColumnRenderer::setFontMetrics(const QFontMetrics& fontMetrics)
{
    Q_D(OffsetColumnRenderer);

    d->setFontMetrics(fontMetrics);
}


}
