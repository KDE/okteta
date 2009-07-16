/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_BOOKMARK_H
#define OKTETA_BOOKMARK_H

// lib
#include "oktetacore_export.h"
// Qt
#include <QtCore/QString>


namespace Okteta
{

// TODO: do we need the invalid status?
class OKTETACORE_EXPORT Bookmark
{
  private:
    static const int InvalidOffset = -1;

  public:
    Bookmark( int offset ); // krazy:exclude=explicit
    Bookmark();
  public:
    bool operator==( const Bookmark &other ) const;
  public:
    int offset() const;
    QString name() const;
    bool isValid() const;
  public:
    void move( int offset );
    void setName( const QString& name );
    void setOffset( int offset );
  protected:
    int mOffset;
    QString mName;
};


inline Bookmark::Bookmark( int offset ) : mOffset( offset ) {}
inline Bookmark::Bookmark() : mOffset( InvalidOffset ) {}
inline bool Bookmark::operator==( const Bookmark &other ) const { return mOffset == other.mOffset; } 
inline bool Bookmark::isValid() const { return mOffset != InvalidOffset; }
inline int Bookmark::offset() const { return mOffset; }
inline QString Bookmark::name() const { return mName; }

inline void Bookmark::move( int offset ) { mOffset += offset; }
inline void Bookmark::setName( const QString& name ) { mName = name; }
inline void Bookmark::setOffset( int offset ) { mOffset = offset; }

}

#endif
