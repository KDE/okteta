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

#ifndef KHE_CORE_ARRAYCHANGEMETRICS_H
#define KHE_CORE_ARRAYCHANGEMETRICS_H

// lib
#include "khe_export.h"

namespace KHE
{

// TODO: do we need the invalid status?
class KHECORE_EXPORT ArrayChangeMetrics
{
  private:
    static const int InvalidOffset = -1;

  public:
    enum Type { /*Insertion, Removal,*/ Replacement, Swapping/*, Filling, Setting*/ };

  public:
    static ArrayChangeMetrics asReplacement( int offset, int removeLength, int insertLength );
    static ArrayChangeMetrics asSwapping( int firstOffset, int secondOffset, int secondLength );

  protected:
    ArrayChangeMetrics( Type type, int offset, int secondArgument, int thirdArgument );

  public:
    bool operator==( const ArrayChangeMetrics &other ) const;

  public:
    void revert();

  public:
    int type() const;
    int offset() const;
    bool isValid() const;

  public: // Replacement properties
    int removeLength() const;
    int insertLength() const;
    int lengthChange() const;

  public: // Swapping properties
    int firstLength() const;
    int secondStart() const;
    int secondEnd() const;
    int secondLength() const;

  protected:
    Type mType;
    int mOffset;
    union
    {
    int mSecondArgument;
    int mRemoveLength;
    int mSecondStart;
    };
    union
    {
    int mThirdArgument;
    int mInsertLength;
    int mSecondLength;
    };
};

inline ArrayChangeMetrics ArrayChangeMetrics::asReplacement( int offset, int removeLength, int insertLength )
{
    return ArrayChangeMetrics( Replacement, offset, removeLength, insertLength );
}

inline ArrayChangeMetrics ArrayChangeMetrics::asSwapping( int firstOffset, int secondOffset, int secondLength )
{
    return ArrayChangeMetrics( Swapping, firstOffset, secondOffset, secondLength );
}


inline ArrayChangeMetrics::ArrayChangeMetrics( Type type, int offset, int secondArgument, int thirdArgument )
 : mType( type ), mOffset( offset ), mSecondArgument( secondArgument ), mThirdArgument( thirdArgument )
{}
// inline ArrayChangeMetrics::ArrayChangeMetrics() : mOffset( InvalidOffset ), mRemoveLength( 0 ), mInsertLength( 0 ) {}
inline bool ArrayChangeMetrics::operator==( const ArrayChangeMetrics &other ) const
{ return mType == other.mType
         && mOffset == other.mOffset
         && mSecondArgument == other.mSecondArgument
         && mThirdArgument == other.mThirdArgument; }
inline void ArrayChangeMetrics::revert()
{
    if( mType == Replacement )
    {
        const int helper = mInsertLength;
        mInsertLength = mRemoveLength;
        mRemoveLength = helper;
    }
    else if( mType == Swapping )
    {
        const int oldSecondLength = mSecondLength;
        mSecondLength = firstLength();
        mSecondStart = mOffset + oldSecondLength;
    }
}

inline bool ArrayChangeMetrics::isValid()     const { return mOffset != InvalidOffset; }
inline int ArrayChangeMetrics::type()         const { return mType; }
inline int ArrayChangeMetrics::offset()       const { return mOffset; }
inline int ArrayChangeMetrics::removeLength() const { return mRemoveLength; }
inline int ArrayChangeMetrics::insertLength() const { return mInsertLength; }
inline int ArrayChangeMetrics::lengthChange() const { return mInsertLength-mRemoveLength; }
inline int ArrayChangeMetrics::secondStart()  const { return mSecondStart; }
inline int ArrayChangeMetrics::secondEnd()    const { return mSecondStart+mSecondLength-1; }
inline int ArrayChangeMetrics::firstLength()  const { return mSecondStart-mOffset; }
inline int ArrayChangeMetrics::secondLength() const { return mSecondLength; }

}

#endif
