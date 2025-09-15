/*
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
    Character(const Character&) = default;
    Character(Character&&) = default;

    ~Character() = default;

    Character& operator=(const Character&) = default;
    Character& operator=(Character&&) = default;

public:
    [[nodiscard]]
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

Q_DECLARE_TYPEINFO(Okteta::Character, Q_MOVABLE_TYPE);

#endif
