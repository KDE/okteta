/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayframerenderer.hpp"

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
#include <QHash>
#include <QDateTime>
#include <QPainter>
#include <QFontMetrics>
#include <QFontDatabase>
#include <QApplication>

static constexpr Okteta::Address DefaultStartOffset = 0;
static constexpr Okteta::Address DefaultFirstLineOffset = 0;
static constexpr int DefaultNoOfBytesPerLine =  16;
static constexpr LayoutStyle DefaultResizeStyle = FixedLayoutStyle; // krazy:exclude=staticobjects
static constexpr Okteta::OffsetFormat::Format DefaultOffsetCoding =  Okteta::OffsetFormat::Hexadecimal; // krazy:exclude=staticobjects
static constexpr Okteta::ValueCoding DefaultValueCoding =  Okteta::HexadecimalCoding; // krazy:exclude=staticobjects
static inline QString DefaultCharCoding() { return QString(); } // -> local 8-bit

static constexpr int BAFInitialHeight = 50;
static constexpr int BAFInitialWidth = 50;

ByteArrayFrameRenderer::ByteArrayFrameRenderer()
    : mHeight(BAFInitialHeight)
    , mWidth(BAFInitialWidth)
    , mLayout(new Okteta::ByteArrayTableLayout(DefaultNoOfBytesPerLine, DefaultFirstLineOffset, DefaultStartOffset, 0, 0))
    , mTableRanges(new Okteta::ByteArrayTableRanges(mLayout.get()))
    , mStylist(new Okteta::PrintColumnStylist())
    , mResizeStyle(DefaultResizeStyle)
{
    mLayout->setNoOfLinesPerPage(noOfLinesPerFrame());

    // set codecs
    mValueCodec.reset(Okteta::ValueCodec::createCodec((Okteta::ValueCoding)DefaultValueCoding));
    mValueCoding = DefaultValueCoding;
    mCharCodec.reset(Okteta::CharCodec::createCodec(DefaultCharCoding()));

    // creating the columns in the needed order
    mOffsetColumnRenderer =
        new Okteta::OffsetColumnRenderer(mStylist.get(), mLayout.get(), DefaultOffsetCoding);
    mFirstBorderColumnRenderer =
        new Okteta::BorderColumnRenderer(mStylist.get(), true, false);
    mValueColumnRenderer =
        new Okteta::ValueByteArrayColumnRenderer(mStylist.get(), mByteArrayModel, mLayout.get(), mTableRanges.get());
    mSecondBorderColumnRenderer =
        new Okteta::BorderColumnRenderer(mStylist.get(), true, false);
    mCharColumnRenderer =
        new Okteta::CharByteArrayColumnRenderer(mStylist.get(), mByteArrayModel, mLayout.get(), mTableRanges.get());

    addColumn(mOffsetColumnRenderer);
    addColumn(mFirstBorderColumnRenderer);
    addColumn(mValueColumnRenderer);
    addColumn(mSecondBorderColumnRenderer);
    addColumn(mCharColumnRenderer);

    mValueColumnRenderer->setValueCodec((Okteta::ValueCoding)mValueCoding, mValueCodec.get());
    mValueColumnRenderer->setCharCodec(mCharCodec.get());
    mCharColumnRenderer->setCharCodec(mCharCodec.get());

    setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

ByteArrayFrameRenderer::~ByteArrayFrameRenderer() = default;

Okteta::AbstractByteArrayModel* ByteArrayFrameRenderer::byteArrayModel() const { return mByteArrayModel; }
Okteta::Address ByteArrayFrameRenderer::offset()                         const { return mLayout->startOffset(); }
Okteta::Size ByteArrayFrameRenderer::length()                            const { return mLayout->length(); }

int ByteArrayFrameRenderer::noOfBytesPerLine()               const { return mLayout->noOfBytesPerLine(); }
Okteta::Address ByteArrayFrameRenderer::firstLineOffset()    const { return mLayout->firstLineOffset(); }
Okteta::Address ByteArrayFrameRenderer::startOffset()        const { return mLayout->startOffset(); }
LayoutStyle ByteArrayFrameRenderer::layoutStyle()            const { return mResizeStyle; }
Okteta::ValueCoding ByteArrayFrameRenderer::valueCoding()   const { return mValueCoding; }
Okteta::PixelX ByteArrayFrameRenderer::byteSpacingWidth()           const { return mValueColumnRenderer->byteSpacingWidth(); }
int ByteArrayFrameRenderer::noOfGroupedBytes()               const { return mValueColumnRenderer->noOfGroupedBytes(); }
Okteta::PixelX ByteArrayFrameRenderer::groupSpacingWidth()          const { return mValueColumnRenderer->groupSpacingWidth(); }
Okteta::PixelX ByteArrayFrameRenderer::binaryGapWidth()             const { return mValueColumnRenderer->binaryGapWidth(); }
bool ByteArrayFrameRenderer::showsNonprinting()              const { return mCharColumnRenderer->isShowingNonprinting(); }
QChar ByteArrayFrameRenderer::substituteChar()               const { return mCharColumnRenderer->substituteChar(); }
QChar ByteArrayFrameRenderer::undefinedChar()                const { return mCharColumnRenderer->undefinedChar(); }
QString ByteArrayFrameRenderer::charCodingName()             const { return mCharCodec->name(); }

bool ByteArrayFrameRenderer::offsetColumnVisible() const { return mOffsetColumnRenderer->isVisible(); }
Okteta::OffsetFormat::Format ByteArrayFrameRenderer::offsetCoding() const { return mOffsetColumnRenderer->format(); }

int ByteArrayFrameRenderer::visibleByteArrayCodings() const
{ return (mValueColumnRenderer->isVisible() ? ValueCodingId : 0) | (mCharColumnRenderer->isVisible() ? CharCodingId : 0); }

int ByteArrayFrameRenderer::height() const { return mHeight; }
int ByteArrayFrameRenderer::width() const { return mWidth; }

int ByteArrayFrameRenderer::framesCount() const
{
    const int charsPerFrame = mLayout->noOfBytesPerLine() * noOfLinesPerFrame();

    // clever calculation works: at least one page for the rest
    // hard to describe, think yourself
    // TODO: needs to include the offset in the first line
    const int frames = ((mLayout->length() - 1) / charsPerFrame) + 1;

    return frames;
}

void ByteArrayFrameRenderer::setByteArrayModel(Okteta::AbstractByteArrayModel* byteArrayModel,
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

void ByteArrayFrameRenderer::setHeight(int height) { mHeight = height; }
void ByteArrayFrameRenderer::setWidth(int width)
{
    if (mWidth == width) {
        return;
    }

    mWidth = width;

    adjustToWidth();
}

void ByteArrayFrameRenderer::setFirstLineOffset(Okteta::Address firstLineOffset)
{
    if (!mLayout->setFirstLineOffset(firstLineOffset)) {
        return;
    }

    // affects:
    // the no of lines -> width
    adjustLayoutToSize();
}

void ByteArrayFrameRenderer::setStartOffset(Okteta::Address startOffset)
{
    if (!mLayout->setStartOffset(startOffset)) {
        return;
    }

    // affects:
    // the no of lines -> width
    adjustLayoutToSize();
}

void ByteArrayFrameRenderer::setBufferSpacing(Okteta::PixelX byteSpacing, int noOfGroupedBytes, Okteta::PixelX groupSpacing)
{
    if (!mValueColumnRenderer->setSpacing(byteSpacing, noOfGroupedBytes, groupSpacing)) {
        return;
    }

    adjustToWidth();
}

void ByteArrayFrameRenderer::setLayoutStyle(LayoutStyle style)
{
    if (mResizeStyle == style) {
        return;
    }

    mResizeStyle = style;

    adjustToWidth();
}

void ByteArrayFrameRenderer::setNoOfBytesPerLine(int noOfBytesPerLine)
{
    // if the number is explicitly set we expect a wish for no automatic resize
    mResizeStyle = FixedLayoutStyle;

    if (!mLayout->setNoOfBytesPerLine(noOfBytesPerLine)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayFrameRenderer::setByteSpacingWidth(Okteta::PixelX byteSpacingWidth)
{
    if (!mValueColumnRenderer->setByteSpacingWidth(byteSpacingWidth)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayFrameRenderer::setNoOfGroupedBytes(int noOfGroupedBytes)
{
    if (!mValueColumnRenderer->setNoOfGroupedBytes(noOfGroupedBytes)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayFrameRenderer::setGroupSpacingWidth(Okteta::PixelX groupSpacingWidth)
{
    if (!mValueColumnRenderer->setGroupSpacingWidth(groupSpacingWidth)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayFrameRenderer::setBinaryGapWidth(Okteta::PixelX binaryGapWidth)
{
    if (!mValueColumnRenderer->setBinaryGapWidth(binaryGapWidth)) {
        return;
    }
    adjustToWidth();
}

void ByteArrayFrameRenderer::setSubstituteChar(QChar substituteChar)
{
    mCharColumnRenderer->setSubstituteChar(substituteChar);
}

void ByteArrayFrameRenderer::setUndefinedChar(QChar undefinedChar)
{
    mCharColumnRenderer->setUndefinedChar(undefinedChar);
}

void ByteArrayFrameRenderer::setShowsNonprinting(bool showsNonprinting)
{
    mCharColumnRenderer->setShowingNonprinting(showsNonprinting);
}

void ByteArrayFrameRenderer::setValueCoding(Okteta::ValueCoding valueCoding)
{
    if (mValueCoding == valueCoding) {
        return;
    }

    const uint oldCodingWidth = mValueCodec->encodingWidth();

    const Okteta::ValueCodec* newValueCodec =
        Okteta::ValueCodec::createCodec(valueCoding);
    if (!newValueCodec) {
        return;
    }

    mValueCodec.reset(newValueCodec);
    mValueCoding = valueCoding;

    mValueColumnRenderer->setValueCodec((Okteta::ValueCoding)mValueCoding, mValueCodec.get());

    const uint newCodingWidth = mValueCodec->encodingWidth();

    // change in the width?
    if (newCodingWidth != oldCodingWidth) {
        adjustToWidth();
    }
}

void ByteArrayFrameRenderer::setCharCoding(const QString& newCharCodingName)
{
    if (charCodingName() == newCharCodingName) {
        return;
    }

    const Okteta::CharCodec* newCharCodec = Okteta::CharCodec::createCodec(newCharCodingName);
    if (!newCharCodec) {
        return;
    }

    mCharCodec.reset(newCharCodec);

    mValueColumnRenderer->setCharCodec(mCharCodec.get());
    mCharColumnRenderer->setCharCodec(mCharCodec.get());
}

void ByteArrayFrameRenderer::setFont(const QFont& font)
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

void ByteArrayFrameRenderer::prepare()
{
}

void ByteArrayFrameRenderer::renderFrame(QPainter* painter, int frameIndex)
{
    painter->setFont(mFont);
    AbstractColumnFrameRenderer::renderFrame(painter, frameIndex);
}

void ByteArrayFrameRenderer::adjustToWidth()
{
    adjustToLayoutNoOfBytesPerLine();
    adjustLayoutToSize();
}

void ByteArrayFrameRenderer::adjustLayoutToSize()
{
    // check whether there is a change with the numbers of fitting bytes per line
    if (mResizeStyle != FixedLayoutStyle) {
        const int bytesPerLine = fittingBytesPerLine();

        // changes?
        if (mLayout->setNoOfBytesPerLine(bytesPerLine)) {
            adjustToLayoutNoOfBytesPerLine();
        }
    }

    setNoOfLines(mLayout->noOfLines());
}

void ByteArrayFrameRenderer::adjustToLayoutNoOfBytesPerLine()
{
    mValueColumnRenderer->resetXBuffer();
    mCharColumnRenderer->resetXBuffer();

    updateWidths();
}

void ByteArrayFrameRenderer::showOffsetColumn(bool visible)
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

void ByteArrayFrameRenderer::setOffsetCoding(Okteta::OffsetFormat::Format offsetCoding)
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
QSize ByteArrayFrameRenderer::sizeHint() const
{
    return QSize(columnsWidth(), columnsHeight());
}

QSize ByteArrayFrameRenderer::minimumSizeHint() const
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

int ByteArrayFrameRenderer::fittingBytesPerLine() const
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

void ByteArrayFrameRenderer::showByteArrayColumns(int newColumns)
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
