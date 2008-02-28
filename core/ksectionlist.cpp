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

KSectionList::KSectionList()
{
}


KSectionList::~KSectionList()
{
}


void KSectionList::addSection( KSection NewSection )
{
  if( !NewSection.isValid() )
    return;

  // we try to insert it by ascending indizes
  // if sections are overlapping we combine them
  iterator S = begin();
  for( ; S!=end(); ++S )
  {
    // new section before next section?
    if( NewSection.endsBefore((*S).beforeStart()) )
    {
      // put the new before it
      insert( S, NewSection );
      return;
    }

    // does the next section overlap?
    if( (*S).isJoinable(NewSection) )
    {
      // Start of the combined sections is the smaller one
      NewSection.extendStartTo( (*S).start() );
      // next we search all the overlapping sections and keep the highest end index
      int End = (*S).end();
      iterator LS = S;
      for( ++LS; LS!=end(); ++LS )
      {
        if( NewSection.endsBefore((*LS).beforeStart()) )
          break;
        End = (*LS).end();
      }
      // the higher end is the end of the combined section
      NewSection.extendEndTo( End );
      // remove all overlapping sections
      S = erase( S, LS );
      // and instead insert the combined one
      insert( S, NewSection );
      return;
    }
  }

  // all others before the new?
  if( S == end() )
    // add it at the end
    append( NewSection );
}

}
