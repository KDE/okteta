/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "bytearrayrawfileloadjob.h"

// lib
// #include "externalbookmarkstorage.h"
#include "bytearrayrawfilesynchronizer.h"
#include "bytearrayrawfileloadthread.h"
#include "bytearraydocument.h"
// KDE
#include <KUrl>
// Qt
#include <QtCore/QCoreApplication>


namespace Kasten2
{

ByteArrayRawFileLoadJob::ByteArrayRawFileLoadJob( ByteArrayRawFileSynchronizer *synchronizer, const KUrl &url )
 : AbstractFileSystemLoadJob( synchronizer, url )
{}

void ByteArrayRawFileLoadJob::startLoadFromFile()
{
    ByteArrayRawFileLoadThread *loadThread = new ByteArrayRawFileLoadThread( this, file() );
    loadThread->start();
    while( !loadThread->wait(100) )
        QCoreApplication::processEvents( QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers, 100 );

    ByteArrayDocument* document = loadThread->document();
    qobject_cast<ByteArrayRawFileSynchronizer*>(synchronizer())->setDocument( document );

    if( document )
//         ExternalBookmarkStorage().readBookmarks( document, url() );
        {}
    else
    {
        // TODO: these reports should go to a notification system, for log or popup
        setError( KJob::KilledJobError );
        setErrorText( loadThread->errorString() );
    }

    delete loadThread;

    setDocument( document );
}

ByteArrayRawFileLoadJob::~ByteArrayRawFileLoadJob() {}

}
