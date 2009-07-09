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

#include "abstractfilesystemconnectjob.h"

// library
#include "abstractmodelfilesystemsynchronizer.h"
#include <kabstractdocument.h>
// KDE
#include <KIO/NetAccess>
#include <KTemporaryFile>
#include <KLocale>
#include <KDirWatch>
// Qt
#include <QtCore/QTimer>


namespace Kasten
{

class AbstractFileSystemConnectJob::Private
{
  public:
    Private( AbstractModelFileSystemSynchronizer* synchronizer, KAbstractDocument* document,
             const KUrl &url, AbstractModelSynchronizer::ConnectOption option );

  public:
    void setTemporaryFile( KTemporaryFile *temporaryFile );
    void setWorkFilePath( const QString &workFilePath );

  public:
    KAbstractDocument *document() const;
    KUrl url() const;
    AbstractModelSynchronizer::ConnectOption option() const;
    KTemporaryFile *temporaryFile() const;
    QString workFilePath() const;
    QWidget *widget() const;
    AbstractModelFileSystemSynchronizer* synchronizer() const;

  protected:
    AbstractModelFileSystemSynchronizer* mSynchronizer;
    KAbstractDocument *mDocument;
    const KUrl mUrl;
    const AbstractModelSynchronizer::ConnectOption mOption;
    KTemporaryFile *mTemporaryFile;
    QString mWorkFilePath;
};

AbstractFileSystemConnectJob::Private::Private( AbstractModelFileSystemSynchronizer* synchronizer,
                                                KAbstractDocument *document,
                                                const KUrl &url, AbstractModelSynchronizer::ConnectOption option )
 : mSynchronizer( synchronizer ), mDocument( document ), mUrl( url ), mOption( option ), mTemporaryFile( 0 )
{}

AbstractModelFileSystemSynchronizer* AbstractFileSystemConnectJob::Private::synchronizer() const
{
    return mSynchronizer;
}
inline KAbstractDocument *AbstractFileSystemConnectJob::Private::document()   const { return mDocument; }
inline KUrl AbstractFileSystemConnectJob::Private::url()               const { return mUrl; }
inline KTemporaryFile *AbstractFileSystemConnectJob::Private::temporaryFile() const { return mTemporaryFile; }
inline QString AbstractFileSystemConnectJob::Private::workFilePath()          const { return mWorkFilePath; }
// TODO: setup a notification system
inline QWidget *AbstractFileSystemConnectJob::Private::widget()               const { return 0; }
inline AbstractModelSynchronizer::ConnectOption AbstractFileSystemConnectJob::Private::option() const
{
    return mOption;
}

inline void AbstractFileSystemConnectJob::Private::setTemporaryFile( KTemporaryFile *temporaryFile )
{
    mTemporaryFile = temporaryFile;
}

inline void AbstractFileSystemConnectJob::Private::setWorkFilePath( const QString &workFilePath )
{
    mWorkFilePath = workFilePath;
}


AbstractFileSystemConnectJob::AbstractFileSystemConnectJob( AbstractModelFileSystemSynchronizer* synchronizer,
                                                            KAbstractDocument *document,
                                                      const KUrl &url, AbstractModelSynchronizer::ConnectOption option )
 : d( new Private(synchronizer,document,url,option) )
{}

AbstractModelFileSystemSynchronizer* AbstractFileSystemConnectJob::synchronizer() const
{
    return d->synchronizer();
}
KAbstractDocument *AbstractFileSystemConnectJob::document()   const { return d->document(); }
QString AbstractFileSystemConnectJob::workFilePath() const { return d->workFilePath(); }
QWidget *AbstractFileSystemConnectJob::widget() const { return d->widget(); }

void AbstractFileSystemConnectJob::start()
{
    QTimer::singleShot( 0, this, SLOT(connectWithFile()) );
}

void AbstractFileSystemConnectJob::connectWithFile()
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
        startConnectWithFile();
    }
    else
    {
        setError( KilledJobError );
        setErrorText( KIO::NetAccess::lastErrorString() );
        // TODO: should we rather skip setDocument in the API?
        emitResult();
    }
}

void AbstractFileSystemConnectJob::complete( bool success )
{
    AbstractModelFileSystemSynchronizer* synchronizer = d->synchronizer();
    if( success )
    {
        KDirWatch *dirWatch = KDirWatch::self();
        const KUrl newUrl = d->url();
        const KUrl oldUrl = synchronizer->url();
        // care for old url
        if( oldUrl.isLocalFile() )
        {
            dirWatch->disconnect( synchronizer );
            dirWatch->removeFile( oldUrl.path() );
        }

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
        // TODO; in path of both constructor by url and synchWithRemote
        // only needed for the first, so constructor writers can forget about this
        // for now we just check in setSynchronizer that new != old before deleting old
        d->document()->setSynchronizer( synchronizer );
    }
    else
    {
        delete synchronizer;
        // TODO: these reports should go to a notification system, for log or popup
        setError( KilledJobError );
        setErrorText( i18nc("@info","Problem while synching with local filesystem.") );
    }

    KIO::NetAccess::removeTempFile( d->workFilePath() );
    delete d->temporaryFile();

    emitResult();
}


AbstractFileSystemConnectJob::~AbstractFileSystemConnectJob()
{
    delete d;
}

}
