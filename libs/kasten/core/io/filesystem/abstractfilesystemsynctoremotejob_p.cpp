/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008-2009,2011,2014 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractfilesystemsynctoremotejob_p.h"

// KF5
#include <KIO/FileCopyJob>
#include <KJobWidgets>
// Qt
#include <QFileInfo>
#include <QDateTime>

namespace Kasten
{

void AbstractFileSystemSyncToRemoteJobPrivate::syncToRemote()
{
    Q_Q( AbstractFileSystemSyncToRemoteJob );

    bool isWorkFileOk;
    const QUrl url = mSynchronizer->url();

    if( url.isLocalFile() )
    {
        mWorkFilePath = url.path(QUrl::FullyDecoded);
        mFile = new QFile( mWorkFilePath );
        isWorkFileOk = mFile->open( QIODevice::WriteOnly );

        mSynchronizer->pauseFileWatching();
    }
    else
    {
        QTemporaryFile* temporaryFile = new QTemporaryFile;
        isWorkFileOk = temporaryFile->open();

        mWorkFilePath = temporaryFile->fileName();
        mFile = temporaryFile;
    }

    if( isWorkFileOk )
        q->startWriteToFile();
    else
    {
        q->setError( KJob::KilledJobError );
        q->setErrorText( mFile->errorString() );
        delete mFile;
        q->emitResult();
    }
}

void AbstractFileSystemSyncToRemoteJobPrivate::completeWrite( bool success )
{
    Q_Q( AbstractFileSystemSyncToRemoteJob );

    if( success )
    {
        mFile->close(); // TODO: when is new time written, on close?
        QFileInfo fileInfo( *mFile );
        mSynchronizer->setFileDateTimeOnSync( fileInfo.lastModified() );

        const QUrl url = mSynchronizer->url();
        const bool isLocalFile = url.isLocalFile();

        if( ! isLocalFile )
        {
            KIO::FileCopyJob* fileCopyJob =
                KIO::file_copy( QUrl::fromLocalFile(mWorkFilePath), url, -1, KIO::Overwrite );
            KJobWidgets::setWindow( fileCopyJob, /*mWidget*/0 );

            success = fileCopyJob->exec();
            if( ! success )
            {
                q->setError( KJob::KilledJobError );
                q->setErrorText( fileCopyJob->errorString() );
            }
            else
                mSynchronizer->setRemoteState( RemoteUnknown );
        }
        else
        {
            mSynchronizer->unpauseFileWatching();
            mSynchronizer->setRemoteState( RemoteInSync );
        }
    }
    else
    {
        q->setError( KJob::KilledJobError );
        q->setErrorText( mFile->errorString() );
    }

    delete mFile;

    q->emitResult();
}

AbstractFileSystemSyncToRemoteJobPrivate::~AbstractFileSystemSyncToRemoteJobPrivate()
{
}

}
