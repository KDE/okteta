/***************************************************************************
                          kcoordrangelist.h  -  description
                             -------------------
    begin                : Wed Aug 13 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_KCOORDRANGELIST_H
#define KHE_KCOORDRANGELIST_H

// qt specific
#include <QLinkedList>
// lib specific
#include "kcoordrange.h"

namespace KHE {

/**
@author Friedrich W. H.  Kossebau
*/
class KCoordRangeList : public QLinkedList<KCoordRange>
{
  public:
    KCoordRangeList();
    ~KCoordRangeList();

  public:
    void addCoordRange( KCoordRange S );
};

}

#endif
