/***************************************************************************
                          kdocumentcreatemanager.cpp  -  description
                             -------------------
    begin                : Fri Jun 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kdocumentcreatemanager.h"

// lib
#include "kabstractdocumentfactory.h"
#include "kdocumentmanager.h"


KDocumentCreateManager::KDocumentCreateManager( KDocumentManager *manager )
 : mManager( manager ), mFactory( 0 ) {}

void KDocumentCreateManager::setWidget( QWidget *widget )
{
    mWidget = widget;
}

void KDocumentCreateManager::setDocumentFactory( KAbstractDocumentFactory *factory )
{
    mFactory = factory;
}


void KDocumentCreateManager::createNew()
{
   KAbstractDocument *document = mFactory->create();
   mManager->addDocument( document );
}


KDocumentCreateManager::~KDocumentCreateManager()
{
    delete mFactory;
}
