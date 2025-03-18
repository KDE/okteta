/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "offsetcolumnrenderer_p.hpp"

// lib
#include "bytearraytablelayout.hpp"
// lib
#include <abstractcolumnstylist.hpp>
// Qt
#include <QPainter>
#include <QPalette>
#include <QFontMetrics>


namespace Okteta {

void OffsetColumnRendererPrivate::renderLine(QPainter* painter, Line lineIndex)
{
    const PixelX offset = mLayout->firstLineOffset() + mLayout->noOfBytesPerLine() * lineIndex;
    PrintFunction(mCodedOffset, offset);

    const QColor& buttonColor = mStylist->palette().buttonText().color();
    painter->setPen(buttonColor);
    painter->drawText(mMargin, mDigitBaseLine, QLatin1String(mCodedOffset));
}

void OffsetColumnRendererPrivate::renderColumnBackground(QPainter* painter, PixelXRange Xs, PixelYRange Ys)
{
    Q_Q(OffsetColumnRenderer);

    q->restrictToXSpan(&Xs);

    const QBrush& buttonBrush = mStylist->palette().button();
    painter->fillRect(Xs.start(), Ys.start(), Xs.width(), Ys.width(), buttonBrush);
}

void OffsetColumnRendererPrivate::renderFirstLine(QPainter* painter, PixelXRange Xs, Line firstLineIndex)
{
    Q_UNUSED(Xs)

    mRenderLineIndex = firstLineIndex;
    renderLine(painter, mRenderLineIndex++);
}

void OffsetColumnRendererPrivate::renderNextLine(QPainter* painter)
{
    renderLine(painter, mRenderLineIndex++);
}

void OffsetColumnRendererPrivate::renderColumn(QPainter* painter, PixelXRange Xs, PixelYRange Ys)
{
    renderColumnBackground(painter, Xs, Ys);
}

void OffsetColumnRendererPrivate::renderEmptyColumn(QPainter* painter, PixelXRange Xs, PixelYRange Ys)
{
    renderColumnBackground(painter, Xs, Ys);
}

void OffsetColumnRendererPrivate::setFormat(OffsetFormat::Format format, const QFontMetrics& fontMetrics)
{
    // no changes?
    if (mFormat == format) {
        return;
    }

    mFormat = format;

    PrintFunction = OffsetFormat::printFunction(mFormat);

    setFontMetrics(fontMetrics);
}

void OffsetColumnRendererPrivate::setFontMetrics(const QFontMetrics& fontMetrics)
{
    mDigitBaseLine = fontMetrics.ascent();

    // use 0 as reference, using a fixed font should always yield same width
    PrintFunction(mCodedOffset, 0);
    const int newOffsetTextWidth = fontMetrics.horizontalAdvance(QLatin1String(mCodedOffset));

    if (newOffsetTextWidth == mOffsetTextWidth) {
        return;
    }

    mOffsetTextWidth = newOffsetTextWidth;

    recalcX();
}

void OffsetColumnRendererPrivate::setMargin(PixelX margin)
{
    // no changes?
    if (mMargin == margin) {
        return;
    }

    mMargin = margin;

    recalcX();
}

void OffsetColumnRendererPrivate::recalcX()
{
    Q_Q(OffsetColumnRenderer);

    // recalculate depend sizes
    q->setWidth(mOffsetTextWidth + 2 * mMargin);
}

}
