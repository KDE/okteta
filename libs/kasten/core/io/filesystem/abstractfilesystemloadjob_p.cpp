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

#include "abstractfilesystemloadjob_p.h"

// library
#include "abstractmodelfilesystemsynchronizer.h"
#include <abstractdocument.h>
// KDE
#include <KIO/NetAccess>
#include <KLocale>
#include <KDirWatch>


namespace Kasten
{

void AbstractFileSystemLoadJobPrivate::load()
{
    Q_Q( AbstractFileSystemLoadJob );

    // TODO: see if this could be used asynchronously instead
    const bool success = KIO::NetAccess::download( mUrl.url(), mWorkFilePath, widget() );

    if( success )
    {
        q->startLoadFromFile();
    }
    else
    {
        q->setError( KJob::KilledJobError );
        q->setErrorText( KIO::NetAccess::lastErrorString() );
        // TODO: should we rather skip setDocument in the API?
        q->AbstractLoadJob::setDocument( 0 );
    }
}


void AbstractFileSystemLoadJobPrivate::setDocument( AbstractDocument* document )
{
    Q_Q( AbstractFileSystemLoadJob );

    if( document )
    {
        mSynchronizer->setUrl( mUrl );
        if( mUrl.isLocalFile() )
        {
            KDirWatch* dirWatch = new KDirWatch( mSynchronizer );
            QObject::connect( dirWatch, SIGNAL(dirty( const QString& )),
                     mSynchronizer, SLOT(onFileDirty( const QString& )) );

            QObject::connect( dirWatch, SIGNAL(created( const QString& )),
                     mSynchronizer, SLOT(onFileCreated( const QString& )) );

            QObject::connect( dirWatch, SIGNAL(deleted( const QString& )),
                     mSynchronizer, SLOT(onFileDeleted( const QString& )) );
            dirWatch->addFile( mUrl.path() );
        }
        document->setSynchronizer( mSynchronizer );
    }
    else
    {
        delete mSynchronizer;
        // TODO: these reports should go to a notification system, for log or popup
        q->setError( KJob::KilledJobError );
        q->setErrorText( i18nc("@info","Problem while loading from local filesystem.") );
    }

    KIO::NetAccess::removeTempFile( mWorkFilePath );

    q->AbstractLoadJob::setDocument( document );
}


}
