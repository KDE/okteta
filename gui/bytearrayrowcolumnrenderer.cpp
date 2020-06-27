/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrowcolumnrenderer.hpp"
#include "bytearrayrowcolumnrenderer_p.hpp"

// lib
#include "coord.hpp"

namespace Okteta {

ByteArrayRowColumnRenderer::ByteArrayRowColumnRenderer(AbstractColumnStylist* stylist,
                                                       AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges)
    : AbstractColumnRenderer(new ByteArrayRowColumnRendererPrivate(this, stylist, byteArrayModel, layout, ranges))
{
}

ByteArrayRowColumnRenderer::~ByteArrayRowColumnRenderer() = default;

AbstractByteArrayView::CodingTypes ByteArrayRowColumnRenderer::visibleCodings() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->visibleCodings();
}

PixelX ByteArrayRowColumnRenderer::byteWidth() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->byteWidth();
}

PixelX ByteArrayRowColumnRenderer::digitWidth() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->digitWidth();
}

PixelX ByteArrayRowColumnRenderer::byteSpacingWidth() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->byteSpacingWidth();
}

PixelX ByteArrayRowColumnRenderer::groupSpacingWidth() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->groupSpacingWidth();
}

PixelY ByteArrayRowColumnRenderer::digitHeight() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->digitHeight();
}

Size ByteArrayRowColumnRenderer::noOfGroupedBytes() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->noOfGroupedBytes();
}

LinePosition ByteArrayRowColumnRenderer::firstLinePos() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->firstLinePos();
}

LinePosition ByteArrayRowColumnRenderer::lastLinePos() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->lastLinePos();
}

LinePositionRange ByteArrayRowColumnRenderer::visibleLinePositions() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->visibleLinePositions();
}

const ByteArrayTableLayout* ByteArrayRowColumnRenderer::layout() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->layout();
}

void ByteArrayRowColumnRenderer::setCharCodec(const CharCodec* charCodec)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->setCharCodec(charCodec);
}

void ByteArrayRowColumnRenderer::setByteTypeColored(bool byteTypeColored)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->setByteTypeColored(byteTypeColored);
}

bool ByteArrayRowColumnRenderer::isByteTypeColored() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->isByteTypeColored();
}

PixelX ByteArrayRowColumnRenderer::binaryGapWidth() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->binaryGapWidth();
}

bool ByteArrayRowColumnRenderer::isShowingNonprinting() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->isShowingNonprinting();
}

QChar ByteArrayRowColumnRenderer::substituteChar() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->substituteChar();
}

QChar ByteArrayRowColumnRenderer::undefinedChar() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->undefinedChar();
}

bool ByteArrayRowColumnRenderer::setSubstituteChar(QChar substituteChar)
{
    Q_D(ByteArrayRowColumnRenderer);

    return d->setSubstituteChar(substituteChar);
}

bool ByteArrayRowColumnRenderer::setUndefinedChar(QChar undefinedChar)
{
    Q_D(ByteArrayRowColumnRenderer);

    return d->setUndefinedChar(undefinedChar);
}

bool ByteArrayRowColumnRenderer::setShowingNonprinting(bool showingNonprinting)
{
    Q_D(ByteArrayRowColumnRenderer);

    return d->setShowingNonprinting(showingNonprinting);
}

AbstractByteArrayView::CodingTypeId ByteArrayRowColumnRenderer::codingIdofY(PixelY y) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->codingIdofY(y);
}

PixelY ByteArrayRowColumnRenderer::rowHeight() const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->rowHeight();
}

PixelY ByteArrayRowColumnRenderer::yOfCodingId(AbstractByteArrayView::CodingTypeId codingId) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->yOfCodingId(codingId);
}

void ByteArrayRowColumnRenderer::set(AbstractByteArrayModel* byteArrayModel)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->set(byteArrayModel);
}

void ByteArrayRowColumnRenderer::resetXBuffer()
{
    Q_D(ByteArrayRowColumnRenderer);

    d->resetXBuffer();
}

void ByteArrayRowColumnRenderer::setVisibleCodings(int visibleCodings)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->setVisibleCodings(visibleCodings);
}

void ByteArrayRowColumnRenderer::setFontMetrics(const QFontMetrics& fontMetrics)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->setFontMetrics(fontMetrics);
}

bool ByteArrayRowColumnRenderer::setSpacing(PixelX byteSpacingWidth, Size NoGB, PixelX groupSpacingWidth)
{
    Q_D(ByteArrayRowColumnRenderer);

    return d->setSpacing(byteSpacingWidth, NoGB, groupSpacingWidth);
}

bool ByteArrayRowColumnRenderer::setByteSpacingWidth(PixelX byteSpacingWidth)
{
    Q_D(ByteArrayRowColumnRenderer);

    return d->setByteSpacingWidth(byteSpacingWidth);
}

