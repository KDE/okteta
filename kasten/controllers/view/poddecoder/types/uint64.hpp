/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UINT64_HPP
#define KASTEN_UINT64_HPP

// Qt
#include <QMetaType>
#include <QString>

struct UInt64
{
public:
    UInt64(quint64 v);
    UInt64();

public:
    QString toString(bool asHex) const;

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

Q_DECLARE_METATYPE(UInt64)

#endif
