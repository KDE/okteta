/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2006-2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "documentmanager.h"

// KDE
#include <KUrl>
// Qt
#include <QtCore/QMutableListIterator>
#include <QtCore/QStringList>

// temporary
#include "documentcreatemanager.h"
#include "documentsyncmanager.h"
#include "modelcodecmanager.h"


namespace Kasten
{

static int lastDocumentId = 0;


DocumentManager::DocumentManager()
 : mCreateManager( new DocumentCreateManager(this) ),
   mSyncManager( new DocumentSyncManager(this) ),
   mCodecManager( new ModelCodecManager(this) )
{}

QList<AbstractDocument*> DocumentManager::documents() const { return mList; }

QStringList DocumentManager::urls() const
{
    QStringList result;

    foreach( AbstractDocument* document, mList )
        result.append( mSyncManager->urlOf(document).url() );

    return result;
}

AbstractDocument* DocumentManager::documentOfUrl( const KUrl& url ) const
{
    AbstractDocument* result = 0;

    foreach( AbstractDocument* document, mList )
    {
        if( url == mSyncManager->urlOf(document) )
        {
            result = document;
            break;
        }
    }

    return result;
}

void DocumentManager::addDocument( AbstractDocument* document )
{
    // TODO: check for double insert
    document->setId( QString::number(++lastDocumentId) );
    mList.append( document );
    // TODO: only emit if document was not included before
    emit added( document );
}

void DocumentManager::closeDocument( AbstractDocument* document )
{
    QMutableListIterator<AbstractDocument*> iterator( mList );

    if( iterator.findNext(document) )
    {
    // TODO: first check if unsaved and ask, only then close

        iterator.remove();
        emit closing( document );
        delete document;
    }
}

void DocumentManager::closeAll()
{
    foreach( AbstractDocument* document, mList )
    {
        emit closing( document );
        delete document;
    }
    mList.clear();
}

bool DocumentManager::canClose( AbstractDocument* document )
{
    return mSyncManager->canClose( document );
}

bool DocumentManager::canCloseAll()
{
    bool canCloseAll = true;

    foreach( AbstractDocument* document, mList )
    {
        if( !mSyncManager->canClose(document) )
        {
            canCloseAll = false;
            break;
        }
    }

    return canCloseAll;
}

void DocumentManager::requestFocus( AbstractDocument* document )
{
    emit focusRequested( document );
}

DocumentManager::~DocumentManager()
{
    delete mCreateManager;
    delete mSyncManager;
    delete mCodecManager;
} //TODO: destroy all documents?

}
