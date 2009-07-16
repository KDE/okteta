/*
    This file is part of the Kasten Framework, part of the KDE project.

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

#include "abstractfilesystemsyncwithremotejob.h"

// library
#include "abstractmodelfilesystemsynchronizer.h"
#include <abstractdocument.h>
// KDE
#include <KIO/NetAccess>
#include <KTemporaryFile>
#include <KLocale>
#include <KDirWatch>
// Qt
#include <QtCore/QTimer>


namespace Kasten
{

class AbstractFileSystemSyncWithRemoteJob::Private
{
  public:
    Private( AbstractModelFileSystemSynchronizer* synchronizer,
             const KUrl& url, AbstractModelSynchronizer::ConnectOption option );

  public:
    void setTemporaryFile( KTemporaryFile *temporaryFile );
    void setWorkFilePath( const QString &workFilePath );

  public:
    KUrl url() const;
    AbstractModelSynchronizer::ConnectOption option() const;
    KTemporaryFile *temporaryFile() const;
    QString workFilePath() const;
    QWidget* widget() const;
    AbstractModelFileSystemSynchronizer* synchronizer() const;

  protected:
    AbstractModelFileSystemSynchronizer* mSynchronizer;
    const KUrl mUrl;
    const AbstractModelSynchronizer::ConnectOption mOption;
    KTemporaryFile *mTemporaryFile;
    QString mWorkFilePath;
};

AbstractFileSystemSyncWithRemoteJob::Private::Private( AbstractModelFileSystemSynchronizer* synchronizer,
                                         const KUrl &url, AbstractModelSynchronizer::ConnectOption option )
 : mSynchronizer( synchronizer ), mUrl( url ), mOption( option ), mTemporaryFile( 0 )
{}

inline KUrl AbstractFileSystemSyncWithRemoteJob::Private::url()             const { return mUrl; }
inline KTemporaryFile *AbstractFileSystemSyncWithRemoteJob::Private::temporaryFile() const { return mTemporaryFile; }
inline QString AbstractFileSystemSyncWithRemoteJob::Private::workFilePath() const { return mWorkFilePath; }
// TODO: setup a notification system
inline QWidget* AbstractFileSystemSyncWithRemoteJob::Private::widget()      const { return 0; }
inline AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncWithRemoteJob::Private::synchronizer() const
{
    return mSynchronizer;
}
inline AbstractModelSynchronizer::ConnectOption AbstractFileSystemSyncWithRemoteJob::Private::option() const
{
    return mOption;
}

inline void AbstractFileSystemSyncWithRemoteJob::Private::setTemporaryFile( KTemporaryFile *temporaryFile )
{
    mTemporaryFile = temporaryFile;
}
inline void AbstractFileSystemSyncWithRemoteJob::Private::setWorkFilePath( const QString &workFilePath )
{
    mWorkFilePath = workFilePath;
}



AbstractFileSystemSyncWithRemoteJob::AbstractFileSystemSyncWithRemoteJob( AbstractModelFileSystemSynchronizer* synchronizer,
                                         const KUrl &url, AbstractModelSynchronizer::ConnectOption option )
 : d( new Private(synchronizer,url,option) )
{}

AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncWithRemoteJob::synchronizer() const
{
    return d->synchronizer();
}
QString AbstractFileSystemSyncWithRemoteJob::workFilePath() const { return d->workFilePath(); }
QWidget* AbstractFileSystemSyncWithRemoteJob::widget() const { return d->widget(); }

void AbstractFileSystemSyncWithRemoteJob::start()
{
    QTimer::singleShot( 0, this, SLOT(syncWithRemote()) );
}

void AbstractFileSystemSyncWithRemoteJob::syncWithRemote()
{
// Comment: here we play tricks to reuse the temporary file
// KIO::NetAccess::removeTempFile only removes tempfiles created by KIO::NetAccess::download
// So if replaceRemote and workFilePath is temporaryFile both don't conflict -> no problem (now)

    QString workFilePath;

    const KUrl newUrl = d->url();
    bool isWorkFileOk;
    if( d->option() == AbstractModelSynchronizer::ReplaceRemote )
    {
        if( newUrl.isLocalFile() )
            workFilePath = newUrl.path();
        else
        {
            KTemporaryFile *temporaryFile = new KTemporaryFile;
            temporaryFile->open();
            workFilePath = temporaryFile->fileName();
            d->setTemporaryFile( temporaryFile );
        }
        isWorkFileOk = true;
    }
    else
        isWorkFileOk = KIO::NetAccess::download( newUrl.url(), workFilePath, d->widget() );

    if( isWorkFileOk )
    {
        d->setWorkFilePath( workFilePath );
        startSyncWithRemote();
    }
    else
    {
        setError( KilledJobError );
        setErrorText( KIO::NetAccess::lastErrorString() );
        // TODO: should we rather skip completeSync in successthe API?
        emitResult();
    }
}


void AbstractFileSystemSyncWithRemoteJob::completeSync( bool success )
{
    AbstractModelFileSystemSynchronizer* synchronizer = d->synchronizer();
    if( success )
    {
        KDirWatch *dirWatch = KDirWatch::self();
        const KUrl oldUrl = synchronizer->url();
        // care for old url
        if( oldUrl.isLocalFile() )
        {
            dirWatch->disconnect( synchronizer );
            dirWatch->removeFile( oldUrl.path() );
        }
        const KUrl newUrl = d->url();
        if( !newUrl.isLocalFile() )
        {
            const bool uploaded = KIO::NetAccess::upload( workFilePath(), newUrl, d->widget() );
            if( !uploaded )
            {
                setError( KilledJobError );
                setErrorText( KIO::NetAccess::lastErrorString() );
            }
        }
        else
        {
            connect( dirWatch, SIGNAL(dirty( const QString & )),
                     synchronizer, SLOT(onFileDirty( const QString & )) );

            connect( dirWatch, SIGNAL(created( const QString & )),
                     synchronizer, SLOT(onFileCreated( const QString & )) );

            connect( dirWatch, SIGNAL(deleted( const QString & )),
                     synchronizer, SLOT(onFileDeleted( const QString & )) );
            dirWatch->addFile( workFilePath() );
        }
        synchronizer->setUrl( newUrl );
    }
    else
    {
        setError( KilledJobError );
        setErrorText( i18nc("@info","Problem while synching with local filesystem.") );
    }

    KIO::NetAccess::removeTempFile( d->workFilePath() );
    delete d->temporaryFile();

    emitResult();
}


AbstractFileSystemSyncWithRemoteJob::~AbstractFileSystemSyncWithRemoteJob()
{
    delete d;
}

}
