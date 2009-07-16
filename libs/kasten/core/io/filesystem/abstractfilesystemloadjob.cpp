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

#include "abstractfilesystemloadjob.h"

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

class AbstractFileSystemLoadJob::Private
{
  public:
    Private( AbstractModelFileSystemSynchronizer* synchronizer, const KUrl& url );

  public:
    void setWorkFilePath( const QString &workFilePath );

  public:
    const KUrl &url() const;
    QString workFilePath() const;
    QWidget* widget() const;
    AbstractModelFileSystemSynchronizer* synchronizer() const;

  protected:
    AbstractModelFileSystemSynchronizer* mSynchronizer;
    const KUrl mUrl;
    QString mWorkFilePath;
};

AbstractFileSystemLoadJob::Private::Private( AbstractModelFileSystemSynchronizer* synchronizer, const KUrl &url )
 : mSynchronizer( synchronizer ), mUrl( url )
{}

inline AbstractModelFileSystemSynchronizer* AbstractFileSystemLoadJob::Private::synchronizer() const
{
    return mSynchronizer;
}
inline const KUrl &AbstractFileSystemLoadJob::Private::url()      const { return mUrl; }
inline QString AbstractFileSystemLoadJob::Private::workFilePath() const { return mWorkFilePath; }
// TODO: setup a notification system
inline QWidget* AbstractFileSystemLoadJob::Private::widget()      const { return 0; }

inline void AbstractFileSystemLoadJob::Private::setWorkFilePath( const QString &workFilePath )
{
    mWorkFilePath = workFilePath;
}



AbstractFileSystemLoadJob::AbstractFileSystemLoadJob( AbstractModelFileSystemSynchronizer* synchronizer,
                                                      const KUrl &url )
 : d( new Private(synchronizer,url) )
{}

AbstractModelFileSystemSynchronizer* AbstractFileSystemLoadJob::synchronizer() const
{
    return d->synchronizer();
}
QString AbstractFileSystemLoadJob::workFilePath() const { return d->workFilePath(); }
QWidget* AbstractFileSystemLoadJob::widget() const { return d->widget(); }

void AbstractFileSystemLoadJob::start()
{
    QTimer::singleShot( 0, this, SLOT(load()) );
}

void AbstractFileSystemLoadJob::load()
{
    QString workFilePath;
    // TODO: see if this could be used asynchronously instead
    if( KIO::NetAccess::download(d->url().url(),workFilePath,d->widget()) )
    {
        d->setWorkFilePath( workFilePath );
        startLoadFromFile();
    }
    else
    {
        setError( KilledJobError );
        setErrorText( KIO::NetAccess::lastErrorString() );
        // TODO: should we rather skip setDocument in the API?
        AbstractLoadJob::setDocument( 0 );
    }
}


void AbstractFileSystemLoadJob::setDocument( AbstractDocument* document )
{
    AbstractModelFileSystemSynchronizer* synchronizer = d->synchronizer();

    if( document )
    {
        synchronizer->setUrl( d->url() );
        if( d->url().isLocalFile() )
        {
            KDirWatch *dirWatch = KDirWatch::self();
            connect( dirWatch, SIGNAL(dirty( const QString & )),
                     synchronizer, SLOT(onFileDirty( const QString & )) );

            connect( dirWatch, SIGNAL(created( const QString & )),
                     synchronizer, SLOT(onFileCreated( const QString & )) );

            connect( dirWatch, SIGNAL(deleted( const QString & )),
                     synchronizer, SLOT(onFileDeleted( const QString & )) );
            dirWatch->addFile( d->url().path() );
        }
        document->setSynchronizer( synchronizer );
    }
    else
    {
        delete synchronizer;
        // TODO: these reports should go to a notification system, for log or popup
        setError( KilledJobError );
        setErrorText( i18nc("@info","Problem while loading from local filesystem.") );
    }

    KIO::NetAccess::removeTempFile( d->workFilePath() );

    AbstractLoadJob::setDocument( document );
}


AbstractFileSystemLoadJob::~AbstractFileSystemLoadJob()
{
    delete d;
}

}
