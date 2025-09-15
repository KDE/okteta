/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charbytearraycolumnhtmlrenderer.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/CharCodec>
#include <Okteta/Character>
// Qt
#include <QTextStream>

namespace Kasten {

CharByteArrayColumnHtmlRenderer::CharByteArrayColumnHtmlRenderer(
    const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
    const Okteta::CoordRange& coordRange,
    int noOfBytesPerLine,
    const QString& charCodecName, QChar substituteChar, QChar undefinedChar)
    : AbstractByteArrayColumnHtmlRenderer(byteArrayModel, offset, coordRange, noOfBytesPerLine)
    , mCharCodec(Okteta::CharCodec::createCodec(charCodecName))
    , mSubstituteChar(substituteChar)
    , mUndefinedChar(undefinedChar)
{
}

CharByteArrayColumnHtmlRenderer::~CharByteArrayColumnHtmlRenderer() = default;

void CharByteArrayColumnHtmlRenderer::renderLine(QTextStream* stream, bool isSubline) const
{
    Q_UNUSED(isSubline)

    *stream << "<td nowrap><tt>";

    int p = 0;
    int pEnd = mNoOfBytesPerLine;
    // correct boundaries
    if (mRenderLine == mCoordRange.start().line()) {
        p = mCoordRange.start().pos();
    }
    if (mRenderLine == mCoordRange.end().line()) {
        pEnd = mCoordRange.end().pos() + 1;
    }

    const QChar tabChar = QLatin1Char('\t');
    const QChar returnChar = QLatin1Char('\n');

    *stream << whiteSpace(p);

    // draw individual chars
    for (; p < pEnd; ++p, ++mOffset) {
        const Okteta::Character byteChar = mCharCodec->decode(mByteArrayModel->byte(mOffset));

        const QChar streamChar = byteChar.isUndefined() ?      Okteta::Character(mUndefinedChar) :
                                 (!byteChar.isPrint()
                                  || byteChar == tabChar
                                  || byteChar == returnChar) ? Okteta::Character(mSubstituteChar) :
                                                               byteChar;
        *stream << htmlEscaped(streamChar);
    }

    *stream << whiteSpace(mNoOfBytesPerLine - p);

    ++mRenderLine;

    *stream << "</tt></td>";
}

}
