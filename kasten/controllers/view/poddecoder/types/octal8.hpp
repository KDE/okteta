/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_OCTAL8_HPP
#define KASTEN_OCTAL8_HPP

// Qt
#include <QMetaType>
#include <QString>

struct Octal8
{
public:
    Octal8(quint8 v);
    Octal8();

public:
    QString toString() const;

public:
    quint8 value = 0;
};

inline Octal8::Octal8() = default;
inline Octal8::Octal8(quint8 v) : value(v) {}

inline QString Octal8::toString() const
{
    return QStringLiteral("%1").arg(value, 3, 8, QChar::fromLatin1('0'));
}

Q_DECLARE_METATYPE(Octal8)

#endif
