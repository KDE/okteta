/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2003 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in AddressRange 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "addressrangelist.h"


namespace Okteta
{

AddressRangeList::AddressRangeList() {}


void AddressRangeList::addAddressRange( const AddressRange& newAddressRange )
{
    if( !newAddressRange.isValid() )
        return;

    // we try to insert it by ascending indizes
    // if addressRanges are overlapping we combine them
    Iterator firstOverlappingIt = begin();
    for( ; firstOverlappingIt!=end(); ++firstOverlappingIt )
    {
        // new addressRange before next addressRange?
        if( newAddressRange.endsBefore((*firstOverlappingIt).nextBeforeStart()) )
        {
            // put the new before it
            insert( firstOverlappingIt, newAddressRange );
            return;
        }

        // does the next addressRange overlap?
        if( (*firstOverlappingIt).isJoinable(newAddressRange) )
        {
            AddressRange joinedAddressRange( newAddressRange );
            // Start of the joined addressRanges is the smaller one
            joinedAddressRange.extendStartTo( (*firstOverlappingIt).start() );
            // next we search all the overlapping addressRanges and keep the highest end index
            int joinedEnd = (*firstOverlappingIt).end();
            Iterator lastOverlappingIt = firstOverlappingIt;
            for( ++lastOverlappingIt; lastOverlappingIt!=end(); ++lastOverlappingIt )
            {
                if( joinedAddressRange.endsBefore((*lastOverlappingIt).nextBeforeStart()) )
                    break;
                joinedEnd = (*lastOverlappingIt).end();
            }
            // the higher end is the end of the joined addressRange
            joinedAddressRange.extendEndTo( joinedEnd );
            // remove all overlapping addressRanges
            firstOverlappingIt = erase( firstOverlappingIt, lastOverlappingIt );
            // and instead insert the joined one
            insert( firstOverlappingIt, joinedAddressRange );
            return;
        }
    }

    // all others before the new?
    if( firstOverlappingIt == end() )
        // add it at the end
        append( newAddressRange );
}

void AddressRangeList::addAddressRangeList( const AddressRangeList& addressRangeList )
{
    // TODO: optimize with two parallel iterators
    foreach( const AddressRange& addressRange, addressRangeList )
        addAddressRange( addressRange );
}


AddressRangeList::~AddressRangeList() {}

}
