/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2004 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHARACTER_HPP
#define OKTETA_CHARACTER_HPP

// lib
#include "oktetacore_export.hpp"
// Qt
#include <QChar>

namespace Okteta {

class OKTETACORE_EXPORT Character : public QChar
{
public:
    constexpr Character(QChar qchar);   // krazy:exclude=explicit
    constexpr Character(QChar qchar, bool isUndefined);

public:
    constexpr bool isUndefined() const;

private:
    // the byte is not defined
    bool mIsUndefined : 1;
};


inline constexpr Character::Character(QChar qchar)
    : QChar(qchar)
    , mIsUndefined(false)
{}

inline constexpr Character::Character(QChar qchar, bool isUndefined)
    : QChar(qchar)
    , mIsUndefined(isUndefined)
{}

inline constexpr bool Character::isUndefined() const { return mIsUndefined; }

}

#endif
