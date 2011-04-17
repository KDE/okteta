/*
    This file is part of the Okteta Core library, made within the KDE community.

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

#ifndef OKTETA_ARRAYCHANGEMETRICS_H
#define OKTETA_ARRAYCHANGEMETRICS_H

// lib
#include "oktetacore_export.h"
#include "size.h"
#include "address.h"
// Qt
#include <QtCore/QDataStream>


namespace Okteta
{

// TODO: do we need the invalid status?
class OKTETACORE_EXPORT ArrayChangeMetrics
{
    friend QDataStream& operator<<( QDataStream& outStream, const ArrayChangeMetrics& metrics );
    friend QDataStream& operator>>( QDataStream& inStream, ArrayChangeMetrics& metrics );

  private:
    static const Address InvalidAddress = -1;

  public:
    enum Type { /*Insertion, Removal,*/ Replacement, Swapping/*, Filling, Setting*/, Invalid };

  public:
    static ArrayChangeMetrics asReplacement( Address offset, Size removeLength, Size insertLength );
    static ArrayChangeMetrics asSwapping( Address firstOffset, Address secondOffset, Size secondLength );

  public:
    ArrayChangeMetrics();

  protected:
  public:
    ArrayChangeMetrics( Type type, Address offset, qint32 secondArgument, qint32 thirdArgument );

  public:
    bool operator==( const ArrayChangeMetrics& other ) const;

  public:
    void revert();

  public:
    int type() const;
    Address offset() const;
    bool isValid() const;

  public: // Replacement properties
    Size removeLength() const;
    Size insertLength() const;
    Size lengthChange() const;

  public: // Swapping properties
    Size firstLength() const;
    Address secondStart() const;
    Address secondEnd() const;
    Size secondLength() const;

  protected:
    Type mType;
    Address mOffset;
    // TODO: how to make the arguments of the size of the largest union member?
    union
    {
    qint32 mSecondArgument;
    Size mRemoveLength;
    Address mSecondStart;
    };
    union
    {
    qint32 mThirdArgument;
    Size mInsertLength;
    Size mSecondLength;
    };
};

inline ArrayChangeMetrics ArrayChangeMetrics::asReplacement( Address offset, Size removeLength, Size insertLength )
{
    return ArrayChangeMetrics( Replacement, offset, removeLength, insertLength );
}

inline ArrayChangeMetrics ArrayChangeMetrics::asSwapping( Address firstOffset, Address secondOffset, Size secondLength )
{
    return ArrayChangeMetrics( Swapping, firstOffset, secondOffset, secondLength );
}


inline ArrayChangeMetrics::ArrayChangeMetrics( Type type, Address offset, qint32 secondArgument, qint32 thirdArgument )
 : mType( type ), mOffset( offset ), mSecondArgument( secondArgument ), mThirdArgument( thirdArgument )
{}
inline ArrayChangeMetrics::ArrayChangeMetrics() : mType(Invalid) ,mOffset( InvalidAddress ), mSecondArgument( 0 ), mThirdArgument( 0 ) {}
inline bool ArrayChangeMetrics::operator==( const ArrayChangeMetrics& other ) const
{
    return mType == other.mType
           && mOffset == other.mOffset
           && mSecondArgument == other.mSecondArgument
           && mThirdArgument == other.mThirdArgument;
}
inline void ArrayChangeMetrics::revert()
{
    if( mType == Replacement )
    {
        const Size helper = mInsertLength;
        mInsertLength = mRemoveLength;
        mRemoveLength = helper;
    }
    else if( mType == Swapping )
    {
        const Size oldSecondLength = mSecondLength;
        mSecondLength = firstLength();
        mSecondStart = mOffset + oldSecondLength;
    }
}

inline bool ArrayChangeMetrics::isValid()        const { return mOffset != InvalidAddress; }
inline int ArrayChangeMetrics::type()            const { return mType; }
inline Address ArrayChangeMetrics::offset()      const { return mOffset; }
inline Size ArrayChangeMetrics::removeLength()   const { return mRemoveLength; }
inline Size ArrayChangeMetrics::insertLength()   const { return mInsertLength; }
inline Size ArrayChangeMetrics::lengthChange()   const { return mInsertLength-mRemoveLength; }
inline Address ArrayChangeMetrics::secondStart() const { return mSecondStart; }
inline Address ArrayChangeMetrics::secondEnd()   const { return mSecondStart+mSecondLength-1; }
inline Size ArrayChangeMetrics::firstLength()    const { return mSecondStart-mOffset; }
inline Size ArrayChangeMetrics::secondLength()   const { return mSecondLength; }


QDataStream& operator<<( QDataStream& outStream, const ArrayChangeMetrics& metrics );
QDataStream& operator>>( QDataStream& inStream, ArrayChangeMetrics& metrics );

inline QDataStream& operator<<( QDataStream& outStream, const ArrayChangeMetrics& metrics )
{
    outStream << metrics.mType << metrics.mOffset << metrics.mSecondArgument << metrics.mThirdArgument;
    return outStream;
}

inline QDataStream& operator>>( QDataStream& inStream, ArrayChangeMetrics& metrics )
{
    int type;
    inStream >> type >> metrics.mOffset >> metrics.mSecondArgument >> metrics.mThirdArgument;
    metrics.mType = (ArrayChangeMetrics::Type)type; //TODO: find out how to stream to enum directly
    return inStream;
}

}

#endif
