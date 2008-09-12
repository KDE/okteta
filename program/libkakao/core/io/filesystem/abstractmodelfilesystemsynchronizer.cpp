/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#include "abstractmodelfilesystemsynchronizer.h"

// lib
#include <kabstractdocument.h>
// KDE
#include <KIO/NetAccess>
#include <KTemporaryFile>
#include <KLocale>
#include <KMessageBox>
#include <KDirWatch>

class AbstractModelFileSystemSynchronizer::Private
{
  public:
    Private();
  public:
    QWidget* widget() const;
    void setWidget( QWidget* widget );
  protected:
    QWidget* mWidget;
};
inline AbstractModelFileSystemSynchronizer::Private::Private() : mWidget( 0 ) {}
inline QWidget* AbstractModelFileSystemSynchronizer::Private::widget() const { return mWidget; }
inline void AbstractModelFileSystemSynchronizer::Private::setWidget( QWidget* widget) { mWidget = widget; }


AbstractModelFileSystemSynchronizer::AbstractModelFileSystemSynchronizer()
 : d( new Private() )
{}


void AbstractModelFileSystemSynchronizer::onFileDirty( const QString& fileName )
{
  Q_UNUSED( fileName )
//     if( url().url() == fileName )
//         document()->setRemoteHasChanges(); TODO: needs a control interface? 
}
void AbstractModelFileSystemSynchronizer::onFileCreated( const QString& fileName )
{
  Q_UNUSED( fileName )
  //TODO: could happen after a delete, what to do?
}
void AbstractModelFileSystemSynchronizer::onFileDeleted( const QString& fileName )
{
  Q_UNUSED( fileName )
//     if( url().url() == fileName )
//         document()->setRemoteHasChanges(); TODO: needs a control interface? 
}

AbstractModelFileSystemSynchronizer::~AbstractModelFileSystemSynchronizer()
{
    if( url().isLocalFile() )
        KDirWatch::self()->removeFile( url().path() );

    delete d;
}
