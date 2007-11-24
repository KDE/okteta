/***************************************************************************
                          kdocumentmanager.cpp  -  description
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


#include "kdocumentmanager.h"

// Qt
#include <QtCore/QMutableListIterator>


// temporary
#include "kdocumentcreatemanager.h"
#include "kdocumentsyncmanager.h"
#include "kdocumentcodecmanager.h"

KDocumentManager::KDocumentManager()
 : mCreateManager( new KDocumentCreateManager(this) ),
   mSyncManager( new KDocumentSyncManager(this) ),
   mCodecManager( new KDocumentCodecManager(this) )
{}

void KDocumentManager::addDocument( KAbstractDocument *document )
{
    // TODO: check for double insert
    mList.append( document );
    // TODO: only emit if document was not included before
    emit added( document );
}

void KDocumentManager::closeDocument( KAbstractDocument *document )
{
    QMutableListIterator<KAbstractDocument*> iterator( mList );

    if( iterator.findNext(document) )
    {
    // TODO: first check if unsaved and ask, only then close

        iterator.remove();
        emit closing( document );
        delete document;
    }
}

bool KDocumentManager::canClose( KAbstractDocument *document )
{
    return mSyncManager->canClose( document );
}

bool KDocumentManager::canCloseAll()
{
    bool canCloseAll = true;

    foreach( KAbstractDocument *document, mList )
    {
        if( !mSyncManager->canClose(document) )
        {
            canCloseAll = false;
            break;
        }
    }

    return canCloseAll;
}

KDocumentManager::~KDocumentManager()
{
    delete mCreateManager;
    delete mSyncManager;
    delete mCodecManager;
} //TODO: destroy all documents?
