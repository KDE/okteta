/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_HEXADECIMAL8_HPP
#define OKTETA_HEXADECIMAL8_HPP

// Qt
#include <QMetaType>
#include <QString>

namespace Okteta {

struct Hexadecimal8
{
public:
    Hexadecimal8(quint8 v);
    Hexadecimal8(const Hexadecimal8&) = default;
    Hexadecimal8();

    ~Hexadecimal8() = default;

    Hexadecimal8& operator=(const Hexadecimal8&) = default;

public:
    QString toString() const;

public:
    quint8 value = 0;
};

inline Hexadecimal8::Hexadecimal8() = default;
inline Hexadecimal8::Hexadecimal8(quint8 v) : value(v) {}

inline QString Hexadecimal8::toString() const
{
    return QStringLiteral("%1").arg(value, 2, 16, QChar::fromLatin1('0'));
}

}

Q_DECLARE_METATYPE(Okteta::Hexadecimal8)

#endif
