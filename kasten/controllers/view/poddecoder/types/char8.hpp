/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHAR8_HPP
#define KASTEN_CHAR8_HPP

// Okteta core
#include <Okteta/Character>
// Qt
#include <QMetaType>
#include <QString>

struct Char8
{
public:
    Char8(Okteta::Character c);
    Char8(const Char8&) = default;
    Char8();

    ~Char8() = default;

    Char8& operator=(const Char8&) = default;

public:
    QString toString(/** TODO: undefinedChar */) const;

public:
    Okteta::Character character = {QChar(0)};
};

inline Char8::Char8() = default;
inline Char8::Char8(Okteta::Character c) : character(c) {}

Q_DECLARE_METATYPE(Char8)

#endif
