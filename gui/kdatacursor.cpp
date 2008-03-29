/*
    This file is part of the Okteta Gui library, part of the KDE project.

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

#include "kdatacursor.h"

// lib
#include "kdatalayout.h"
// Okteta core
#include <arraychangemetricslist.h>


namespace KHEUI
{

KDataCursor::KDataCursor( const KDataLayout *layout )
 : mLayout( layout ),
   mIndex( 0 ),
   mCoord( 0, 0 ),
   mBehind( false ),
   mAppendPosEnabled( false )
{
}


bool KDataCursor::operator==( const KDataCursor &other ) const
{
    return ( mIndex == other.mIndex && mBehind == other.mBehind );
}


void KDataCursor::setAppendPosEnabled( bool appendPosEnabled )
{
    if( mAppendPosEnabled == appendPosEnabled )
        return;

    mAppendPosEnabled = appendPosEnabled;
    // reposition Cursor
    const int length = mLayout->length();
    if( realIndex() >= length && mCoord.pos() < mLayout->noOfBytesPerLine()-1 && length > 0 )
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


void KDataCursor::gotoPreviousByte()
{
    if( mBehind )
        mBehind = false;
    else if( mIndex > 0 )
    {
        --mIndex;
        mCoord.goCLeft( mLayout->noOfBytesPerLine()-1 );
    }
}


void KDataCursor::gotoPreviousByte( int indexSteps )
{
    if( mBehind )
    {
        --indexSteps;
        mBehind = false;
    }
    const int newIndex = mIndex - indexSteps;
    // would step before first position?
    if( newIndex < 0 )
    {
        if( mIndex > 0 )
            gotoStart();
    }
    else
        gotoIndex( newIndex );
}


void KDataCursor::gotoNextByte()
{
    const int lastIndex = mLayout->length() -1 ;

    if( mIndex < lastIndex )
    {
        ++mIndex;
        mCoord.goCRight( mLayout->noOfBytesPerLine()-1 );
        mBehind = false;
    }
    else if( mIndex == lastIndex )
        stepToEnd();
}


void KDataCursor::gotoNextByte( int indexSteps ) // TODO: think about consistency with gotoNextByte!!!
{
    if( mBehind )
    {
        ++indexSteps;
        mBehind = false;
    }
    const int newIndex = mIndex + indexSteps;
    // would step behind the end?
    if( newIndex >= mLayout->length() )
        gotoEnd();
    else
        gotoIndex( newIndex );
}


void KDataCursor::gotoNextByteInLine()
{
    const int lastIndex = mLayout->length()-1;

    if( mIndex < lastIndex )
    {
        ++mIndex;

        if( mCoord.pos() < mLayout->noOfBytesPerLine()-1 )
            mCoord.goRight();
        else
            mBehind = true;
    }
    else if( mIndex == lastIndex )
        stepToEnd();
}


void KDataCursor::gotoUp()
{
    // can we even go up?
    if( mCoord.isBelow(mLayout->startLine()) )
    {
        mCoord.goUp();
        if( mCoord.isPriorInLineThan(mLayout->start()) )
        {
            mIndex = 0;
            mCoord.setPos( mLayout->startPos() );
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


void KDataCursor::gotoDown()
{
    if( mCoord.isAbove(mLayout->finalLine()) )
    {
        mCoord.goDown();
        // behind End?
        if( mCoord.isLaterInLineThan(mLayout->final()) )
            gotoEnd();
        else
            mIndex += mLayout->noOfBytesPerLine();
    }
}


void KDataCursor::gotoLineStart()
{
    const int oldIndex = mIndex;
    mIndex = mLayout->indexAtLineStart( mCoord.line() );
    mCoord.goLeft( oldIndex-mIndex );
    mBehind = false;
}


void KDataCursor::gotoLineEnd()
{
    if( mIndex < mLayout->length() )
    {
        const int oldIndex = mIndex;
        mIndex = mLayout->indexAtLineEnd( mCoord.line() );
        mCoord.goRight( mIndex-oldIndex );

        stepToEnd();
    }
}


void KDataCursor::gotoStart()
{
    mIndex = 0;
    mCoord = mLayout->start();
    mBehind = false;
}


void KDataCursor::gotoEnd()
{
    const int lastIndex = mLayout->length()-1;
    if( lastIndex >= 0 )
    {
        mIndex = lastIndex;
        mCoord = mLayout->final();

        stepToEnd();
    }
    else
        gotoStart();
}


void KDataCursor::gotoCIndex( int index )
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


void KDataCursor::gotoCCoord( const KCoord &coord )
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


void KDataCursor::stepToEnd()
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


void KDataCursor::gotoIndex( int index )
{
    mIndex = index;
    mCoord = mLayout->coordOfIndex( mIndex );
    mBehind = false;
}


void KDataCursor::gotoRealIndex()
{
    if( mBehind )
    {
        ++mIndex;
        mCoord = mLayout->coordOfIndex( mIndex );
        mBehind = false;
    }
}


void KDataCursor::gotoCoord( const KCoord &coord )
{
    mIndex = mLayout->indexAtCoord( coord );
    mCoord = coord;
    mBehind = false;
}


void KDataCursor::updateCoord()
{
    mCoord = mLayout->coordOfIndex( mIndex );
}

// page down should be: one page minus one line
// -> if in the very first line page down will put the cursor on the same page into the last line
void KDataCursor::gotoPageUp()
{
    const int noOfLinesPerPage = mLayout->noOfLinesPerPage();
    const int newIndex = mIndex - noOfLinesPerPage * mLayout->noOfBytesPerLine();
    if( newIndex >= 0 )
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


void KDataCursor::gotoPageDown()
{
    const int noOfLinesPerPage = mLayout->noOfLinesPerPage();
    const int newIndex = mIndex + noOfLinesPerPage * mLayout->noOfBytesPerLine();
    if( newIndex < mLayout->length() )
    {
        mIndex = newIndex;
        mCoord.goDown( noOfLinesPerPage );
    }
    else
        gotoEnd();
}


int KDataCursor::validIndex()       const { return mIndex < mLayout->length() ? mIndex : -1; }
int KDataCursor::indexAtLineStart() const { return mLayout->indexAtLineStart( mCoord.line() ); }
int KDataCursor::indexAtLineEnd()   const { return mLayout->indexAtLineEnd( mCoord.line() ); }


bool KDataCursor::atStart()     const { return mIndex == 0; }
bool KDataCursor::atEnd()       const { return mIndex == mLayout->length() - 1; }
bool KDataCursor::atAppendPos() const { return realIndex() >= mLayout->length(); }


bool KDataCursor::atLineStart() const { return mLayout->atLineStart( mCoord ); }
bool KDataCursor::atLineEnd()   const { return mLayout->atLineEnd( mCoord ); }

// TODO: oldLength is a hack, as DataLayout is already updated and used by e.g. gotoCIndex
void KDataCursor::adaptToChanges( const KHE::ArrayChangeMetricsList &changeList, int oldLength )
{
    for( int i=0; i<changeList.size(); ++i )
    {
        const KHE::ArrayChangeMetrics &change = changeList[i];
        // cursor affected?
        if( mIndex >= change.offset() )
        {
            switch( change.type() )
            {
            case KHE::ArrayChangeMetrics::Replacement:
                oldLength += change.lengthChange();
                if( oldLength > 0 )
                {
                    // step behind removed range if inside 
                    const int newIndexAfterRemove = ( mIndex >= change.offset()+change.removeLength() ) ?
                                                    mIndex - change.removeLength() :
                                                    change.offset();
                    const int newIndex = newIndexAfterRemove + change.insertLength();
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
            case KHE::ArrayChangeMetrics::Swapping:
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

KDataCursor::~KDataCursor() {}

}
