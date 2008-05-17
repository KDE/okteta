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

#include "bytearraytablelayout.h"


namespace KHEUI
{

ByteArrayTableLayout::ByteArrayTableLayout( int noOfBytesPerLine, int startOffset, int length )
 : mNoOfBytesPerLine( noOfBytesPerLine ),
   mStartOffset( startOffset ),
   mLength( length )
{
    calcStart();
    calcEnd();
}


bool ByteArrayTableLayout::setStartOffset( int startOffset )
{
    // rejecting <0
    if( startOffset < 0 )
        startOffset = 0;

    if( mStartOffset == startOffset )
        return false;

    mStartOffset = startOffset;

    calcStart();
    calcEnd();
    return true;
}


bool ByteArrayTableLayout::setNoOfBytesPerLine( int noOfBytesPerLine )
{
    // rejecting <1
    if( noOfBytesPerLine < 1 )
        noOfBytesPerLine = 1;

    // no changes?
    if( mNoOfBytesPerLine == noOfBytesPerLine )
        return false;

    mNoOfBytesPerLine = noOfBytesPerLine;

    calcStart();
    calcEnd();
    return true;
}


bool ByteArrayTableLayout::setLength( int length )
{
    // rejecting < 0
    if( length < 0 )
        length = 0;

    // no changes?
    if( mLength == length )
        return false;

    mLength = length;

    calcEnd();
    return true;
}


void ByteArrayTableLayout::setNoOfLinesPerPage( int noOfLinesPerPage )
{
    mNoOfLinesPerPage = noOfLinesPerPage;
}


void ByteArrayTableLayout::calcStart()
{
    mCoordRange.setStart( Coord::fromIndex(mStartOffset,mNoOfBytesPerLine) );
}


void ByteArrayTableLayout::calcEnd()
{
    mCoordRange.setEnd( (mLength>0)?Coord::fromIndex(mLength-1+mStartOffset,mNoOfBytesPerLine):
                                    Coord(-1,mCoordRange.start().line()) );
}


int ByteArrayTableLayout::indexAtCFirstLinePosition( int line ) const
{
    return ( line <= mCoordRange.start().line() ) ? 0:
           ( line > mCoordRange.end().line() ) ?    mLength-1:
                                                    line * mNoOfBytesPerLine - mStartOffset;
}


int ByteArrayTableLayout::indexAtCLastLinePosition( int line ) const
{
    return ( line < mCoordRange.start().line() ) ? 0:
           ( line >= mCoordRange.end().line() ) ?  mLength-1:
                                                   (line+1)*mNoOfBytesPerLine-mStartOffset-1;
}


int ByteArrayTableLayout::indexAtCCoord( const Coord &coord ) const
{
    const int index = indexAtCoord( coord );

    return ( index <= 0 ) ?       0:
           ( index >= mLength ) ? mLength-1:
                                  index;
}


int ByteArrayTableLayout::lineAtCIndex( int index ) const
{
    return ( index <= 0 ) ?       mCoordRange.start().line():
           ( index >= mLength ) ? mCoordRange.end().line():
                                  lineAtIndex(index);
}


Coord ByteArrayTableLayout::coordOfCIndex( int index ) const
{
    return ( index <= 0 ) ?       mCoordRange.start():
           ( index >= mLength ) ? mCoordRange.end():
                                  coordOfIndex(index);
}


int ByteArrayTableLayout::indexAtFirstLinePosition( int line ) const
{
    return ( line == mCoordRange.start().line() ) ? 0 : line*mNoOfBytesPerLine-mStartOffset;
}


int ByteArrayTableLayout::indexAtLastLinePosition( int line ) const
{
    return ( line == mCoordRange.end().line() ) ? mLength-1 : (line+1)*mNoOfBytesPerLine-mStartOffset-1;
}


int ByteArrayTableLayout::indexAtCoord( const Coord &coord ) const
{
    return coord.indexByLineWidth( mNoOfBytesPerLine ) - mStartOffset;
}

int ByteArrayTableLayout::lineAtIndex( int index ) const
{
    return (index+mStartOffset)/mNoOfBytesPerLine;
}

Coord ByteArrayTableLayout::coordOfIndex( int index ) const
{
    return Coord::fromIndex( index+mStartOffset, mNoOfBytesPerLine );
}

CoordRange ByteArrayTableLayout::coordRangeOfIndizes( const KHE::KSection &indizes ) const
{
    return CoordRange(
             Coord::fromIndex(indizes.start()+mStartOffset, mNoOfBytesPerLine),
             Coord::fromIndex(indizes.end()+mStartOffset,   mNoOfBytesPerLine) );
}



int ByteArrayTableLayout::correctIndex( int index ) const
{
    return ( index <= 0 ) ?       0:
           ( index >= mLength ) ? mLength-1:
                                  index;
}


Coord ByteArrayTableLayout::correctCoord( const Coord &coord ) const
{
    return ( coord <= mCoordRange.start() ) ?    mCoordRange.start():
           ( coord >= mCoordRange.end() ) ?      mCoordRange.end():
           ( coord.pos() >= mNoOfBytesPerLine ) ? Coord( mNoOfBytesPerLine-1, coord.line() ):
                                                  coord;
}


bool ByteArrayTableLayout::atFirstLinePosition( const Coord &coord ) const
{
    return ( coord.line() == mCoordRange.start().line() ) ? coord.pos() == mCoordRange.start().pos():
                                                            coord.pos() == 0;
}

bool ByteArrayTableLayout::atLastLinePosition( const Coord &coord ) const
{
    return ( coord.line() == mCoordRange.end().line() ) ? coord.pos() == mCoordRange.end().pos():
                                                          coord.pos() == mNoOfBytesPerLine-1;
}


KHE::KSection ByteArrayTableLayout::linePositions( int line ) const
{
    return KHE::KSection( firstLinePosition(line), lastLinePosition(line) );
}


int ByteArrayTableLayout::firstLinePosition( const Coord &coord ) const
{
    return ( mCoordRange.start().isLaterInLineThan(coord) ) ? mCoordRange.start().pos() : coord.pos();
}

int ByteArrayTableLayout::lastLinePosition( const Coord &coord ) const
{
    return ( mCoordRange.end().isPriorInLineThan(coord) ) ? mCoordRange.end().pos() : coord.pos();
}

int ByteArrayTableLayout::firstLinePosition( int line ) const
{
    return line == mCoordRange.start().line() ? mCoordRange.start().pos() : 0;
}

int ByteArrayTableLayout::lastLinePosition( int line ) const
{
    return ( line == mCoordRange.end().line() ) ? mCoordRange.end().pos() : mNoOfBytesPerLine-1;
}

bool ByteArrayTableLayout::hasContent( int line ) const
{
    return mCoordRange.includesLine( line );
}

ByteArrayTableLayout::~ByteArrayTableLayout() {}

}
