/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractfilesystemsyncwithremotejob_p.h"

// library
#include "abstractmodelfilesystemsynchronizer.h"
#include <abstractdocument.h>
// KDE
#include <KIO/NetAccess>
#include <KLocale>
#include <KDirWatch>


namespace Kasten
{
void AbstractFileSystemSyncWithRemoteJobPrivate::syncWithRemote()
{
    Q_Q( AbstractFileSystemSyncWithRemoteJob );

// Comment: here we play tricks to reuse the temporary file
// KIO::NetAccess::removeTempFile only removes tempfiles created by KIO::NetAccess::download
// So if replaceRemote and workFilePath is temporaryFile both don't conflict -> no problem (now)

    bool isWorkFileOk;
    if( mOption == AbstractModelSynchronizer::ReplaceRemote )
    {
        if( mUrl.isLocalFile() )
            mWorkFilePath = mUrl.path();
        else
        {
            mTemporaryFile = new KTemporaryFile;
            mTemporaryFile->open();
            mWorkFilePath = mTemporaryFile->fileName();
        }
        isWorkFileOk = true;
    }
    else
        isWorkFileOk = KIO::NetAccess::download( mUrl.url(), mWorkFilePath, widget() );

    if( isWorkFileOk )
    {
        q->startSyncWithRemote();
    }
    else
    {
        q->setError( KJob::KilledJobError );
        q->setErrorText( KIO::NetAccess::lastErrorString() );
        // TODO: should we rather skip completeSync in successthe API?
        q->emitResult();
    }
}


void AbstractFileSystemSyncWithRemoteJobPrivate::completeSync( bool success )
{
    Q_Q( AbstractFileSystemSyncWithRemoteJob );

    if( success )
    {
        KDirWatch* dirWatch = KDirWatch::self();
        const KUrl oldUrl = mSynchronizer->url();
        // care for old url
        if( oldUrl.isLocalFile() )
        {
            dirWatch->disconnect( mSynchronizer );
            dirWatch->removeFile( oldUrl.path() );
        }
        if( ! mUrl.isLocalFile() )
        {
            const bool uploaded = KIO::NetAccess::upload( mWorkFilePath, mUrl, widget() );
            if( ! uploaded )
            {
                q->setError( KJob::KilledJobError );
                q->setErrorText( KIO::NetAccess::lastErrorString() );
            }
        }
        else
        {
            QObject::connect( dirWatch, SIGNAL(dirty( const QString& )),
                              mSynchronizer, SLOT(onFileDirty( const QString& )) );

            QObject::connect( dirWatch, SIGNAL(created( const QString& )),
                              mSynchronizer, SLOT(onFileCreated( const QString& )) );

            QObject::connect( dirWatch, SIGNAL(deleted( const QString& )),
                              mSynchronizer, SLOT(onFileDeleted( const QString& )) );
            dirWatch->addFile( mWorkFilePath );
        }
        mSynchronizer->setUrl( mUrl );
    }
    else
    {
        q->setError( KJob::KilledJobError );
        q->setErrorText( i18nc("@info","Problem while synching with local filesystem.") );
    }

    KIO::NetAccess::removeTempFile( mWorkFilePath );
    delete mTemporaryFile;

    q->emitResult();
}

}

