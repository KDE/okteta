/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CONTAINEDSTRING_H
#define CONTAINEDSTRING_H

// Okteta core
#include <address.h>
#include <size.h>
// Qt
#include <QtCore/QString>


class ContainedString
{
  public:
    ContainedString( const QString &string, Okteta::Address offset );

  public:
    void move( Okteta::Size offset );

  public:
    const QString& string() const;
    Okteta::Address offset() const;

  protected:
    QString mString;
    Okteta::Address mOffset;
};


inline ContainedString::ContainedString( const QString &string, Okteta::Address offset )
 : mString( string ), mOffset( offset )
{}

inline void ContainedString::move( Okteta::Size offset ) { mOffset += offset; }

inline const QString& ContainedString::string()  const { return mString; }
inline Okteta::Address ContainedString::offset() const { return mOffset; }

#endif
