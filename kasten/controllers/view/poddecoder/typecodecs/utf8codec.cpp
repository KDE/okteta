/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "utf8codec.hpp"

// tool
#include "../types/utf8.hpp"
#include "../poddata.hpp"
// KF
#include <KLocalizedString>
// Qt
#include <QTextCodec>

namespace Okteta {

Utf8Codec::Utf8Codec()
    : AbstractTypeCodec(i18nc("@label:textbox", "UTF-8"))
    , mUtf8Codec(QTextCodec::codecForName("UTF-8"))
{}

Utf8Codec::~Utf8Codec() = default;

QVariant Utf8Codec::value(const PODData& data, int* byteCount) const
{
    // UTF-8
    // interpreted as a sequence of bytes, there is no endian problem
    // source: https://unicode.org/faq/utf_bom.html#3
    const QChar replacementChar(QChar::ReplacementCharacter);
    const char* originalData = (const char*)data.originalData();
    const int maxUtf8DataSize = data.size();

    QString utf8;
    bool isUtf8 = false;
    *byteCount = 0;
    for (int i = 1; i <= maxUtf8DataSize; ++i) {
        utf8 = mUtf8Codec->toUnicode(originalData, i);
        if (utf8.size() == 1 && utf8[0] != replacementChar) {
            isUtf8 = true;
            *byteCount = i;
            break;
        }
    }

    return isUtf8 ? QVariant::fromValue<Utf8>(Utf8(utf8[0])) : QVariant();
}

QByteArray Utf8Codec::valueToBytes(const QVariant& value) const
{
    const QChar valueChar = value.value<Utf8>().value;

    return mUtf8Codec->fromUnicode(valueChar);
}

bool Utf8Codec::areEqual(const QVariant& value, QVariant& otherValue) const
{
    return (value.value<Utf8>().value == otherValue.value<Utf8>().value);
}

}
