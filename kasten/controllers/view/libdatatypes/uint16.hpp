/*
    SPDX-FileCopyrightText: 2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_UINT16_HPP
#define OKTETA_UINT16_HPP

// Qt
#include <QLocale>
#include <QMetaType>
#include <QString>

namespace Okteta {

struct UInt16
{
public:
    UInt16(quint16 v);
    UInt16(const UInt16&) = default;
    UInt16(UInt16&&) = default;
    UInt16();

    ~UInt16() = default;

    UInt16& operator=(const UInt16&) = default;
    UInt16& operator=(UInt16&&) = default;

public:
    [[nodiscard]]
    QString toString(bool asHex) const;
    [[nodiscard]]
    QString toString(bool asHex, const QLocale& locale) const;

public:
    quint16 value = 0;
};

inline UInt16::UInt16() = default;
inline UInt16::UInt16(quint16 v) : value(v) {}

inline QString UInt16::toString(bool asHex) const
{
    return asHex ? QStringLiteral("0x%1").arg(value, 4, 16, QChar::fromLatin1('0')) :
                   QString::number(value);
}

inline QString UInt16::toString(bool asHex, const QLocale& locale) const
{
    return asHex ? QStringLiteral("0x%1").arg(value, 4, 16, QChar::fromLatin1('0')) :
                   locale.toString(value);
}

}

Q_DECLARE_METATYPE(Okteta::UInt16)

#endif
