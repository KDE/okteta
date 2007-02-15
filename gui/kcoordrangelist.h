/***************************************************************************
                          kcoordrangelist.h  -  description
                             -------------------
    begin                : Wed Aug 13 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_UI_KCOORDRANGELIST_H
#define KHE_UI_KCOORDRANGELIST_H


// qt specific
#include <QtCore/QLinkedList>
// lib specific
#include "kcoordrange.h"

namespace KHEUI {

/**
@author Friedrich W. H.  Kossebau
*/
class KCoordRangeList : public QLinkedList<KCoordRange>
{
  public:
    KCoordRangeList();
    ~KCoordRangeList();

  public:
    void addCoordRange( KCoordRange R );
};

}

#endif
