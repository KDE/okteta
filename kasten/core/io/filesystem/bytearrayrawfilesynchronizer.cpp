/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayrawfilesynchronizer.h"

// lib
#include "bytearrayrawfileloadjob.h"
#include "bytearrayrawfileconnectjob.h"
#include "bytearrayrawfilewritejob.h"
#include "bytearrayrawfilereloadjob.h"
#include "bytearrayrawfilewritetojob.h"
#include "bytearraydocument.h"
// Okteta core
#include <piecetablebytearraymodel.h>
// KDE
#include <KUrl>


namespace Kasten2
{

ByteArrayRawFileSynchronizer::ByteArrayRawFileSynchronizer()
 : mDocument( 0 )
{
    connect( this, SIGNAL(urlChanged(KUrl)), SLOT(onUrlChange(KUrl)) );
}

AbstractDocument* ByteArrayRawFileSynchronizer::document() const { return mDocument; }

LocalSyncState ByteArrayRawFileSynchronizer::localSyncState() const
{
    return mDocument ?
        (mDocument->content()->isModified() ? LocalHasChanges : LocalInSync) : LocalInSync;
}

void ByteArrayRawFileSynchronizer::setDocument( ByteArrayDocument* document )
{
    mDocument = document;
    if( mDocument )
        connect( mDocument->content(), SIGNAL(modifiedChanged(bool)),
                 SLOT(onModelModified(bool)) );
}

void ByteArrayRawFileSynchronizer::startOffering( AbstractDocument* document ) { Q_UNUSED(document) }

AbstractLoadJob *ByteArrayRawFileSynchronizer::startLoad( const KUrl &url )
{
    return new ByteArrayRawFileLoadJob( this, url );
}

AbstractSyncToRemoteJob *ByteArrayRawFileSynchronizer::startSyncToRemote()
{
    return new ByteArrayRawFileWriteJob( this );
}

AbstractSyncFromRemoteJob *ByteArrayRawFileSynchronizer::startSyncFromRemote()
{
    return new ByteArrayRawFileReloadJob( this );
}

AbstractSyncWithRemoteJob *ByteArrayRawFileSynchronizer::startSyncWithRemote( const KUrl &url, AbstractModelSynchronizer::ConnectOption option  )
{
    return new ByteArrayRawFileWriteToJob( this, url, option );
}

AbstractConnectJob *ByteArrayRawFileSynchronizer::startConnect( AbstractDocument* document,
                                              const KUrl& url, AbstractModelSynchronizer::ConnectOption option )
{
    return new ByteArrayRawFileConnectJob( this, document, url, option );
}

void ByteArrayRawFileSynchronizer::onUrlChange( const KUrl &url )
{
    mDocument->setTitle( url.fileName() );
}

void ByteArrayRawFileSynchronizer::onModelModified( bool isModified )
{
    emit localSyncStateChanged( (isModified ? LocalHasChanges : LocalInSync) );
}

}
