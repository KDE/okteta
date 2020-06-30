/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charbytearraycolumntextrenderer.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/CharCodec>
#include <Okteta/Character>
// Qt
#include <QTextStream>

namespace Kasten {

CharByteArrayColumnTextRenderer::CharByteArrayColumnTextRenderer(
    const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
    const Okteta::CoordRange& coordRange,
    int noOfBytesPerLine, int byteSpacingWidth, int noOfGroupedBytes,
    const QString& charCodecName, QChar substituteChar, QChar undefinedChar)
    : AbstractByteArrayColumnTextRenderer(byteArrayModel, offset, coordRange, noOfBytesPerLine)
    , mCharCodec(Okteta::CharCodec::createCodec(charCodecName))
    , mSubstituteChar(substituteChar)
    , mUndefinedChar(undefinedChar)
{
    setWidths(1, byteSpacingWidth, noOfGroupedBytes);
}

CharByteArrayColumnTextRenderer::~CharByteArrayColumnTextRenderer()
{
    delete mCharCodec;
}

void CharByteArrayColumnTextRenderer::renderLine(QTextStream* stream, bool isSubline) const
{
    Q_UNUSED(isSubline)

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

    // draw individual chars
    uint e = 0;
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

    *stream << whiteSpace(mNoOfCharsPerLine - e);

    ++mRenderLine;
}

}
