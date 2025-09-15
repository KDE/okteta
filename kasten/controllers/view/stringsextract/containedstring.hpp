/*
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
    ContainedString(const ContainedString&) = default;
    ContainedString(ContainedString&&) = default;

    ~ContainedString() = default;

    ContainedString& operator=(const ContainedString&) = default;
    ContainedString& operator=(ContainedString&&) = default;

public:
    void move(Okteta::Size offset);

public:
    [[nodiscard]]
    const QString& string() const;
    [[nodiscard]]
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
