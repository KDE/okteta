/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KHE_CORE_KBOOKMARK_H
#define KHE_CORE_KBOOKMARK_H

// lib
#include "khe_export.h"


namespace KHECore
{

// TODO: do we need the invalid status?
class KHECORE_EXPORT KBookmark
{
  private:
    static const int InvalidOffset = -1;

  public:
    KBookmark( int offset ); // krazy:exclude=explicit
    KBookmark();
  public:
    bool operator==( const KBookmark &other ) const;
  public:
    int offset() const;
    bool isValid() const;
  public:
    void move( int offset );
  protected:
    int mOffset;
};

inline KBookmark::KBookmark( int offset ) : mOffset( offset ) {}
inline KBookmark::KBookmark() : mOffset( InvalidOffset ) {}
inline bool KBookmark::operator==( const KBookmark &other ) const { return mOffset == other.mOffset; }
inline bool KBookmark::isValid() const { return mOffset != InvalidOffset; }
inline int KBookmark::offset() const { return mOffset; }
inline void KBookmark::move( int offset ) { mOffset += offset; }

}

#endif
