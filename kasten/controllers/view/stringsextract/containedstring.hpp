/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CONTAINEDSTRING_HPP
#define KASTEN_CONTAINEDSTRING_HPP

// Okteta core
#include <Okteta/Address>
#include <Okteta/Size>
// Qt
#include <QString>

class ContainedString
{
public:
    ContainedString(const QString& string, Okteta::Address offset);

public:
    void move(Okteta::Size offset);

public:
    const QString& string() const;
    Okteta::Address offset() const;

private:
    QString mString;
    Okteta::Address mOffset;
};

inline ContainedString::ContainedString(const QString& string, Okteta::Address offset)
    : mString(string)
    , mOffset(offset)
{}

inline void ContainedString::move(Okteta::Size offset) { mOffset += offset; }

inline const QString& ContainedString::string()  const { return mString; }
inline Okteta::Address ContainedString::offset() const { return mOffset; }

#endif
