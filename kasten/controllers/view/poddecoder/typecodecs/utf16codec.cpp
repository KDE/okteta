/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "utf16codec.hpp"

// tool
#include "../poddata.hpp"
// libdatatypes
#include <utf16.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QTextCodec>
#include <QTextEncoder>
#include <QTextDecoder>

namespace Okteta {

Utf16Codec::Utf16Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "UTF-16"))
{
    auto* utf16Codec = QTextCodec::codecForName("UTF-16");
    mUtf16Encoder.reset(utf16Codec->makeEncoder(QTextCodec::IgnoreHeader));
    mUtf16Decoder.reset(utf16Codec->makeDecoder(QTextCodec::IgnoreHeader));
}

Utf16Codec::~Utf16Codec() = default;

QVariant Utf16Codec::value(const PODData& data, int* byteCount) const
{
    const char* pointer = (char*)data.pointer(2);

    if (pointer) {
        const QString utf16 = mUtf16Decoder->toUnicode(pointer, 2);
        if (utf16.size() == 1 && utf16[0] != QChar(QChar::ReplacementCharacter)) {
            *byteCount = 2;
            return QVariant::fromValue<Utf16>(Utf16(utf16[0]));
        }
    }
    *byteCount = 0;
    return QVariant();
}

QByteArray Utf16Codec::valueToBytes(const QVariant& value) const
{
    const QChar valueChar = value.value<Utf16>().value;

    return mUtf16Encoder->fromUnicode(valueChar);
}

bool Utf16Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<Utf16>().value == otherValue.value<Utf16>().value);
}

}
