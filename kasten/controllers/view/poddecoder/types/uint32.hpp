/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UINT32_HPP
#define KASTEN_UINT32_HPP

// Qt
#include <QMetaType>
#include <QString>

struct UInt32
{
public:
    UInt32(quint32 v);
    UInt32();

public:
    QString toString(bool asHex) const;

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

Q_DECLARE_METATYPE(UInt32)

#endif
