/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraytablecursor.h"

// lib
#include "bytearraytablelayout.h"
// Okteta core
#include <arraychangemetricslist.h>


namespace Okteta
{

ByteArrayTableCursor::ByteArrayTableCursor( const ByteArrayTableLayout* layout )
 : mLayout( layout ),
   mIndex( layout->byteArrayOffset() ),
   mCoord( layout->startCoord() ),
   mBehind( false ),
   mAppendPosEnabled( false )
{
}

void ByteArrayTableCursor::setAppendPosEnabled( bool appendPosEnabled )
{
    if( mAppendPosEnabled == appendPosEnabled )
        return;

    mAppendPosEnabled = appendPosEnabled;
    // reposition Cursor
    if( realIndex() > mLayout->lastByteArrayOffset()
        && mCoord.pos() < mLayout->noOfBytesPerLine()-1
        && mLayout->length() > 0 )
    {
        if( mAppendPosEnabled )
        {
            ++mIndex;
            mCoord.goRight();
            mBehind = false;
        }
        else
        {
            --mIndex;
            mCoord.goLeft();
            mBehind = true;
        }
    }
}


void ByteArrayTableCursor::gotoPreviousByte()
{
    if( mBehind )
        mBehind = false;
    else if( mIndex > mLayout->byteArrayOffset() )
    {
        --mIndex;
        mCoord.goCLeft( mLayout->noOfBytesPerLine()-1 );
    }
}


void ByteArrayTableCursor::gotoPreviousByte( Size indexSteps )
{
    if( mBehind )
    {
        --indexSteps;
        mBehind = false;
    }
    const Address newIndex = mIndex - indexSteps;
    // would step before first position?
    const Address firstIndex = mLayout->byteArrayOffset();
    if( newIndex < firstIndex )
    {
        if( mIndex > firstIndex )
            gotoStart();
    }
    else
        gotoIndex( newIndex );
}


void ByteArrayTableCursor::gotoNextByte()
{
    const Address lastIndex = mLayout->lastByteArrayOffset();

    if( mIndex < lastIndex )
    {
        ++mIndex;
        mCoord.goCRight( mLayout->noOfBytesPerLine()-1 );
        mBehind = false;
    }
    else if( mIndex == lastIndex )
        stepToEnd();
}


void ByteArrayTableCursor::gotoNextByte( Size indexSteps ) // TODO: think about consistency with gotoNextByte!!!
{
    if( mBehind )
    {
        ++indexSteps;
        mBehind = false;
    }
    const Address newIndex = mIndex + indexSteps;
    // would step behind the end?
    if( newIndex > mLayout->lastByteArrayOffset() )
        gotoEnd();
    else
        gotoIndex( newIndex );
}


void ByteArrayTableCursor::gotoUp()
{
    // can we even go up?
    if( mCoord.isBelow(mLayout->startLine()) )
    {
        mCoord.goUp();
        if( mCoord.isPriorInLineThan(mLayout->startCoord()) )
        {
            mIndex = mLayout->byteArrayOffset();
            mCoord.setPos( mLayout->firstStartLinePosition() );
            mBehind = false;
        }
        else
        {
            mIndex -= mLayout->noOfBytesPerLine();
            if( mBehind && !atLineEnd() )
            {
                ++mIndex;
                mCoord.goRight();
                mBehind = false;
            }
        }
    }
}


void ByteArrayTableCursor::gotoDown()
{
    if( mCoord.isAbove(mLayout->finalLine()) )
    {
        mCoord.goDown();
        // behind End?
        if( mCoord.isLaterInLineThan(mLayout->finalCoord()) )
            gotoEnd();
        else
            mIndex += mLayout->noOfBytesPerLine();
    }
}


void ByteArrayTableCursor::gotoLineStart()
{
    const Address oldIndex = mIndex;
    mIndex = mLayout->indexAtFirstLinePosition( mCoord.line() );
    mCoord.goLeft( oldIndex-mIndex );
    mBehind = false;
}


void ByteArrayTableCursor::gotoLineEnd()
{
    if( mIndex <= mLayout->lastByteArrayOffset() )
    {
        const Address oldIndex = mIndex;
        mIndex = mLayout->indexAtLastLinePosition( mCoord.line() );
        mCoord.goRight( mIndex-oldIndex );

        stepToEnd();
    }
}


void ByteArrayTableCursor::gotoStart()
{
    mIndex = mLayout->byteArrayOffset();
    mCoord = mLayout->startCoord();
    mBehind = false;
}


void ByteArrayTableCursor::gotoEnd()
{
    const Address lastIndex = mLayout->lastByteArrayOffset();
    if( lastIndex >= 0 )
    {
        mIndex = lastIndex;
        mCoord = mLayout->finalCoord();

        stepToEnd();
    }
    else
        gotoStart();
}


void ByteArrayTableCursor::gotoCIndex( Address index )
{
    if( mLayout->length() > 0 )
    {
        mIndex = mLayout->correctIndex( index );
        mCoord = mLayout->coordOfIndex( mIndex );
        mBehind = ( index > mIndex );
    }
    else
        gotoStart();
}


void ByteArrayTableCursor::gotoCCoord( const Coord& coord )
{
    if( mLayout->length() > 0 )
    {
        mCoord = mLayout->correctCoord( coord );
        mIndex = mLayout->indexAtCoord( mCoord );
        if( coord > mCoord )
            stepToEnd();
        else
            mBehind = false;
    }
    else
        gotoStart();
}


void ByteArrayTableCursor::stepToEnd()
{
    if( mAppendPosEnabled && (mCoord.pos() < mLayout->noOfBytesPerLine()-1) )
    {
        ++mIndex;
        mCoord.goRight();
        mBehind = false;
    }
    else
        mBehind = true;
}


void ByteArrayTableCursor::gotoIndex( Address index )
{
    mIndex = index;
    mCoord = mLayout->coordOfIndex( mIndex );
    mBehind = false;
}


void ByteArrayTableCursor::gotoRealIndex()
{
    if( mBehind
        && (mAppendPosEnabled || ( mIndex < mLayout->lastByteArrayOffset() )) )
    {
        ++mIndex;
        mCoord.goCRight( mLayout->noOfBytesPerLine()-1 );
        mBehind = false;
    }
}


void ByteArrayTableCursor::gotoCoord( const Coord& coord )
{
    mIndex = mLayout->indexAtCoord( coord );
    mCoord = coord;
    mBehind = false;
}


void ByteArrayTableCursor::updateCoord()
{
    mCoord = mLayout->coordOfIndex( mIndex );
}

// page down should be: one page minus one line
// -> if in the very first line page down will put the cursor on the same page into the last line
void ByteArrayTableCursor::gotoPageUp()
{
    const LineSize noOfLinesPerPage = mLayout->noOfLinesPerPage();
    const Address newIndex = mIndex - noOfLinesPerPage * mLayout->noOfBytesPerLine();
    if( newIndex >= mLayout->byteArrayOffset() )
    {
        mIndex = newIndex;
        mCoord.goUp( noOfLinesPerPage );
        if( mBehind && !atLineEnd() )
        {
            ++mIndex;
            mCoord.goRight();
            mBehind = false;
        }
    }
    else
        gotoStart();
}


void ByteArrayTableCursor::gotoPageDown()
{
    const LineSize noOfLinesPerPage = mLayout->noOfLinesPerPage();
    const Address newIndex = mIndex + noOfLinesPerPage * mLayout->noOfBytesPerLine();
    if( newIndex <= mLayout->lastByteArrayOffset() )
    {
        mIndex = newIndex;
        mCoord.goDown( noOfLinesPerPage );
    }
    else
        gotoEnd();
}


Address ByteArrayTableCursor::validIndex() const
{
    return ( mLayout->byteArrayOffset() <= mIndex && mIndex <= mLayout->lastByteArrayOffset() ) ? mIndex : -1;
}

Address ByteArrayTableCursor::indexAtLineStart() const { return mLayout->indexAtFirstLinePosition( mCoord.line() ); }
Address ByteArrayTableCursor::indexAtLineEnd()   const { return mLayout->indexAtLastLinePosition( mCoord.line() ); }


bool ByteArrayTableCursor::atStart()     const { return mIndex == mLayout->byteArrayOffset(); }
bool ByteArrayTableCursor::atEnd()       const { return realIndex() == mLayout->lastByteArrayOffset()+1; }
bool ByteArrayTableCursor::atAppendPos() const { return mIndex == mLayout->lastByteArrayOffset()+1; }


bool ByteArrayTableCursor::atLineStart() const { return mLayout->atFirstLinePosition( mCoord ); }
bool ByteArrayTableCursor::atLineEnd()   const { return mLayout->atLastLinePosition( mCoord ); }

// TODO: oldLength is a hack, as DataLayout is already updated and used by e.g. gotoCIndex
void ByteArrayTableCursor::adaptToChanges( const ArrayChangeMetricsList& changeList, Size oldLength )
{
    foreach( const ArrayChangeMetrics& change, changeList )
    {
        // cursor affected?
        if( mIndex >= change.offset() )
        {
            switch( change.type() )
            {
            case ArrayChangeMetrics::Replacement:
                oldLength += change.lengthChange();
                if( oldLength > 0 )
                {
                    const Address newIndex =
                        // cursor behind the removed section?
                        ( mIndex >= change.offset()+change.removeLength() ) ? mIndex + change.lengthChange() :
                        // cursor at substituted section?
                        ( mIndex < change.offset()+change.insertLength() ) ?  mIndex :
                        // cursor at unsubstituted section
                                                                              change.offset() + change.insertLength();

                    // if the cursor gets behind, it will never get inside again.
                    if( newIndex >= oldLength )
                    {
                        gotoEnd();
                        return;
                    }
                    mIndex = newIndex;
                }
                // if the cursor gets at the start, it will stay there
                else
                {
                    gotoStart();
                    return;
                }
                break;
            case ArrayChangeMetrics::Swapping:
                if( mIndex < change.secondStart() )
                {
                    mIndex += change.secondLength();
                }
                else if( mIndex <= change.secondEnd() )
                {
                    mIndex -= change.firstLength();
                }
                break;
            default:
                ;
            }
        }
    }

    const bool wasBehind = ( mIndex >= oldLength );
    if( wasBehind )
        mIndex = oldLength - 1;
    updateCoord();
    if( wasBehind )
        stepToEnd();
}

ByteArrayTableCursor::~ByteArrayTableCursor() {}

}
