/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "fixedsizebytearraymodel.h"

// lib
#include <arraychangemetricslist.h>
// C
#include <string.h>


namespace Okteta
{

FixedSizeByteArrayModel::FixedSizeByteArrayModel( Byte* data, int size, Byte fillUpByte, QObject* parent )
  : AbstractByteArrayModel( parent ),
    mData( data ),
    mSize( size ),
    mFillUpByte( fillUpByte ),
    mReadOnly( true ),
    mModified( false ),
    mAutoDelete( false )
{
}

FixedSizeByteArrayModel::FixedSizeByteArrayModel( int size, Byte fillUpByte, QObject* parent )
  : AbstractByteArrayModel( parent ),
    mData( new Byte[size] ),
    mSize( size ),
    mFillUpByte( fillUpByte ),
    mReadOnly( false ),
    mModified( false ),
    mAutoDelete( true )
{
    reset( 0, size );
}


void FixedSizeByteArrayModel::setByte( Address offset, Byte byte )
{
    const bool wasModifiedBefore = mModified;

    mData[offset] = byte;
    mModified = true;

    emit contentsChanged( ArrayChangeMetricsList::oneReplacement(offset, 1, 1) );
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );
}



Size FixedSizeByteArrayModel::insert( Address offset, const Byte* insertData, int insertLength )
{
    // check all parameters
    if( offset >= mSize || insertLength == 0 )
        return 0;

    const bool wasModifiedBefore = mModified;

    if( offset + insertLength > mSize )
        insertLength = mSize - offset;

    const Address behindInsertOffset = offset + insertLength;
    // fmove right data behind the input range
    memmove( &mData[behindInsertOffset], &mData[offset], mSize-behindInsertOffset );
    // insert input
    memcpy( &mData[offset], insertData, insertLength );

    mModified = true;

    emit contentsChanged( ArrayChangeMetricsList::oneReplacement(offset, 0, insertLength) );
    //emit contentsReplaced( offset, , 0 ); TODO: how to signal the removed data?
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );

    return insertLength;
}


Size FixedSizeByteArrayModel::remove( const AddressRange& _removeRange )
{
    AddressRange removeRange( _removeRange );
    if( removeRange.start() >= mSize || removeRange.width() == 0 )
        return 0;

    const bool wasModifiedBefore = mModified;

    removeRange.restrictEndTo( mSize-1 );

    const Size removeLength = removeRange.width();
    const Address behindRemoveOffset = removeRange.nextBehindEnd();
    // fmove right data behind the input range
    memmove( &mData[removeRange.start()], &mData[behindRemoveOffset], mSize-behindRemoveOffset );
    // clear freed space
    reset( mSize-removeLength, removeLength );

    mModified = true;

    emit contentsChanged( ArrayChangeMetricsList::oneReplacement(removeRange.start(), removeRange.width(), 0) );
    //emit contentsReplaced( offset, 0,  ); TODO: how to signal the inserted data?
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );

    return removeLength;
}


Size FixedSizeByteArrayModel::replace( const AddressRange& _removeRange, const Byte* insertData, int insertLength )
{
    AddressRange removeRange( _removeRange );
    // check all parameters
    if( removeRange.startsBehind( mSize-1 ) || (removeRange.width()==0 && insertLength==0) )
        return 0;

    const bool wasModifiedBefore = mModified;

    removeRange.restrictEndTo( mSize-1 );
    if( removeRange.start() + insertLength > mSize )
        insertLength = mSize - removeRange.start();

    const Size sizeDiff = insertLength - removeRange.width();

    // is input longer than removed?
    if( sizeDiff > 0 )
    {
        const Address behindInsertOffset = removeRange.start() + insertLength;
        // fmove right data behind the input range
        memmove( &mData[behindInsertOffset], &mData[removeRange.nextBehindEnd()], mSize-behindInsertOffset );
    }
    // is input smaller than removed?
    else if( sizeDiff < 0 )
    {
        const Address behindRemoveOffset = removeRange.nextBehindEnd();
        // fmove right data behind the input range
        memmove( &mData[removeRange.start()+insertLength], &mData[behindRemoveOffset], mSize-behindRemoveOffset );
        // clear freed space
        reset( mSize+sizeDiff, -sizeDiff );
    }
    // insert input
    memcpy( &mData[removeRange.start()], insertData, insertLength );

    mModified = true;

    emit contentsChanged( ArrayChangeMetricsList::oneReplacement(removeRange.start(), removeRange.width(), insertLength) );
    //emit contentsReplaced( offset, 0,  ); TODO: how to signal the changed data at the end?
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );

    return insertLength;
}


