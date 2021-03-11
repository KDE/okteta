/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BOOKMARK_HPP
#define OKTETA_BOOKMARK_HPP

// lib
#include "oktetacore_export.hpp"
#include "address.hpp"
#include "size.hpp"
// Qt
#include <QString>

namespace Okteta {

// TODO: do we need the invalid status?
class OKTETACORE_EXPORT Bookmark
{
private:
    static constexpr Address InvalidAddress = -1;

public:
    Bookmark(Address offset);   // krazy:exclude=explicit
    Bookmark();

public:
    bool operator==(const Bookmark& other) const;

public:
    Address offset() const;
    QString name() const;
    bool isValid() const;

public:
    void move(Size offset);
    void setName(const QString& name);
    void setOffset(Address offset);

private:
    Address mOffset = InvalidAddress;
    QString mName;
};

inline Bookmark::Bookmark(Address offset) : mOffset(offset) {}
inline Bookmark::Bookmark() = default;
inline bool Bookmark::operator==(const Bookmark& other) const { return mOffset == other.mOffset; }
inline bool Bookmark::isValid() const { return mOffset != InvalidAddress; }
inline Address Bookmark::offset() const { return mOffset; }
inline QString Bookmark::name() const { return mName; }

inline void Bookmark::move(Size offset) { mOffset += offset; }
inline void Bookmark::setName(const QString& name) { mName = name; }
inline void Bookmark::setOffset(Address offset) { mOffset = offset; }

}

Q_DECLARE_TYPEINFO(Okteta::Bookmark, Q_MOVABLE_TYPE);

#endif
