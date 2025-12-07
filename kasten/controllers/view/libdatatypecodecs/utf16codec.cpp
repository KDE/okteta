/*
    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "utf16codec.hpp"

// tool
#include "poddata.hpp"
// libdatatypes
#include <utf16.hpp>
// KF
#include <KLocalizedString>

namespace Okteta {

Utf16Codec::Utf16Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "UTF-16"))
    , mUtf16Decoder(QStringConverter::Utf16, QStringConverter::Flag::ConvertInitialBom)
    , mUtf16Encoder(QStringConverter::Utf16)
{
}

Utf16Codec::~Utf16Codec() = default;

QVariant Utf16Codec::value(const PODData& data, int* byteCount) const
{
    const char* const pointer = (char*)data.pointer(2);

    if (pointer) {
        mUtf16Decoder.resetState();
        const QString utf16 = mUtf16Decoder.decode({pointer, 2});
        if (utf16.size() == 1 && utf16[0] != QChar(QChar::ReplacementCharacter)) {
            *byteCount = 2;
            return QVariant::fromValue<Utf16>(Utf16(utf16[0]));
        }
    }
    *byteCount = 0;
    return {};
}

QByteArray Utf16Codec::valueToBytes(const QVariant& value) const
{
    const QChar valueChar = value.value<Utf16>().value;

    mUtf16Encoder.resetState();
    return mUtf16Encoder.encode(valueChar);
}

bool Utf16Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<Utf16>().value == otherValue.value<Utf16>().value);
}

}
