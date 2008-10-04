/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractfilesystemsyncfromremotejob.h"

// library
#include "abstractmodelfilesystemsynchronizer.h"
#include <kabstractdocument.h>
// KDE
#include <KIO/NetAccess>
#include <KTemporaryFile>
#include <KLocale>
#include <KMessageBox>
#include <KDirWatch>
// Qt
#include <QtCore/QTimer>


class AbstractFileSystemSyncFromRemoteJob::Private
{
  public:
    Private( AbstractModelFileSystemSynchronizer* synchronizer );

  public:
    void setWorkFilePath( const QString &workFilePath );

  public:
    KUrl url() const;
    QString workFilePath() const;
    QWidget *widget() const;
    AbstractModelFileSystemSynchronizer* synchronizer() const;

  protected:
    AbstractModelFileSystemSynchronizer* mSynchronizer;
    QString mWorkFilePath;
};

AbstractFileSystemSyncFromRemoteJob::Private::Private( AbstractModelFileSystemSynchronizer* synchronizer )
 : mSynchronizer( synchronizer )
{}

inline KUrl AbstractFileSystemSyncFromRemoteJob::Private::url()      const { return mSynchronizer->url(); }
inline QString AbstractFileSystemSyncFromRemoteJob::Private::workFilePath() const { return mWorkFilePath; }
// TODO: setup a notification system
inline QWidget *AbstractFileSystemSyncFromRemoteJob::Private::widget()      const { return 0; }
inline AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncFromRemoteJob::Private::synchronizer() const
{
    return mSynchronizer;
}

inline void AbstractFileSystemSyncFromRemoteJob::Private::setWorkFilePath( const QString &workFilePath )
{
    mWorkFilePath = workFilePath;
}



AbstractFileSystemSyncFromRemoteJob::AbstractFileSystemSyncFromRemoteJob( AbstractModelFileSystemSynchronizer* synchronizer )
 : d( new Private(synchronizer) )
{}

AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncFromRemoteJob::synchronizer() const
{
    return d->synchronizer();
}
QString AbstractFileSystemSyncFromRemoteJob::workFilePath() const { return d->workFilePath(); }
QWidget *AbstractFileSystemSyncFromRemoteJob::widget() const { return d->widget(); }

void AbstractFileSystemSyncFromRemoteJob::start()
{
    QTimer::singleShot( 0, this, SLOT(syncFromRemote()) );
}

void AbstractFileSystemSyncFromRemoteJob::syncFromRemote()
{
    QString workFilePath;
    // TODO: see if this could be used asynchronously instead
    if( KIO::NetAccess::download(d->url().url(),workFilePath,d->widget()) )
    {
        d->setWorkFilePath( workFilePath );
        startReadFromFile();
    }
    else
    {
        setError( KilledJobError );
        setErrorText( KIO::NetAccess::lastErrorString() );
        emitResult();
    }
}


void AbstractFileSystemSyncFromRemoteJob::completeRead( bool success )
{
    if( !success )
    {
        setError( KilledJobError );
        setErrorText( i18nc("@info","Problem when loading from local filesystem.") );
    }

    KIO::NetAccess::removeTempFile( d->workFilePath() );

    emitResult();
}


AbstractFileSystemSyncFromRemoteJob::~AbstractFileSystemSyncFromRemoteJob()
{
    delete d;
}

#include "abstractfilesystemsyncfromremotejob.moc"
