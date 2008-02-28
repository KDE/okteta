/*
    This file is part of the Okteta Core library, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KHE_CORE_REPLACEMENTSCOPE_H
#define KHE_CORE_REPLACEMENTSCOPE_H

// lib
#include "khe_export.h"


namespace KHE
{

// TODO: do we need the invalid status?
class KHECORE_EXPORT ReplacementScope
{
  private:
    static const int InvalidOffset = -1;

  public:
    ReplacementScope( int offset, int removeLength, int insertLength );
    ReplacementScope();

  public:
    bool operator==( const ReplacementScope &other ) const;

  public:
    void revert();

  public:
    int offset() const;
    int removeLength() const;
    int insertLength() const;
    int lengthChange() const;
    bool isValid() const;

  protected:
    int mOffset;
    int mRemoveLength;
    int mInsertLength;
};

inline ReplacementScope::ReplacementScope( int offset, int removeLength, int insertLength )
 : mOffset( offset ), mRemoveLength( removeLength ), mInsertLength( insertLength )
{}
inline ReplacementScope::ReplacementScope() : mOffset( InvalidOffset ), mRemoveLength( 0 ), mInsertLength( 0 ) {}
inline bool ReplacementScope::operator==( const ReplacementScope &other ) const
{ return mOffset == other.mOffset && mRemoveLength == other.mRemoveLength && mInsertLength == other.mInsertLength; }
inline void ReplacementScope::revert()
{ const int helper = mInsertLength; mInsertLength = mRemoveLength; mRemoveLength = helper; }

inline bool ReplacementScope::isValid() const { return mOffset != InvalidOffset; }
inline int ReplacementScope::offset() const { return mOffset; }
inline int ReplacementScope::removeLength() const { return mRemoveLength; }
inline int ReplacementScope::insertLength() const { return mInsertLength; }
inline int ReplacementScope::lengthChange() const { return mInsertLength-mRemoveLength; }

}

#endif
