/***************************************************************************
                          ksectionlist.h  -  description
                             -------------------
    begin                : Mon Jun 30 2003
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


#ifndef KHE_KSECTIONLIST_H
#define KHE_KSECTIONLIST_H

// lib
#include "ksection.h"
// Qt
#include <QtCore/QLinkedList>


namespace KHE {

/**
@author Friedrich W. H.  Kossebau
*/
class KSectionList : public QLinkedList<KSection>
{
  public:
    KSectionList();
    ~KSectionList();

  public:
    void addSection( KSection S );
};

}

#endif
