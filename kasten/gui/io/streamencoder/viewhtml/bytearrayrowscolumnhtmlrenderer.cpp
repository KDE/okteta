/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrowscolumnhtmlrenderer.hpp"

// Okteta core
#include <Okteta/ValueCodec>
#include <Okteta/CharCodec>
#include <Okteta/Character>
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QTextStream>

namespace Kasten {

ByteArrayRowsColumnHtmlRenderer::ByteArrayRowsColumnHtmlRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                                 Okteta::Address offset,
                                                                 const Okteta::CoordRange& coordRange,
                                                                 int noOfBytesPerLine, int noOfGroupedBytes,
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
{
    Q_UNUSED(noOfGroupedBytes)
}

ByteArrayRowsColumnHtmlRenderer::~ByteArrayRowsColumnHtmlRenderer() = default;

int ByteArrayRowsColumnHtmlRenderer::noOfSublinesNeeded() const
{
    return (mVisibleCodings > 2) ? 2 : 1;
}

void ByteArrayRowsColumnHtmlRenderer::renderFirstLine(QTextStream* stream, int lineIndex) const
{
    mRenderLine = lineIndex;
    renderLine(stream, false);
}

void ByteArrayRowsColumnHtmlRenderer::renderNextLine(QTextStream* stream, bool isSubline) const
{
    renderLine(stream, isSubline);
}

void ByteArrayRowsColumnHtmlRenderer::renderLine(QTextStream* stream, bool isSubline) const
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
    const bool needsFilledEmptyCells =
        (mCoordRange.lines() == 1) ? ((0 < p) || (pEnd < mNoOfBytesPerLine)) :
        (mCoordRange.lines() == 2) ? (mCoordRange.end().pos() + 1 < mCoordRange.start().pos() ) :
        /* eefault */                false;
    const unsigned int codingWidth = needsFilledEmptyCells ? mValueCodec->encodingWidth() : 0;

    *stream << emptyCells(p, codingWidth);

    if (isSubline || mVisibleCodings == 2) {
        const QChar tabChar = QLatin1Char('\t');
        const QChar returnChar = QLatin1Char('\n');

        // render individual chars
        for (; p < pEnd; ++p, ++mOffset) {
            const Okteta::Character byteChar = mCharCodec->decode(mByteArrayModel->byte(mOffset));

            const QChar streamChar = byteChar.isUndefined() ?      Okteta::Character(mUndefinedChar) :
                                     (!byteChar.isPrint()
                                      || byteChar == tabChar
                                      || byteChar == returnChar) ? Okteta::Character(mSubstituteChar) :
                                                                   byteChar;
            *stream << "<td><tt>" << htmlEscaped(streamChar) << "</tt></td>";
        }
    } else {
        QString E;
        E.resize(mValueCodec->encodingWidth());
        // render individual values
        for (; p < pEnd; ++p, ++mOffset) {
            mValueCodec->encode(&E, 0, mByteArrayModel->byte(mOffset));
            *stream << "<td><tt>" << E.toHtmlEscaped() << "</tt></td>";
        }
    }

    *stream << emptyCells(mNoOfBytesPerLine - p, codingWidth);

    if (isSubline || mVisibleCodings < 3) {
        ++mRenderLine;
    } else {
        mOffset = lineOffset;
    }
}

}
