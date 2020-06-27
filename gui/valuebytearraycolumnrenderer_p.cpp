/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "valuebytearraycolumnrenderer_p.hpp"

// lib
#include "helper.hpp"
// lib
#include <abstractcolumnstylist.hpp>
// Okteta core
#include <Okteta/ValueCodec>
#include <Okteta/CharCodec>
// KF
#include <KColorScheme>
// Qt
#include <QPainter>


namespace Okteta {

void ValueByteArrayColumnRendererPrivate::setValueCodec(ValueCoding valueCoding, const ValueCodec* valueCodec)
{
    mValueCoding = valueCoding;
    mValueCodec = valueCodec;
    mDecodedByteText.resize(mValueCodec->encodingWidth());

    // recalculate depend sizes
    recalcByteWidth();

    if (mLinePosLeftPixelX) {
        recalcX();
    }
}

bool ValueByteArrayColumnRendererPrivate::setBinaryGapWidth(PixelX binaryGapWidth)
{
    // no changes?
    if (mBinaryGapWidth == binaryGapWidth) {
        return false;
    }

    mBinaryGapWidth = binaryGapWidth;

    // recalculate depend sizes
    recalcByteWidth();

    if (mLinePosLeftPixelX) {
        recalcX();
    }
    return true;
}

void ValueByteArrayColumnRendererPrivate::recalcByteWidth()
{
    // use 0 as reference, using a fixed font should always yield same width
    mValueCodec->encode(&mDecodedByteText, 0, Byte(0));
    if (mValueCoding == BinaryCoding) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
        const int binaryHalfWidth = mFontMetrics.horizontalAdvance(mDecodedByteText.left(4));
#else
        const int binaryHalfWidth = mFontMetrics.width(mDecodedByteText.left(4));
#endif
        mBinaryHalfOffset = binaryHalfWidth + mBinaryGapWidth;
        setByteWidth(mBinaryHalfOffset + binaryHalfWidth);
    } else {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
        setByteWidth(mFontMetrics.horizontalAdvance(mDecodedByteText));
#else
        setByteWidth(mFontMetrics.width(mDecodedByteText));
#endif
    }
}

// perhaps sometimes there will be a grammar
void ValueByteArrayColumnRendererPrivate::renderEditedByte(QPainter* painter, Byte byte, const QString& editBuffer)
{
    Q_Q(ValueByteArrayColumnRenderer);

    const Character byteChar = mCharCodec->decode(byte);

    const QPalette& palette = mStylist->palette();
    KColorScheme colorScheme(palette.currentColorGroup(), KColorScheme::View);
    const KColorScheme::ForegroundRole foregroundRole =
        mByteTypeColored ? foregroundRoleForChar(byteChar) : KColorScheme::NormalText;
    const QBrush brush = colorScheme.foreground(foregroundRole);
    painter->fillRect(0, 0, byteWidth(), q->lineHeight(), brush);

    const QBrush backgroundBrush = colorScheme.background();
    const QColor& charColor = backgroundBrush.color();
    renderCode(painter, editBuffer, charColor);
}

void ValueByteArrayColumnRendererPrivate::renderByteText(QPainter* painter,
                                                         Byte byte, Character byteChar, const QColor& color) const
{
    Q_UNUSED(byteChar)

    mValueCodec->encode(&mDecodedByteText, 0, byte);
    renderCode(painter, mDecodedByteText, color);
}

void ValueByteArrayColumnRendererPrivate::renderCode(QPainter* painter, const QString& code, const QColor& color) const
{
    painter->setPen(color);
    if (mValueCoding == Okteta::BinaryCoding) {
        // leave a gap in the middle
        painter->drawText(0, mDigitBaseLine, code.left(4));
        painter->drawText(mBinaryHalfOffset, mDigitBaseLine, code.right(4));
    } else {
        painter->drawText(0, mDigitBaseLine, code);
    }
}

}
