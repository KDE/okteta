/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractmodelfilesystemsynchronizer_p.h"

// KDE
#include <KIO/NetAccess>
#include <KTemporaryFile>
#include <KLocale>
#include <KMessageBox>
#include <KDirWatch>

#include <KDebug>
namespace Kasten
{

void AbstractModelFileSystemSynchronizerPrivate::onFileDirty( const QString& fileName )
{
  Q_UNUSED( fileName )
kDebug()<<fileName;
//     if( url().url() == fileName )
//         document()->setRemoteHasChanges(); TODO: needs a control interface? 
}

void AbstractModelFileSystemSynchronizerPrivate::onFileCreated( const QString& fileName )
{
  Q_UNUSED( fileName )
kDebug()<<fileName;
  //TODO: could happen after a delete, what to do?
}

void AbstractModelFileSystemSynchronizerPrivate::onFileDeleted( const QString& fileName )
{
  Q_UNUSED( fileName )
kDebug()<<fileName;
//     if( url().url() == fileName )
//         document()->setRemoteHasChanges(); TODO: needs a control interface? 
}

AbstractModelFileSystemSynchronizerPrivate::~AbstractModelFileSystemSynchronizerPrivate()
{
    if( url().isLocalFile() )
        KDirWatch::self()->removeFile( url().path() );
}

}
