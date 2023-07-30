/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_UINT32_HPP
#define OKTETA_UINT32_HPP

// Qt
#include <QLocale>
#include <QMetaType>
#include <QString>

namespace Okteta {

struct UInt32
{
public:
    UInt32(quint32 v);
    UInt32(const UInt32&) = default;
    UInt32();

    ~UInt32() = default;

    UInt32& operator=(const UInt32&) = default;

public:
    QString toString(bool asHex) const;
    QString toString(bool asHex, const QLocale& locale) const;

public:
    quint32 value = 0;
};

inline UInt32::UInt32() = default;
inline UInt32::UInt32(quint32 v) : value(v) {}

inline QString UInt32::toString(bool asHex) const
{
    return asHex ? QStringLiteral("0x%1").arg(value, 8, 16, QChar::fromLatin1('0')) :
                   QString::number(value);
}

inline QString UInt32::toString(bool asHex, const QLocale& locale) const
{
    return asHex ? QStringLiteral("0x%1").arg(value, 8, 16, QChar::fromLatin1('0')) :
                   locale.toString(value);
}

}

Q_DECLARE_METATYPE(Okteta::UInt32)

#endif
