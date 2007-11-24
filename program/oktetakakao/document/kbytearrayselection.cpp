/***************************************************************************
                          kbytearrayselection.cpp  -  description
                             -------------------
    begin                : Thu Nov 22 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kbytearrayselection.h"

// lib
#include "kbytearraydocument.h"


KByteArraySelection::KByteArraySelection( KByteArrayDocument *document )
 : mDocument( document )
{}

bool KByteArraySelection::isValid() const { return mDocument && mSection.isValid(); }
KHE::KSection KByteArraySelection::section() const { return mSection; }
KAbstractDocument *KByteArraySelection::document() const { return mDocument; }

void KByteArraySelection::setSection( const KHE::KSection &section ) { mSection = section; }

KByteArraySelection::~KByteArraySelection()
{}
