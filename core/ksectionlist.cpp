/*
    This file is part of the Okteta Core library, part of the KDE project.

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

#include "ksectionlist.h"


namespace KHE
{

KSectionList::KSectionList() {}


void KSectionList::addSection( const KSection &newSection )
{
    if( !newSection.isValid() )
        return;

    // we try to insert it by ascending indizes
    // if sections are overlapping we combine them
    Iterator firstOverlappingIt = begin();
    for( ; firstOverlappingIt!=end(); ++firstOverlappingIt )
    {
        // new section before next section?
        if( newSection.endsBefore((*firstOverlappingIt).nextBeforeStart()) )
        {
            // put the new before it
            insert( firstOverlappingIt, newSection );
            return;
        }

        // does the next section overlap?
        if( (*firstOverlappingIt).isJoinable(newSection) )
        {
            KSection joinedSection( newSection );
            // Start of the joined sections is the smaller one
            joinedSection.extendStartTo( (*firstOverlappingIt).start() );
            // next we search all the overlapping sections and keep the highest end index
            int joinedEnd = (*firstOverlappingIt).end();
            Iterator lastOverlappingIt = firstOverlappingIt;
            for( ++lastOverlappingIt; lastOverlappingIt!=end(); ++lastOverlappingIt )
            {
                if( joinedSection.endsBefore((*lastOverlappingIt).nextBeforeStart()) )
                    break;
                joinedEnd = (*lastOverlappingIt).end();
            }
            // the higher end is the end of the joined section
            joinedSection.extendEndTo( joinedEnd );
            // remove all overlapping sections
            firstOverlappingIt = erase( firstOverlappingIt, lastOverlappingIt );
            // and instead insert the joined one
            insert( firstOverlappingIt, joinedSection );
            return;
        }
    }

    // all others before the new?
    if( firstOverlappingIt == end() )
        // add it at the end
        append( newSection );
}

void KSectionList::addSectionList( const KSectionList& sectionList )
{
    // TODO: optimize with two parallel iterators
    foreach( const KSection &section, sectionList )
        addSection( section );
}


KSectionList::~KSectionList() {}

}
