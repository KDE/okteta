/*
    This file is part of the Okteta Core library, part of the KDE project.

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

FixedSizeByteArrayModel::FixedSizeByteArrayModel( char* data, unsigned int size, char fillUpChar )
  : mData( data ),
    mSize( size ),
    mFillUpChar( fillUpChar ),
    mReadOnly( true ),
    mModified( false ),
    mAutoDelete( false )
{
}

FixedSizeByteArrayModel::FixedSizeByteArrayModel( unsigned int size, char fillUpChar )
  : mData( new char[size] ),
    mSize( size ),
    mFillUpChar( fillUpChar ),
    mReadOnly( false ),
    mModified( false ),
    mAutoDelete( true )
{
    reset( 0, size );
}


void FixedSizeByteArrayModel::setDatum( unsigned int offset, const char datum )
{
    const bool wasModifiedBefore = mModified;

    mData[offset] = datum;
    mModified = true;

    emit contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(offset, 1, 1) );
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );
}



int FixedSizeByteArrayModel::insert( int offset, const char* insertData, int insertLength )
{
    // check all parameters
    if( offset >= (int)mSize || insertLength == 0 )
        return 0;

    const bool wasModifiedBefore = mModified;

    if( offset + insertLength > (int)mSize )
        insertLength = mSize - offset;

    const unsigned int behindInsertOffset = offset + insertLength;
    // fmove right data behind the input range
    memmove( &mData[behindInsertOffset], &mData[offset], mSize-behindInsertOffset );
    // insert input
    memcpy( &mData[offset], insertData, insertLength );

    mModified = true;

    emit contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(offset, 0, insertLength) );
    //emit contentsReplaced( offset, , 0 ); TODO: how to signal the removed data?
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );

    return insertLength;
}


int FixedSizeByteArrayModel::remove( const KDE::Section& _removeSection )
{
    KDE::Section removeSection( _removeSection );
    if( removeSection.start() >= (int)mSize || removeSection.width() == 0 )
        return 0;

    const bool wasModifiedBefore = mModified;

    removeSection.restrictEndTo( mSize-1 );

    const int removeLength = removeSection.width();
    const int behindRemoveOffset = removeSection.nextBehindEnd();
    // fmove right data behind the input range
    memmove( &mData[removeSection.start()], &mData[behindRemoveOffset], mSize-behindRemoveOffset );
    // clear freed space
    reset( mSize-removeLength, removeLength );

    mModified = true;

    emit contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(removeSection.start(), removeSection.width(), 0) );
    //emit contentsReplaced( offset, 0,  ); TODO: how to signal the inserted data?
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );

    return removeLength;
}


unsigned int FixedSizeByteArrayModel::replace( const KDE::Section& _removeSection, const char* insertData, unsigned int insertLength )
{
    KDE::Section removeSection( _removeSection );
    // check all parameters
    if( removeSection.startsBehind( mSize-1 ) || (removeSection.width()==0 && insertLength==0) )
        return 0;

    const bool wasModifiedBefore = mModified;

    removeSection.restrictEndTo( mSize-1 );
    if( removeSection.start() + insertLength > mSize )
        insertLength = mSize - removeSection.start();

    const int sizeDiff = insertLength - removeSection.width();

    // is input longer than removed?
    if( sizeDiff > 0 )
    {
        unsigned int behindInsertOffset = removeSection.start() + insertLength;
        // fmove right data behind the input range
        memmove( &mData[behindInsertOffset], &mData[removeSection.nextBehindEnd()], mSize-behindInsertOffset );
    }
    // is input smaller than removed?
    else if( sizeDiff < 0 )
    {
        unsigned int behindRemoveOffset = removeSection.nextBehindEnd();
        // fmove right data behind the input range
        memmove( &mData[removeSection.start()+insertLength], &mData[behindRemoveOffset], mSize-behindRemoveOffset );
        // clear freed space
        reset( mSize+sizeDiff, -sizeDiff );
    }
    // insert input
    memcpy( &mData[removeSection.start()], insertData, insertLength );

    mModified = true;

    emit contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(removeSection.start(), removeSection.width(), insertLength) );
    //emit contentsReplaced( offset, 0,  ); TODO: how to signal the changed data at the end?
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );

    return insertLength;
}


bool FixedSizeByteArrayModel::swap( int firstStart, const KDE::Section& secondSection )
{
    KDE::Section sourceSection( secondSection );
    // check all parameters
    if( sourceSection.start() >= (int)mSize || sourceSection.width() == 0
        || firstStart > (int)mSize || sourceSection.start() == firstStart )
        return false;

    const bool wasModifiedBefore = mModified;

    sourceSection.restrictEndTo( mSize-1 );
    const bool toRight = firstStart > sourceSection.start();
    const int movedLength = sourceSection.width();
    const int displacedLength = toRight ?  firstStart - sourceSection.end()-1 : sourceSection.start() - firstStart;

    // find out section that is smaller
    int smallPartLength, largePartLength, smallPartStart, largePartStart, smallPartDest, largePartDest;
    // moving part is smaller?
    if( movedLength < displacedLength )
    {
        smallPartStart = sourceSection.start();
        smallPartLength = movedLength;
        largePartLength = displacedLength;
        // moving part moves right?
        if( toRight )
        {
            smallPartDest = firstStart - movedLength;
            largePartStart = sourceSection.nextBehindEnd();
            largePartDest = sourceSection.start();
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
        largePartStart = sourceSection.start();
        largePartLength = movedLength;
        smallPartLength = displacedLength;
        // moving part moves right?
        if( toRight )
        {
            largePartDest = firstStart - movedLength;
            smallPartStart = sourceSection.nextBehindEnd();
            smallPartDest = sourceSection.start();
        }
        else
        {
            largePartDest = firstStart;
            smallPartStart = firstStart;
            smallPartDest = firstStart + movedLength;
        }
    }

    // copy smaller part to tempbuffer
    char* tempBuffer = new char[smallPartLength];
    memcpy( tempBuffer, &mData[smallPartStart], smallPartLength );

    // move the larger part
    memmove( &mData[largePartDest], &mData[largePartStart], largePartLength );

    // copy smaller part to its new dest
    memcpy( &mData[smallPartDest], tempBuffer, smallPartLength );
    delete [] tempBuffer;

    mModified = true;

    emit contentsChanged( KDE::ArrayChangeMetricsList::oneSwapping(firstStart, sourceSection.start(),sourceSection.width()) );
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );

    return true;
}


int FixedSizeByteArrayModel::fill( const char fillChar, unsigned int offset, int fillLength )
{
    // nothing to fill
    if( offset >= mSize )
        return 0;

    const bool wasModifiedBefore = mModified;

    const unsigned int lengthToEnd = mSize - offset;

    if( fillLength < 0 || fillLength > (int)lengthToEnd )
        fillLength = lengthToEnd;

    memset( &mData[offset], fillChar, fillLength );
    mModified = true;

    emit contentsChanged( KDE::ArrayChangeMetricsList::oneReplacement(offset, fillLength, fillLength) );
    if( ! wasModifiedBefore )
        emit modifiedChanged( true );

    return fillLength;
}


int FixedSizeByteArrayModel::compare( const AbstractByteArrayModel& other, const KDE::Section& _otherRange, unsigned int offset )
{
    KDE::Section otherRange( _otherRange );
    //kDebug() << QString("offset: %1, otherRange: (%3/%4)" ).arg(offset).arg(otherRange.start()).arg(otherRange.end())
    //    << endl;
    // test other values
    if( otherRange.startsBehind(other.size()-1) )
        return 1;

    // check own values
    if( offset >= mSize )
        return -1;

    int valueByLength = 0; // default: equal

    KDE::Section range = KDE::Section::fromWidth( offset, otherRange.width() );
    int lastOffset = other.size()-1;
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
    int oi = otherRange.start();
    for( int i=range.start(); i<=range.end(); ++i,++oi )
    {
        char OD = other.datum(oi);
        char data = mData[i];
        //kDebug() << QString("%1==%2").arg((int)data).arg((int)OD) ;
        if( OD == data )
            continue;
        return ( OD < data ) ? 1 : -1;
    }

    return valueByLength;
}


void FixedSizeByteArrayModel::reset( unsigned int offset, unsigned int length )
{
    memset( &mData[offset], mFillUpChar, length );
}


FixedSizeByteArrayModel::~FixedSizeByteArrayModel()
{
    if( mAutoDelete )
        delete [] mData;
}

}
