/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearraycolumnrenderer.hpp"
#include "abstractbytearraycolumnrenderer_p.hpp"

// lib
#include "coord.hpp"

namespace Okteta {

AbstractByteArrayColumnRenderer::AbstractByteArrayColumnRenderer(AbstractByteArrayColumnRendererPrivate* d)
    : AbstractColumnRenderer(d)
{
}

AbstractByteArrayColumnRenderer::~AbstractByteArrayColumnRenderer() = default;

PixelX AbstractByteArrayColumnRenderer::byteWidth() const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->byteWidth();
}

PixelX AbstractByteArrayColumnRenderer::digitWidth() const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->digitWidth();
}

PixelX AbstractByteArrayColumnRenderer::byteSpacingWidth() const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->byteSpacingWidth();
}

PixelX AbstractByteArrayColumnRenderer::groupSpacingWidth() const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->groupSpacingWidth();
}

int AbstractByteArrayColumnRenderer::noOfGroupedBytes() const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->noOfGroupedBytes();
}

LinePosition AbstractByteArrayColumnRenderer::firstLinePos() const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->firstLinePos();
}

LinePosition AbstractByteArrayColumnRenderer::lastLinePos() const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->lastLinePos();
}

LinePositionRange AbstractByteArrayColumnRenderer::visibleLinePositions() const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->visibleLinePositions();
}

const ByteArrayTableLayout* AbstractByteArrayColumnRenderer::layout() const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->layout();
}

void AbstractByteArrayColumnRenderer::setCharCodec(const CharCodec* charCodec)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->setCharCodec(charCodec);
}

void AbstractByteArrayColumnRenderer::setByteTypeColored(bool byteTypeColored)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->setByteTypeColored(byteTypeColored);
}

bool AbstractByteArrayColumnRenderer::isByteTypeColored() const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->isByteTypeColored();
}

void AbstractByteArrayColumnRenderer::set(AbstractByteArrayModel* byteArrayModel)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->set(byteArrayModel);
}

void AbstractByteArrayColumnRenderer::resetXBuffer()
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->resetXBuffer();
}

void AbstractByteArrayColumnRenderer::setFontMetrics(const QFontMetrics& fontMetrics)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->setFontMetrics(fontMetrics);
}

bool AbstractByteArrayColumnRenderer::setSpacing(PixelX byteSpacingWidth, int NoGB, PixelX groupSpacingWidth)
{
    Q_D(AbstractByteArrayColumnRenderer);

    return d->setSpacing(byteSpacingWidth, NoGB, groupSpacingWidth);
}

bool AbstractByteArrayColumnRenderer::setByteSpacingWidth(PixelX byteSpacingWidth)
{
    Q_D(AbstractByteArrayColumnRenderer);

    return d->setByteSpacingWidth(byteSpacingWidth);
}

bool AbstractByteArrayColumnRenderer::setNoOfGroupedBytes(int NoGB)
{
    Q_D(AbstractByteArrayColumnRenderer);

    return d->setNoOfGroupedBytes(NoGB);
}

bool AbstractByteArrayColumnRenderer::setGroupSpacingWidth(PixelX groupSpacingWidth)
{
    Q_D(AbstractByteArrayColumnRenderer);

    return d->setGroupSpacingWidth(groupSpacingWidth);
}

LinePosition AbstractByteArrayColumnRenderer::linePositionOfX(PixelX PX) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->linePositionOfX(PX);
}

LinePosition AbstractByteArrayColumnRenderer::magneticLinePositionOfX(PixelX PX) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->magneticLinePositionOfX(PX);
}

LinePositionRange AbstractByteArrayColumnRenderer::linePositionsOfX(PixelX PX, PixelX PW) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->linePositionsOfX(PX, PW);
}

PixelX AbstractByteArrayColumnRenderer::xOfLinePosition(LinePosition linePosition) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->xOfLinePosition(linePosition);
}

PixelX AbstractByteArrayColumnRenderer::rightXOfLinePosition(LinePosition linePosition) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->rightXOfLinePosition(linePosition);
}

LinePosition AbstractByteArrayColumnRenderer::linePositionOfColumnX(PixelX PX) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->linePositionOfColumnX(PX);
}

LinePositionRange AbstractByteArrayColumnRenderer::linePositionsOfColumnXs(PixelX pixelX, PixelX pixelWidth) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->linePositionsOfColumnXs(pixelX, pixelWidth);
}

PixelX AbstractByteArrayColumnRenderer::columnXOfLinePosition(LinePosition linePosition) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->columnXOfLinePosition(linePosition);
}

PixelX AbstractByteArrayColumnRenderer::columnRightXOfLinePosition(LinePosition linePosition) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->columnRightXOfLinePosition(linePosition);
}

PixelXRange AbstractByteArrayColumnRenderer::xsOfLinePositionsInclSpaces(const LinePositionRange& linePositions) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->xsOfLinePositionsInclSpaces(linePositions);
}

PixelXRange AbstractByteArrayColumnRenderer::columnXsOfLinePositionsInclSpaces(const LinePositionRange& linePositions) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->columnXsOfLinePositionsInclSpaces(linePositions);
}

QRect AbstractByteArrayColumnRenderer::byteRect(const Coord& coord) const
{
    Q_D(const AbstractByteArrayColumnRenderer);

    return d->byteRect(coord);
}

void AbstractByteArrayColumnRenderer::prepareRendering(const PixelXRange& Xs)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->prepareRendering(Xs);
}

void AbstractByteArrayColumnRenderer::renderFirstLine(QPainter* painter, const PixelXRange& Xs, Line firstLineIndex)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->renderFirstLine(painter, Xs, firstLineIndex);
}

void AbstractByteArrayColumnRenderer::renderNextLine(QPainter* painter)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->renderNextLine(painter);
}

void AbstractByteArrayColumnRenderer::renderLinePositions(QPainter* painter, Line lineIndex, const LinePositionRange& linePositions)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->renderLinePositions(painter, lineIndex, linePositions);
}

void AbstractByteArrayColumnRenderer::renderByte(QPainter* painter, Address byteIndex)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->renderByte(painter, byteIndex);
}

void AbstractByteArrayColumnRenderer::renderFramedByte(QPainter* painter, Address byteIndex, FrameStyle frameStyle)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->renderFramedByte(painter, byteIndex, frameStyle);
}

void AbstractByteArrayColumnRenderer::renderCursor(QPainter* painter, Address byteIndex)
{
    Q_D(AbstractByteArrayColumnRenderer);

    d->renderCursor(painter, byteIndex);
}

}
