/*
    SPDX-FileCopyrightText: 2003, 2008-2009, 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycolumnframerenderer.hpp"

// lib
#include "printcolumnstylist.hpp"
// Okteta gui
#include <Okteta/OffsetColumnRenderer>
#include <Okteta/BorderColumnRenderer>
#include <Okteta/ValueByteArrayColumnRenderer>
#include <Okteta/CharByteArrayColumnRenderer>
#include <Okteta/ByteArrayTableLayout>
#include <Okteta/ByteArrayTableRanges>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ValueCodec>
#include <Okteta/CharCodec>
// Qt
#include <QFontMetrics>
#include <QFontDatabase>
// Std
#include <utility>

static constexpr Okteta::OffsetFormat::Format DefaultColumnOffsetCoding =  Okteta::OffsetFormat::Hexadecimal; // krazy:exclude=staticobjects

ByteArrayColumnFrameRenderer::ByteArrayColumnFrameRenderer()
{
    // creating the columns in the needed order
    auto offsetColumnRenderer =
        std::make_unique<Okteta::OffsetColumnRenderer>(mStylist.get(), mLayout.get(), DefaultColumnOffsetCoding);
    mOffsetColumnRenderer = offsetColumnRenderer.get();
    auto  firstBorderColumnRenderer =
        std::make_unique<Okteta::BorderColumnRenderer>(mStylist.get(), true, false);
    mFirstBorderColumnRenderer = firstBorderColumnRenderer.get();
    auto valueColumnRenderer =
        std::make_unique<Okteta::ValueByteArrayColumnRenderer>(mStylist.get(), mByteArrayModel, mLayout.get(), mTableRanges.get());
    mValueColumnRenderer = valueColumnRenderer.get();
    auto secondBorderColumnRenderer =
        std::make_unique<Okteta::BorderColumnRenderer>(mStylist.get(), true, false);
    mSecondBorderColumnRenderer = secondBorderColumnRenderer.get();
    auto charColumnRenderer =
        std::make_unique<Okteta::CharByteArrayColumnRenderer>(mStylist.get(), mByteArrayModel, mLayout.get(), mTableRanges.get());
    mCharColumnRenderer = charColumnRenderer.get();

    addColumn(std::move(offsetColumnRenderer));
    addColumn(std::move(firstBorderColumnRenderer));
    addColumn(std::move(valueColumnRenderer));
    addColumn(std::move(secondBorderColumnRenderer));
    addColumn(std::move(charColumnRenderer));

    mValueColumnRenderer->setValueCodec((Okteta::ValueCoding)mValueCoding, mValueCodec.get());
    mValueColumnRenderer->setCharCodec(mCharCodec.get());
    mCharColumnRenderer->setCharCodec(mCharCodec.get());

    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

ByteArrayColumnFrameRenderer::~ByteArrayColumnFrameRenderer() = default;

Okteta::PixelX ByteArrayColumnFrameRenderer::byteSpacingWidth()           const { return mValueColumnRenderer->byteSpacingWidth(); }
int ByteArrayColumnFrameRenderer::noOfGroupedBytes()               const { return mValueColumnRenderer->noOfGroupedBytes(); }
Okteta::PixelX ByteArrayColumnFrameRenderer::groupSpacingWidth()          const { return mValueColumnRenderer->groupSpacingWidth(); }
Okteta::PixelX ByteArrayColumnFrameRenderer::binaryGapWidth()             const { return mValueColumnRenderer->binaryGapWidth(); }
bool ByteArrayColumnFrameRenderer::showsNonprinting()              const { return mCharColumnRenderer->isShowingNonprinting(); }
QChar ByteArrayColumnFrameRenderer::substituteChar()               const { return mCharColumnRenderer->substituteChar(); }
QChar ByteArrayColumnFrameRenderer::undefinedChar()                const { return mCharColumnRenderer->undefinedChar(); }

bool ByteArrayColumnFrameRenderer::offsetColumnVisible() const { return mOffsetColumnRenderer->isVisible(); }
Okteta::OffsetFormat::Format ByteArrayColumnFrameRenderer::offsetCoding() const { return mOffsetColumnRenderer->format(); }

int ByteArrayColumnFrameRenderer::visibleByteArrayCodings() const
{ return (mValueColumnRenderer->isVisible() ? ValueCodingId : 0) | (mCharColumnRenderer->isVisible() ? CharCodingId : 0); }

void ByteArrayColumnFrameRenderer::setByteArrayModel(Okteta::AbstractByteArrayModel* byteArrayModel,
                                               Okteta::Address offset, Okteta::Size length)
{
    mByteArrayModel = byteArrayModel;
    length = (!byteArrayModel) ?                            0 :
             (length == -1) ?                               byteArrayModel->size() - offset :
             (length <= byteArrayModel->size() - offset) ?  length :
             /* else */                                     byteArrayModel->size() - offset;

    mValueColumnRenderer->set(byteArrayModel);
    mCharColumnRenderer->set(byteArrayModel);

    // affected:
    // length -> no of lines -> width
    mLayout->setByteArrayOffset(offset);
    mLayout->setLength(length);

    adjustLayoutToSize();
}

