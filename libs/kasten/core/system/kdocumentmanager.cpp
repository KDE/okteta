/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kdocumentmanager.h"

// KDE
#include <KUrl>
// Qt
#include <QtCore/QMutableListIterator>
#include <QtCore/QStringList>

// temporary
#include "kdocumentcreatemanager.h"
#include "kdocumentsyncmanager.h"
#include "modelcodecmanager.h"


namespace Kasten
{

static int lastDocumentId = 0;


KDocumentManager::KDocumentManager()
 : mCreateManager( new KDocumentCreateManager(this) ),
   mSyncManager( new KDocumentSyncManager(this) ),
   mCodecManager( new ModelCodecManager(this) )
{}

QList<KAbstractDocument*> KDocumentManager::documents() const { return mList; }

QStringList KDocumentManager::urls() const
{
    QStringList result;

    foreach( KAbstractDocument *document, mList )
        result.append( mSyncManager->urlOf(document).url() );

    return result;
}

KAbstractDocument* KDocumentManager::documentOfUrl( const KUrl& url ) const
{
    KAbstractDocument* result = 0;

    foreach( KAbstractDocument *document, mList )
    {
        if( url == mSyncManager->urlOf(document) )
        {
            result = document;
            break;
        }
    }

    return result;
}

void KDocumentManager::addDocument( KAbstractDocument *document )
{
    // TODO: check for double insert
    document->setId( QString::number(++lastDocumentId) );
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

void KDocumentManager::requestFocus( KAbstractDocument* document )
{
    emit focusRequested( document );
}

KDocumentManager::~KDocumentManager()
{
    delete mCreateManager;
    delete mSyncManager;
    delete mCodecManager;
} //TODO: destroy all documents?

}
