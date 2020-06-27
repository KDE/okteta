/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrowscolumntextrenderer.hpp"

// Okteta core
#include <Okteta/ValueCodec>
#include <Okteta/CharCodec>
#include <Okteta/Character>
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QTextStream>

namespace Kasten {

ByteArrayRowsColumnTextRenderer::ByteArrayRowsColumnTextRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                                 Okteta::Address offset,
                                                                 const Okteta::CoordRange& coordRange,
                                                                 int noOfBytesPerLine, int byteSpacingWidth, int noOfGroupedBytes,
                                                                 int visibleCodings,
                                                                 Okteta::ValueCoding valueCoding,
                                                                 const QString& charCodecName, QChar substituteChar, QChar undefinedChar)
    : mByteArrayModel(byteArrayModel)
    , mCoordRange(coordRange)
    , mNoOfBytesPerLine(noOfBytesPerLine)
    , mVisibleCodings(visibleCodings)
    , mValueCodec(Okteta::ValueCodec::createCodec(valueCoding))
    , mCharCodec(Okteta::CharCodec::createCodec(charCodecName))
    , mSubstituteChar(substituteChar)
    , mUndefinedChar(undefinedChar)
    , mOffset(offset)
    , mLinePositions(new int[mNoOfBytesPerLine])
{
    const int encodingWidth = (mVisibleCodings & 1) ? mValueCodec->encodingWidth() : 1;
    setWidths(encodingWidth, byteSpacingWidth, noOfGroupedBytes);
}

ByteArrayRowsColumnTextRenderer::~ByteArrayRowsColumnTextRenderer()
{
    delete [] mLinePositions;
    delete mCharCodec;
    delete mValueCodec;
}

int ByteArrayRowsColumnTextRenderer::noOfSublinesNeeded() const
{
    return (mVisibleCodings > 2) ? 2 : 1;
}

void ByteArrayRowsColumnTextRenderer::setWidths(int byteWidth, int byteSpacingWidth, int noOfGroupedBytes)
{
    // TODO: remove this hack and make it more general
    if (byteSpacingWidth > 0) {
        byteSpacingWidth = DefaultTRByteSpacingWidth;
    }

    int spacingTrigger = noOfGroupedBytes - 1;
    if (spacingTrigger < 0) {
        spacingTrigger = mNoOfBytesPerLine; // ensures to never trigger the group spacing

    }
    int N = 0;
    int p = 0;
    int gs = 0;
    int* P = mLinePositions;
    for (; P < &mLinePositions[mNoOfBytesPerLine]; ++P, ++p, ++gs) {
        *P = N;
        N += byteWidth;

        // is there a space behind the actual byte (if it is not the last)?
        if (gs == spacingTrigger) {
            N += TRGroupSpacingWidth;
            gs = -1;
        } else {
            N += byteSpacingWidth;
        }
    }

    N -= (gs == 0) ? TRGroupSpacingWidth : byteSpacingWidth;

    mNoOfCharsPerLine = N;
}

void ByteArrayRowsColumnTextRenderer::renderFirstLine(QTextStream* stream, int lineIndex) const
{
    mRenderLine = lineIndex;
    renderLine(stream, false);
}

void ByteArrayRowsColumnTextRenderer::renderNextLine(QTextStream* stream, bool isSubline) const
{
    renderLine(stream, isSubline);
}

void ByteArrayRowsColumnTextRenderer::renderLine(QTextStream* stream, bool isSubline) const
{
    const int lineOffset = mOffset;

    int p = 0;
    int pEnd = mNoOfBytesPerLine;
    // correct boundaries
    if (mRenderLine == mCoordRange.start().line()) {
        p = mCoordRange.start().pos();
    }
    if (mRenderLine == mCoordRange.end().line()) {
        pEnd = mCoordRange.end().pos() + 1;
    }

    uint e = 0;
    if (isSubline || mVisibleCodings == 2) {
        const QChar tabChar = QLatin1Char('\t');
        const QChar returnChar = QLatin1Char('\n');

        // draw individual chars
        for (; p < pEnd; ++p, ++mOffset) {
            // get next position
            const uint t = mLinePositions[p];
            // clear spacing
            *stream << whiteSpace(t - e);

            // print char
            const Okteta::Character byteChar = mCharCodec->decode(mByteArrayModel->byte(mOffset));

            const QChar streamChar = byteChar.isUndefined() ?      Okteta::Character(mUndefinedChar) :
                                     (!byteChar.isPrint()
                                      || byteChar == tabChar
                                      || byteChar == returnChar) ? Okteta::Character(mSubstituteChar) :
                                                                   byteChar;
            *stream << streamChar;

            e = t + 1;
        }
    } else {
        QString E;
        E.resize(mValueCodec->encodingWidth());
        // draw individual chars
        for (; p < pEnd; ++p, ++mOffset) {
            // get next position
            const uint t = mLinePositions[p];
            // clear spacing
            *stream << whiteSpace(t - e);
            mValueCodec->encode(&E, 0, mByteArrayModel->byte(mOffset));
            *stream << E;
            e = t + mValueCodec->encodingWidth();
        }
    }

    *stream << whiteSpace(mNoOfCharsPerLine - e);

    if (isSubline || mVisibleCodings < 3) {
        ++mRenderLine;
    } else {
        mOffset = lineOffset;
    }
}

}
