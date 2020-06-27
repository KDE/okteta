/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BINARY8_HPP
#define KASTEN_BINARY8_HPP

// Qt
#include <QMetaType>
#include <QString>

struct Binary8
{
public:
    Binary8(quint8 v);
    Binary8();

public:
    QString toString() const;

public:
    quint8 value = 0;
};

inline Binary8::Binary8() = default;
inline Binary8::Binary8(quint8 v) : value(v) {}

inline QString Binary8::toString() const
{
    return QStringLiteral("%1").arg(value, 8, 2, QChar::fromLatin1('0'));
}

Q_DECLARE_METATYPE(Binary8)

#endif
