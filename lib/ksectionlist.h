/***************************************************************************
                          ksectionlist.h  -  description
                             -------------------
    begin                : Mon Jun 30 2003
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


#ifndef KHE_KSECTIONLIST_H
#define KHE_KSECTIONLIST_H

// qt specific
#include <qvaluelist.h>
// lib specific
#include "ksection.h"

namespace KHE {

typedef QValueList<KSection> KSectionBasicList;
/**
@author Friedrich W. H.  Kossebau
*/
class KSectionList : public KSectionBasicList
{
  public:
    KSectionList();
    ~KSectionList();

  public:
    void addSection( KSection S );
};

}

#endif
