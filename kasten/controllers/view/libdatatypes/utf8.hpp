/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_UTF8_HPP
#define OKTETA_UTF8_HPP

// Qt
#include <QMetaType>
#include <QString>

namespace Okteta {

struct Utf8
{
public:
    Utf8(QChar v);
    Utf8(const Utf8&) = default;
    Utf8(Utf8&&) = default;
    Utf8();

    ~Utf8() = default;

    Utf8& operator=(const Utf8&) = default;
    Utf8& operator=(Utf8&&) = default;

public:
    [[nodiscard]]
    QString toString() const;

public:
    QChar value;
};

inline Utf8::Utf8() = default;
inline Utf8::Utf8(QChar v) : value(v) {}

inline QString Utf8::toString() const { return QString(value); }

}

Q_DECLARE_METATYPE(Okteta::Utf8)

#endif
