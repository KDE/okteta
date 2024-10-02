/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008-2009, 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrowframerenderer.hpp"

// lib
#include "printcolumnstylist.hpp"
// Okteta gui
#include <Okteta/OffsetColumnRenderer>
#include <Okteta/BorderColumnRenderer>
#include <Okteta/ByteArrayRowColumnRenderer>
#include <Okteta/ByteArrayTableLayout>
#include <Okteta/ByteArrayTableRanges>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ValueCodec>
#include <Okteta/CharCodec>
// Qt
#include <QHash>
#include <QFontMetrics>
#include <QFontDatabase>

static constexpr Okteta::OffsetFormat::Format DefaultRowOffsetCoding =  Okteta::OffsetFormat::Hexadecimal; // krazy:exclude=staticobjects

ByteArrayRowFrameRenderer::ByteArrayRowFrameRenderer()
{
    // creating the columns in the needed order
    mOffsetColumnRenderer =
        new Okteta::OffsetColumnRenderer(mStylist, mLayout, DefaultRowOffsetCoding);
    mFirstBorderColumnRenderer =
        new Okteta::BorderColumnRenderer(mStylist, true, false);
    mByteArrayColumnRenderer =
        new Okteta::ByteArrayRowColumnRenderer(mStylist, mByteArrayModel, mLayout, mTableRanges);

    addColumn(mOffsetColumnRenderer);
    addColumn(mFirstBorderColumnRenderer);
    addColumn(mByteArrayColumnRenderer);

    mByteArrayColumnRenderer->setValueCodec((Okteta::ValueCoding)mValueCoding, mValueCodec);
    mByteArrayColumnRenderer->setCharCodec(mCharCodec);

    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

ByteArrayRowFrameRenderer::~ByteArrayRowFrameRenderer() = default;

Okteta::PixelX ByteArrayRowFrameRenderer::byteSpacingWidth()           const { return mByteArrayColumnRenderer->byteSpacingWidth(); }
int ByteArrayRowFrameRenderer::noOfGroupedBytes()               const { return mByteArrayColumnRenderer->noOfGroupedBytes(); }
Okteta::PixelX ByteArrayRowFrameRenderer::groupSpacingWidth()          const { return mByteArrayColumnRenderer->groupSpacingWidth(); }
Okteta::PixelX ByteArrayRowFrameRenderer::binaryGapWidth()             const { return mByteArrayColumnRenderer->binaryGapWidth(); }
bool ByteArrayRowFrameRenderer::showsNonprinting()              const { return mByteArrayColumnRenderer->isShowingNonprinting(); }
QChar ByteArrayRowFrameRenderer::substituteChar()               const { return mByteArrayColumnRenderer->substituteChar(); }
QChar ByteArrayRowFrameRenderer::undefinedChar()                const { return mByteArrayColumnRenderer->undefinedChar(); }

bool ByteArrayRowFrameRenderer::offsetColumnVisible() const { return mOffsetColumnRenderer->isVisible(); }
Okteta::OffsetFormat::Format ByteArrayRowFrameRenderer::offsetCoding() const { return mOffsetColumnRenderer->format(); }

int ByteArrayRowFrameRenderer::visibleByteArrayCodings() const
{ return mByteArrayColumnRenderer->visibleCodings(); }

void ByteArrayRowFrameRenderer::setByteArrayModel(Okteta::AbstractByteArrayModel* byteArrayModel,
                                               Okteta::Address offset, Okteta::Size length)
{
    mByteArrayModel = byteArrayModel;
    length = (!byteArrayModel) ?                            0 :
             (length == -1) ?                               byteArrayModel->size() - offset :
             (length <= byteArrayModel->size() - offset) ?  length :
             /* else */                                     byteArrayModel->size() - offset;

    mByteArrayColumnRenderer->set(byteArrayModel);

    // affected:
    // length -> no of lines -> width
    mLayout->setByteArrayOffset(offset);
    mLayout->setLength(length);

    adjustLayoutToSize();
}

void ByteArrayRowFrameRenderer::setBufferSpacing(Okteta::PixelX byteSpacing, int noOfGroupedBytes, Okteta::PixelX groupSpacing)
{
    if (!mByteArrayColumnRenderer->setSpacing(byteSpacing, noOfGroupedBytes, groupSpacing)) {
        return;
    }

    adjustToWidth();
}

void ByteArrayRowFrameRenderer::setByteSpacingWidth(Okteta::PixelX byteSpacingWidth)
{
    if (!mByteArrayColumnRenderer->setByteSpacingWidth(byteSpacingWidth)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayRowFrameRenderer::setNoOfGroupedBytes(int noOfGroupedBytes)
{
    if (!mByteArrayColumnRenderer->setNoOfGroupedBytes(noOfGroupedBytes)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayRowFrameRenderer::setGroupSpacingWidth(Okteta::PixelX groupSpacingWidth)
{
    if (!mByteArrayColumnRenderer->setGroupSpacingWidth(groupSpacingWidth)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayRowFrameRenderer::setBinaryGapWidth(Okteta::PixelX binaryGapWidth)
{
    if (!mByteArrayColumnRenderer->setBinaryGapWidth(binaryGapWidth)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayRowFrameRenderer::setSubstituteChar(QChar substituteChar)
{
    mByteArrayColumnRenderer->setSubstituteChar(substituteChar);
}

void ByteArrayRowFrameRenderer::setUndefinedChar(QChar undefinedChar)
{
    mByteArrayColumnRenderer->setUndefinedChar(undefinedChar);
}

void ByteArrayRowFrameRenderer::setShowsNonprinting(bool showsNonprinting)
{
    mByteArrayColumnRenderer->setShowingNonprinting(showsNonprinting);
}

void ByteArrayRowFrameRenderer::setValueCoding(Okteta::ValueCoding valueCoding)
{
    if (mValueCoding == valueCoding) {
        return;
    }

    const uint oldCodingWidth = mValueCodec->encodingWidth();

    Okteta::ValueCodec* newValueCodec =
        Okteta::ValueCodec::createCodec(valueCoding);
    if (!newValueCodec) {
        return;
    }

    delete mValueCodec;
    mValueCodec = newValueCodec;
    mValueCoding = valueCoding;

    mByteArrayColumnRenderer->setValueCodec((Okteta::ValueCoding)mValueCoding, mValueCodec);

    const uint newCodingWidth = mValueCodec->encodingWidth();

    // change in the width?
    if (newCodingWidth != oldCodingWidth) {
        adjustToWidth();
    }
}

void ByteArrayRowFrameRenderer::setCharCoding(Okteta::CharCoding charCoding)
{
    if (mCharCoding == charCoding) {
        return;
    }

    Okteta::CharCodec* newCharCodec = Okteta::CharCodec::createCodec(charCoding);
    if (!newCharCodec) {
        return;
    }

    delete mCharCodec;
    mCharCodec = newCharCodec;
    mCharCoding = charCoding;

    mByteArrayColumnRenderer->setCharCodec(mCharCodec);
}

// TODO: join with function above!
void ByteArrayRowFrameRenderer::setCharCoding(const QString& newCharCodingName)
{
    if (charCodingName() == newCharCodingName) {
        return;
    }

    Okteta::CharCodec* newCharCodec = Okteta::CharCodec::createCodec(newCharCodingName);
    if (!newCharCodec) {
        return;
    }

    delete mCharCodec;
    mCharCodec = newCharCodec;
    mCharCoding = Okteta::LocalEncoding; // TODO: add encoding no to every known codec

    mByteArrayColumnRenderer->setCharCodec(mCharCodec);
}

void ByteArrayRowFrameRenderer::setFont(const QFont& font)
{
    mFont = font;

    // get new values
    QFontMetrics fontMetrics(font);

    mOffsetColumnRenderer->setFontMetrics(fontMetrics);
    mByteArrayColumnRenderer->setFontMetrics(fontMetrics);

    const int rowHeight = mByteArrayColumnRenderer->rowHeight();

    setLineHeight(rowHeight);

    // update all dependent structures
    mLayout->setNoOfLinesPerPage(noOfLinesPerFrame());

    adjustToWidth();
}

void ByteArrayRowFrameRenderer::adjustToLayoutNoOfBytesPerLine()
{
    mByteArrayColumnRenderer->resetXBuffer();

    updateWidths();
}

void ByteArrayRowFrameRenderer::showOffsetColumn(bool visible)
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

void ByteArrayRowFrameRenderer::setOffsetCoding(Okteta::OffsetFormat::Format offsetCoding)
{
    const Okteta::OffsetFormat::Format currentFormat = mOffsetColumnRenderer->format();
    // no change?
    if (currentFormat == offsetCoding) {
        return;
    }

    mOffsetColumnRenderer->setFormat(offsetCoding, QFontMetrics(mFont));

    adjustToWidth();
}

int ByteArrayRowFrameRenderer::fittingBytesPerLine() const
{
    const Okteta::PixelX nonDataWidth =
        mOffsetColumnRenderer->visibleWidth()
        + mFirstBorderColumnRenderer->visibleWidth();

    // abstract offset and border columns width
    const Okteta::PixelX maxDataWidth = width() - nonDataWidth;

    //  // no width left for resizeable columns? TODO: put this in resizeEvent
    //  if( fullWidth < 0 )
    //    return;

    Okteta::PixelX availableWidth = maxDataWidth;

    // prepare needed values
    const Okteta::PixelX byteWidth = mByteArrayColumnRenderer->byteWidth();
    const Okteta::PixelX byteSpacingWidth = mByteArrayColumnRenderer->byteSpacingWidth();
    Okteta::PixelX groupSpacingWidth;
    int noOfGroupedBytes = mByteArrayColumnRenderer->noOfGroupedBytes();
    // no grouping?
    if (noOfGroupedBytes == 0) {
        // fake no grouping by grouping with 1 and using byteSpacingWidth
        noOfGroupedBytes = 1;
        groupSpacingWidth = byteSpacingWidth;
    } else {
        groupSpacingWidth = mByteArrayColumnRenderer->groupSpacingWidth();
    }

    const Okteta::PixelX byteGroupWidth =  noOfGroupedBytes * byteWidth + (noOfGroupedBytes - 1) * byteSpacingWidth;
    const Okteta::PixelX totalGroupWidth = byteGroupWidth + groupSpacingWidth;

    // calculate fitting groups per line
    const int fittingGroupsPerLine = (availableWidth + groupSpacingWidth) // fake spacing after last group
                                        / totalGroupWidth;

    // calculate the fitting bytes per line by groups
    int fittingBytesPerLine = noOfGroupedBytes * fittingGroupsPerLine;

    // not only full groups?
    if (noOfGroupedBytes > 1 && mResizeStyle == FullSizeLayoutStyle) {

        if (fittingGroupsPerLine > 0) {
            availableWidth -= fittingGroupsPerLine * totalGroupWidth; // includes additional spacing after last group

        }

        if (availableWidth > 0) {
            fittingBytesPerLine += (availableWidth + byteSpacingWidth) / (byteWidth + byteSpacingWidth);
        }

        // is there not even the space for a single byte?
        if (fittingBytesPerLine == 0) {
            // ensure at least one byte per line
            fittingBytesPerLine = 1;
            // and
        }
    }
    // is there not the space for a single group?
    else if (fittingBytesPerLine == 0) {
        // ensures at least one group
        fittingBytesPerLine = noOfGroupedBytes;
    }

    return fittingBytesPerLine;
}

void ByteArrayRowFrameRenderer::showByteArrayColumns(int newColumns)
{
    int columns = visibleByteArrayCodings();

    // no changes or no column selected?
    if (newColumns == columns || !(newColumns & (ValueCodingId | CharCodingId))) {
        return;
    }

    mByteArrayColumnRenderer->setVisibleCodings(newColumns);

    const int rowHeight = mByteArrayColumnRenderer->rowHeight();
    setLineHeight(rowHeight);

    adjustToWidth();
}