bool FixedSizeByteArrayModel::swap( Address firstStart, const AddressRange& _secondRange )
{
    AddressRange secondRange( _secondRange );
    // check all parameters
    if( secondRange.start() >= mSize || secondRange.width() == 0
        || firstStart > mSize || secondRange.start() == firstStart )
        return false;

    const bool wasModifiedBefore = mModified;

    secondRange.restrictEndTo( mSize-1 );
    const bool toRight = firstStart > secondRange.start();
    const Size movedLength = secondRange.width();
    const Size displacedLength = toRight ?  firstStart - secondRange.end()-1 : secondRange.start() - firstStart;

    // find out section that is smaller
    Size smallPartLength, largePartLength, smallPartStart, largePartStart, smallPartDest, largePartDest;
    // moving part is smaller?
    if( movedLength < displacedLength )
    {
        smallPartStart = secondRange.start();
        smallPartLength = movedLength;
        largePartLength = displacedLength;
        // moving part moves right?
        if( toRight )
        {
            smallPartDest = firstStart - movedLength;
            largePartStart = secondRange.nextBehindEnd();
            largePartDest = secondRange.start();
        }
        else
        {
            smallPartDest = firstStart;
            largePartStart = firstStart;
            largePartDest = firstStart + movedLength;
        }
    }
    else
    {
        largePartStart = secondRange.start();
        largePartLength = movedLength;
        smallPartLength = displacedLength;
        // moving part moves right?
        if( toRight )
        {
            largePartDest = firstStart - movedLength;
            smallPartStart = secondRange.nextBehindEnd();
            smallPartDest = secondRange.start();
        }
        else
        {
            largePartDest = firstStart;
            smallPartStart = firstStart;
            smallPartDest = firstStart + movedLength;
        }
    }

    // copy smaller part to tempbuffer
    Byte* tempBuffer = new Byte[smallPartLength];
    memcpy( tempBuffer, &mData[smallPartStart], smallPartLength );

    // move the larger part
    memmove( &mData[largePartDest], &mData[largePartStart], largePartLength );

    // copy smaller part to its new dest
    memcpy( &mData[smallPartDest], tempBuffer, smallPartLength );
    delete [] tempBuffer;

    mModified = true;

    emit contentsChanged( ArrayChangeMetricsList::oneSwapping(firstStart, secondRange.start(),secondRange.width()) );
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );

    return true;
}


Size FixedSizeByteArrayModel::fill( Byte fillByte, Address offset, Size fillLength )
{
    // nothing to fill
    if( offset >= mSize )
        return 0;

    const bool wasModifiedBefore = mModified;

    const Size lengthToEnd = mSize - offset;

    if( fillLength < 0 || fillLength > lengthToEnd )
        fillLength = lengthToEnd;

    memset( &mData[offset], fillByte, fillLength );
    mModified = true;

    emit contentsChanged( ArrayChangeMetricsList::oneReplacement(offset, fillLength, fillLength) );
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );

    return fillLength;
}


int FixedSizeByteArrayModel::compare( const AbstractByteArrayModel& other, const AddressRange& _otherRange, Address offset )
{
    AddressRange otherRange( _otherRange );
    //kDebug() << QString("offset: %1, otherRange: (%3/%4)" ).arg(offset).arg(otherRange.start()).arg(otherRange.end())
    //    << endl;
    // test other values
    if( otherRange.startsBehind(other.size()-1) )
        return 1;

    // check own values
    if( offset >= mSize )
        return -1;

    int valueByLength = 0; // default: equal

    AddressRange range = AddressRange::fromWidth( offset, otherRange.width() );
    Address lastOffset = other.size()-1;
    // 
    if( otherRange.endsBehind(lastOffset) )
    {
        // make shorter
        otherRange.setEnd( lastOffset );
        if( otherRange.width() < range.width() )
            valueByLength = 1;
    }
    lastOffset = mSize-1;
    if( range.endsBehind(lastOffset) )
    {
        // make shorter
        range.setEnd( lastOffset );
        if( otherRange.width() > range.width() )
            valueByLength = -1;
    }
    //kDebug()
    //    << QString( "range: (%1/%2), otherRange: (%3/%4)" ).arg(range.start()).arg(range.end()).arg(otherRange.start()).arg(otherRange.end())
    //    << endl;
    const Address rangeEnd = range.end();
    Address oi = otherRange.start();
    for( Address i=range.start(); i<=rangeEnd; ++i,++oi )
    {
        Byte OD = other.byte(oi);
        Byte data = mData[i];
        //kDebug() << QString("%1==%2").arg((int)data).arg((int)OD) ;
        if( OD == data )
            continue;
        return ( OD < data ) ? 1 : -1;
    }

    return valueByLength;
}


void FixedSizeByteArrayModel::reset( unsigned int offset, unsigned int length )
{
    memset( &mData[offset], mFillUpByte, length );
}


FixedSizeByteArrayModel::~FixedSizeByteArrayModel()
{
    if( mAutoDelete )
        delete [] mData;
}

}
