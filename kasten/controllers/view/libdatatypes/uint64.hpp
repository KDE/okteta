/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_UINT64_HPP
#define OKTETA_UINT64_HPP

// Qt
#include <QLocale>
#include <QMetaType>
#include <QString>

namespace Okteta {

struct UInt64
{
public:
    UInt64(quint64 v);
    UInt64(const UInt64&) = default;
    UInt64(UInt64&&) = default;
    UInt64();

    ~UInt64() = default;

    UInt64& operator=(const UInt64&) = default;
    UInt64& operator=(UInt64&&) = default;

public:
    [[nodiscard]]
    QString toString(bool asHex) const;
    [[nodiscard]]
    QString toString(bool asHex, const QLocale& locale) const;

public:
    quint64 value = 0;
};

inline UInt64::UInt64() = default;
inline UInt64::UInt64(quint64 v) : value(v) {}

inline QString UInt64::toString(bool asHex) const
{
    return asHex ? QStringLiteral("0x%1").arg(value, 16, 16, QChar::fromLatin1('0')) :
                   QString::number(value);
}

inline QString UInt64::toString(bool asHex, const QLocale& locale) const
{
    return asHex ? QStringLiteral("0x%1").arg(value, 16, 16, QChar::fromLatin1('0')) :
                   locale.toString(value);
}

}

Q_DECLARE_METATYPE(Okteta::UInt64)

#endif