bool ByteArrayRowColumnRenderer::setNoOfGroupedBytes(Size NoGB)
{
    Q_D(ByteArrayRowColumnRenderer);

    return d->setNoOfGroupedBytes(NoGB);
}

bool ByteArrayRowColumnRenderer::setGroupSpacingWidth(PixelX groupSpacingWidth)
{
    Q_D(ByteArrayRowColumnRenderer);

    return d->setGroupSpacingWidth(groupSpacingWidth);
}

void ByteArrayRowColumnRenderer::setValueCodec(ValueCoding valueCoding, const ValueCodec* valueCodec)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->setValueCodec(valueCoding, valueCodec);
}

bool ByteArrayRowColumnRenderer::setBinaryGapWidth(PixelX binaryGapWidth)
{
    Q_D(ByteArrayRowColumnRenderer);

    return d->setBinaryGapWidth(binaryGapWidth);
}

// perhaps sometimes there will be a grammar
void ByteArrayRowColumnRenderer::renderEditedByte(QPainter* painter, Byte byte, const QString& editBuffer)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->renderEditedByte(painter, byte, editBuffer);
}

// TODO: why are inlined functions not available as symbols when defined before their use
// TODO: works not precisely for the byte rects but includes spacing and left and right
/*inline*/ LinePosition ByteArrayRowColumnRenderer::linePositionOfX(PixelX PX) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->linePositionOfX(PX);
}

LinePosition ByteArrayRowColumnRenderer::magneticLinePositionOfX(PixelX PX) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->magneticLinePositionOfX(PX);
}

LinePositionRange ByteArrayRowColumnRenderer::linePositionsOfX(PixelX PX, PixelX PW) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->linePositionsOfX(PX, PW);
}

PixelX ByteArrayRowColumnRenderer::xOfLinePosition(LinePosition linePosition) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->xOfLinePosition(linePosition);
}

PixelX ByteArrayRowColumnRenderer::rightXOfLinePosition(LinePosition linePosition) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->rightXOfLinePosition(linePosition);
}

LinePosition ByteArrayRowColumnRenderer::linePositionOfColumnX(PixelX PX) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->linePositionOfColumnX(PX);
}

LinePositionRange ByteArrayRowColumnRenderer::linePositionsOfColumnXs(PixelX pixelX, PixelX pixelWidth) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->linePositionsOfColumnXs(pixelX, pixelWidth);
}

PixelX ByteArrayRowColumnRenderer::columnXOfLinePosition(LinePosition linePosition) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->columnXOfLinePosition(linePosition);
}

PixelX ByteArrayRowColumnRenderer::columnRightXOfLinePosition(LinePosition linePosition) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->columnRightXOfLinePosition(linePosition);
}

PixelXRange ByteArrayRowColumnRenderer::xsOfLinePositionsInclSpaces(const LinePositionRange& linePositions) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->xsOfLinePositionsInclSpaces(linePositions);
}

PixelXRange ByteArrayRowColumnRenderer::columnXsOfLinePositionsInclSpaces(const LinePositionRange& linePositions) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->columnXsOfLinePositionsInclSpaces(linePositions);
}

QRect ByteArrayRowColumnRenderer::byteRect(const Coord& coord) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->byteRect(coord);
}

QRect ByteArrayRowColumnRenderer::byteRect(const Coord& coord, AbstractByteArrayView::CodingTypeId codingId) const
{
    Q_D(const ByteArrayRowColumnRenderer);

    return d->byteRect(coord, codingId);
}

void ByteArrayRowColumnRenderer::prepareRendering(const PixelXRange& Xs)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->prepareRendering(Xs);
}

void ByteArrayRowColumnRenderer::renderFirstLine(QPainter* painter, const PixelXRange& Xs, Line firstLineIndex)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->renderFirstLine(painter, Xs, firstLineIndex);
}

void ByteArrayRowColumnRenderer::renderNextLine(QPainter* painter)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->renderNextLine(painter);
}

void ByteArrayRowColumnRenderer::renderByte(QPainter* painter,
                                            Address byteIndex, AbstractByteArrayView::CodingTypeId codingId)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->renderByte(painter, byteIndex, codingId);
}

// TODO: think about making framestyle a enum of a class ByteArrayColumnCursor
void ByteArrayRowColumnRenderer::renderFramedByte(QPainter* painter,
                                                  Address byteIndex, AbstractByteArrayView::CodingTypeId codingId,
                                                  FrameStyle frameStyle)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->renderFramedByte(painter, byteIndex, codingId, frameStyle);
}

void ByteArrayRowColumnRenderer::renderCursor(QPainter* painter, Address byteIndex, AbstractByteArrayView::CodingTypeId codingId)
{
    Q_D(ByteArrayRowColumnRenderer);

    d->renderCursor(painter, byteIndex, codingId);
}


}
