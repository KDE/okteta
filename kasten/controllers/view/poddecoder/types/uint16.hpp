/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UINT16_HPP
#define KASTEN_UINT16_HPP

// Qt
#include <QMetaType>
#include <QString>

struct UInt16
{
public:
    UInt16(quint16 v);
    UInt16();

public:
    QString toString(bool asHex) const;

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

Q_DECLARE_METATYPE(UInt16)

#endif
