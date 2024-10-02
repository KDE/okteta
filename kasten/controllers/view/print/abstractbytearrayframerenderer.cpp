/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008-2009, 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearrayframerenderer.hpp"

// lib
#include "printcolumnstylist.hpp"
// Okteta gui
#include <Okteta/ByteArrayTableLayout>
#include <Okteta/ByteArrayTableRanges>
// Okteta core
#include <Okteta/ValueCodec>
#include <Okteta/CharCodec>
// Qt
#include <QPainter>

static constexpr Okteta::Address DefaultStartOffset = 0;
static constexpr Okteta::Address DefaultFirstLineOffset = 0;
static constexpr int DefaultNoOfBytesPerLine =  16;
static constexpr LayoutStyle DefaultResizeStyle = FixedLayoutStyle; // krazy:exclude=staticobjects
static constexpr Okteta::ValueCoding DefaultValueCoding =  Okteta::HexadecimalCoding; // krazy:exclude=staticobjects
static constexpr Okteta::CharCoding DefaultCharCoding = Okteta::LocalEncoding; // krazy:exclude=staticobjects

static constexpr int BAFInitialHeight = 50;
static constexpr int BAFInitialWidth = 50;

AbstractByteArrayFrameRenderer::AbstractByteArrayFrameRenderer()
    : mHeight(BAFInitialHeight)
    , mWidth(BAFInitialWidth)
    , mResizeStyle(DefaultResizeStyle)
{
    mLayout = new Okteta::ByteArrayTableLayout(DefaultNoOfBytesPerLine, DefaultFirstLineOffset, DefaultStartOffset, 0, 0);
    mLayout->setNoOfLinesPerPage(noOfLinesPerFrame());
    mTableRanges = new Okteta::ByteArrayTableRanges(mLayout);

    // set codecs
    mValueCodec = Okteta::ValueCodec::createCodec((Okteta::ValueCoding)DefaultValueCoding);
    mValueCoding = DefaultValueCoding;
    mCharCodec = Okteta::CharCodec::createCodec((Okteta::CharCoding)DefaultCharCoding);
    mCharCoding = DefaultCharCoding;

    mStylist = new Okteta::PrintColumnStylist();
}

AbstractByteArrayFrameRenderer::~AbstractByteArrayFrameRenderer()
{
    delete mStylist;
    delete mTableRanges;
    delete mLayout;
    delete mValueCodec;
    delete mCharCodec;
}

Okteta::AbstractByteArrayModel* AbstractByteArrayFrameRenderer::byteArrayModel() const { return mByteArrayModel; }
Okteta::Address AbstractByteArrayFrameRenderer::offset()                         const { return mLayout->startOffset(); }
Okteta::Size AbstractByteArrayFrameRenderer::length()                            const { return mLayout->length(); }

int AbstractByteArrayFrameRenderer::noOfBytesPerLine()            const { return mLayout->noOfBytesPerLine(); }
Okteta::Address AbstractByteArrayFrameRenderer::firstLineOffset() const { return mLayout->firstLineOffset(); }
Okteta::Address AbstractByteArrayFrameRenderer::startOffset()     const { return mLayout->startOffset(); }
LayoutStyle AbstractByteArrayFrameRenderer::layoutStyle()         const { return mResizeStyle; }
Okteta::ValueCoding AbstractByteArrayFrameRenderer::valueCoding() const { return mValueCoding; }

const QString& AbstractByteArrayFrameRenderer::charCodingName()   const { return mCharCodec->name(); }
Okteta::CharCoding AbstractByteArrayFrameRenderer::charCoding()   const { return mCharCoding; }

int AbstractByteArrayFrameRenderer::height() const { return mHeight; }
int AbstractByteArrayFrameRenderer::width()  const { return mWidth; }

void AbstractByteArrayFrameRenderer::prepare()
{
}

void AbstractByteArrayFrameRenderer::renderFrame(QPainter* painter, int frameIndex)
{
    painter->setFont(mFont);
    AbstractColumnFrameRenderer::renderFrame(painter, frameIndex);
}

int AbstractByteArrayFrameRenderer::framesCount() const
{
    const int charsPerFrame = mLayout->noOfBytesPerLine() * noOfLinesPerFrame();

    // clever calculation works: at least one page for the rest
    // hard to describe, think yourself
    // TODO: needs to include the offset in the first line
    const int frames = ((mLayout->length() - 1) / charsPerFrame) + 1;

    return frames;
}

void AbstractByteArrayFrameRenderer::setHeight(int height) { mHeight = height; }
void AbstractByteArrayFrameRenderer::setWidth(int width)
{
    if (mWidth == width) {
        return;
    }

    mWidth = width;

    adjustToWidth();
}

void AbstractByteArrayFrameRenderer::setFirstLineOffset(Okteta::Address firstLineOffset)
{
    if (!mLayout->setFirstLineOffset(firstLineOffset)) {
        return;
    }

    // affects:
    // the no of lines -> width
    adjustLayoutToSize();
}

void AbstractByteArrayFrameRenderer::setStartOffset(Okteta::Address startOffset)
{
    if (!mLayout->setStartOffset(startOffset)) {
        return;
    }

    // affects:
    // the no of lines -> width
    adjustLayoutToSize();
}

void AbstractByteArrayFrameRenderer::setLayoutStyle(LayoutStyle style)
{
    if (mResizeStyle == style) {
        return;
    }

    mResizeStyle = style;

    adjustToWidth();
}

void AbstractByteArrayFrameRenderer::setNoOfBytesPerLine(int noOfBytesPerLine)
{
    // if the number is explicitly set we expect a wish for no automatic resize
    mResizeStyle = FixedLayoutStyle;

    if (!mLayout->setNoOfBytesPerLine(noOfBytesPerLine)) {
        return;
    }
    adjustToWidth();
}

void AbstractByteArrayFrameRenderer::adjustToWidth()
{
    adjustToLayoutNoOfBytesPerLine();
    adjustLayoutToSize();
}

void AbstractByteArrayFrameRenderer::adjustLayoutToSize()
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
