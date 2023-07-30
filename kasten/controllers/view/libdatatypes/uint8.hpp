/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_UINT8_HPP
#define OKTETA_UINT8_HPP

// Qt
#include <QLocale>
#include <QMetaType>
#include <QString>

namespace Okteta {

struct UInt8
{
public:
    UInt8(quint8 v);
    UInt8(const UInt8&) = default;
    UInt8();

    ~UInt8() = default;

    UInt8& operator=(const UInt8&) = default;

public:
    QString toString(bool asHex) const;
    QString toString(bool asHex, const QLocale& locale) const;

public:
    quint8 value = 0;
};

inline UInt8::UInt8() = default;
inline UInt8::UInt8(quint8 v) : value(v) {}

inline QString UInt8::toString(bool asHex) const
{
    return asHex ? QStringLiteral("0x%1").arg(value, 2, 16, QChar::fromLatin1('0')) :
                   QString::number(value);
}

inline QString UInt8::toString(bool asHex, const QLocale& locale) const
{
    return asHex ? QStringLiteral("0x%1").arg(value, 2, 16, QChar::fromLatin1('0')) :
                   locale.toString(value);
}

}

Q_DECLARE_METATYPE(Okteta::UInt8)

#endif
