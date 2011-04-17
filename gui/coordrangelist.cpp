/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "coordrangelist.h"


namespace Okteta
{

CoordRangeList::CoordRangeList()
{
}


void CoordRangeList::addCoordRange( const CoordRange& coordRange )
{
    if( ! coordRange.isValid() )
        return;

    // we try to insert it by ascending indizes
    // if sections are overlapping we combine them
    Iterator it = begin();
    Iterator endIt = end();
    for( ; it != endIt; ++it )
    {
    // TODO: add bufferwidth to rangelist so consecutive ranges can be joined, cmp sectionlist
        // is next CoordRange behind the new CoordRange?
        if( coordRange.endsBefore(*it) )
        {
            // put the new before it
            insert( it, coordRange );
            return;
        }

        // does the next CoordRange overlap?
        if( (*it).overlaps(coordRange) )
        {
            CoordRange mergedCoordRange( coordRange );
            // Start of the combined sections is the smaller one
            mergedCoordRange.extendStartTo( (*it).start() );
            // next we search all the overlapping sections and keep the highest end index
            Coord endCoord( (*it).end() );
            iterator it2 = it;
            for( ++it2; it2 != endIt; ++it2 )
            {
                if( coordRange.endsBefore((*it2).start()) )
                    break;
                endCoord = (*it2).end();
            }
            // the higher end is the end of the combined CoordRange
            mergedCoordRange.extendEndTo( endCoord );
            // remove all overlapping sections
            it = erase( it, it2 );
            // and instead insert the combined one
            insert( it, mergedCoordRange );
            return;
        }
    }

    // all others are before the new?
    if( it == endIt )
        // add it at the end
        append( coordRange );
}


CoordRangeList::~CoordRangeList()
{
}

}