void ByteArrayColumnFrameRenderer::setBufferSpacing(Okteta::PixelX byteSpacing, int noOfGroupedBytes, Okteta::PixelX groupSpacing)
{
    if (!mValueColumnRenderer->setSpacing(byteSpacing, noOfGroupedBytes, groupSpacing)) {
        return;
    }

    adjustToWidth();
}

void ByteArrayColumnFrameRenderer::setByteSpacingWidth(Okteta::PixelX byteSpacingWidth)
{
    if (!mValueColumnRenderer->setByteSpacingWidth(byteSpacingWidth)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayColumnFrameRenderer::setNoOfGroupedBytes(int noOfGroupedBytes)
{
    if (!mValueColumnRenderer->setNoOfGroupedBytes(noOfGroupedBytes)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayColumnFrameRenderer::setGroupSpacingWidth(Okteta::PixelX groupSpacingWidth)
{
    if (!mValueColumnRenderer->setGroupSpacingWidth(groupSpacingWidth)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayColumnFrameRenderer::setBinaryGapWidth(Okteta::PixelX binaryGapWidth)
{
    if (!mValueColumnRenderer->setBinaryGapWidth(binaryGapWidth)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayColumnFrameRenderer::setSubstituteChar(QChar substituteChar)
{
    mCharColumnRenderer->setSubstituteChar(substituteChar);
}

void ByteArrayColumnFrameRenderer::setUndefinedChar(QChar undefinedChar)
{
    mCharColumnRenderer->setUndefinedChar(undefinedChar);
}

void ByteArrayColumnFrameRenderer::setShowsNonprinting(bool showsNonprinting)
{
    mCharColumnRenderer->setShowingNonprinting(showsNonprinting);
}

void ByteArrayColumnFrameRenderer::setValueCoding(Okteta::ValueCoding valueCoding)
{
    if (mValueCoding == valueCoding) {
        return;
    }

    const uint oldCodingWidth = mValueCodec->encodingWidth();

    auto newValueCodec = Okteta::ValueCodec::createCodec(valueCoding);
    if (!newValueCodec) {
        return;
    }

    mValueCodec = std::move(newValueCodec);
    mValueCoding = valueCoding;

    mValueColumnRenderer->setValueCodec((Okteta::ValueCoding)mValueCoding, mValueCodec.get());

    const uint newCodingWidth = mValueCodec->encodingWidth();

    // change in the width?
    if (newCodingWidth != oldCodingWidth) {
        adjustToWidth();
    }
}

void ByteArrayColumnFrameRenderer::setCharCoding(const QString& newCharCodingName)
{
    if (charCodingName() == newCharCodingName) {
        return;
    }

    auto newCharCodec = Okteta::CharCodec::createCodec(newCharCodingName);
    if (!newCharCodec) {
        return;
    }

    mCharCodec = std::move(newCharCodec);

    mValueColumnRenderer->setCharCodec(mCharCodec.get());
    mCharColumnRenderer->setCharCodec(mCharCodec.get());
}

void ByteArrayColumnFrameRenderer::setFont(const QFont& font)
{
    mFont = font;

    // get new values
    QFontMetrics fontMetrics(font);

    setLineHeight(fontMetrics.height());

    // update all dependent structures
    mLayout->setNoOfLinesPerPage(noOfLinesPerFrame());

    mOffsetColumnRenderer->setFontMetrics(fontMetrics);
    mValueColumnRenderer->setFontMetrics(fontMetrics);
    mCharColumnRenderer->setFontMetrics(fontMetrics);

    adjustToWidth();
}

void ByteArrayColumnFrameRenderer::adjustToLayoutNoOfBytesPerLine()
{
    mValueColumnRenderer->resetXBuffer();
    mCharColumnRenderer->resetXBuffer();

    updateWidths();
}

void ByteArrayColumnFrameRenderer::showOffsetColumn(bool visible)
{
    bool OCVisible = mOffsetColumnRenderer->isVisible();
    // no change?
    if (OCVisible == visible) {
        return;
    }

    mOffsetColumnRenderer->setVisible(visible);
    mFirstBorderColumnRenderer->setVisible(visible);

    adjustToWidth();
}

void ByteArrayColumnFrameRenderer::setOffsetCoding(Okteta::OffsetFormat::Format offsetCoding)
{
    const Okteta::OffsetFormat::Format currentFormat = mOffsetColumnRenderer->format();
    // no change?
    if (currentFormat == offsetCoding) {
        return;
    }

    mOffsetColumnRenderer->setFormat(offsetCoding, QFontMetrics(mFont));

    adjustToWidth();
}

#if 0
QSize ByteArrayColumnFrameRenderer::sizeHint() const
{
    return QSize(columnsWidth(), columnsHeight());
}

QSize ByteArrayColumnFrameRenderer::minimumSizeHint() const
{
    // TODO: better minimal width (visibility!)

    const int width =
        mOffsetColumnRenderer->visibleWidth()
        + mFirstBorderColumnRenderer->visibleWidth()
        + mSecondBorderColumnRenderer->visibleWidth()
        + mValueColumnRenderer->byteWidth()
        + mCharColumnRenderer->byteWidth();
    const int height = lineHeight() * noOfLines();

    return QSize(width, height);
}
#endif

int ByteArrayColumnFrameRenderer::fittingBytesPerLine() const
{
    const Okteta::PixelX nonDataWidth =
        mOffsetColumnRenderer->visibleWidth()
        + mFirstBorderColumnRenderer->visibleWidth()
        + mSecondBorderColumnRenderer->visibleWidth();

    // abstract offset and border columns width
    const Okteta::PixelX maxDataWidth = width() - nonDataWidth;

    // prepare needed values
    const Okteta::PixelX charByteWidth = mCharColumnRenderer->isVisible() ? mCharColumnRenderer->digitWidth() : 0;
    const Okteta::PixelX valueByteWidth = mValueColumnRenderer->isVisible() ? mValueColumnRenderer->byteWidth() : 0;
    const Okteta::PixelX valueByteSpacingWidth = mValueColumnRenderer->isVisible() ? mValueColumnRenderer->byteSpacingWidth() : 0;
    Okteta::PixelX valueByteGroupSpacingWidth;
    int noOfGroupedBytes = mValueColumnRenderer->noOfGroupedBytes();
    // no grouping?
    if (noOfGroupedBytes == 0) {
        // faking grouping by 1
        noOfGroupedBytes = 1;
        valueByteGroupSpacingWidth = 0;
    } else {
        valueByteGroupSpacingWidth = mValueColumnRenderer->isVisible() ? mValueColumnRenderer->groupSpacingWidth() : 0;
    }

    Okteta::PixelX valueByteGroupWidth =  noOfGroupedBytes * valueByteWidth + (noOfGroupedBytes - 1) * valueByteSpacingWidth;
    Okteta::PixelX charByteGroupWidth =   noOfGroupedBytes * charByteWidth;
    Okteta::PixelX charAndValueGroupWidth = (valueByteGroupWidth + valueByteGroupSpacingWidth) + charByteGroupWidth;

    // calculate fitting groups per line

    // the last value byte group does not need a group spacing behind, but it gets into the calculation.
    // so we simply add one to the max width to match that
    const int fittingGroupsPerLine = (maxDataWidth + valueByteGroupSpacingWidth)
                                     / charAndValueGroupWidth;

    // calculate the fitting bytes per line by groups
    int fittingBytesPerLine = noOfGroupedBytes * fittingGroupsPerLine;

    // groups can be split and not only full groups are requested?
    if (noOfGroupedBytes > 1 && mResizeStyle == FullSizeLayoutStyle) {
        const int leftDataWidth = maxDataWidth - fittingGroupsPerLine * charAndValueGroupWidth;

        if (leftDataWidth > 0) {
            const int charAndValueByteWidth = valueByteWidth + valueByteSpacingWidth + charByteWidth;
            // the last value byte  does not need a spacing behind, but it gets into the calculation.
            // so we simply add one to the left width to match that
            const int ungroupedBytes = (leftDataWidth + valueByteSpacingWidth)
                                       / charAndValueByteWidth;
            fittingBytesPerLine += ungroupedBytes;
        }

        // is there not even the space for a single byte?
//         if( fittingBytesPerLine < 1 )
        // ensure at least one byte per line
//             fittingBytesPerLine = 1;
    } else {
        // is there not the space for a single group?
//         if( fittingBytesPerLine < 1 )
        // ensures at least one group
//             fittingBytesPerLine = noOfGroupedBytes;
    }

    return fittingBytesPerLine;
}

void ByteArrayColumnFrameRenderer::showByteArrayColumns(int newColumns)
{
    int columns = visibleByteArrayCodings();

    // no changes or no column selected?
    if (newColumns == columns || !(newColumns & (ValueCodingId | CharCodingId))) {
        return;
    }

    mValueColumnRenderer->setVisible(ValueCodingId & newColumns);
    mCharColumnRenderer->setVisible(CharCodingId & newColumns);
    mSecondBorderColumnRenderer->setVisible(newColumns == (ValueCodingId | CharCodingId));

    adjustToWidth();
}
