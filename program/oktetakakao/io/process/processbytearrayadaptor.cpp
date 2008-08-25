/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "processbytearrayadaptor.h"

// lib
#include <kbytearraydocument.h>
// Okteta core
#include <changehistory.h>
#include <kversionable.h>
#include <kabstractbytearraymodel.h>
// Qt
#include <QtCore/QByteArray>


ProcessByteArrayAdaptor::ProcessByteArrayAdaptor( KByteArrayDocument* byteArrayDocument )
 : mByteArrayDocument( byteArrayDocument )
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    if( byteArrayModel )
    {
        connect( byteArrayModel, SIGNAL(changesDone( const QList<KHECore::ByteArrayChange>&, int, int )),
                 SIGNAL(changesDone( const QList<KHECore::ByteArrayChange>&, int, int )) );
        connect( byteArrayModel, SIGNAL(revertedToVersionIndex( int )),
                 SIGNAL(revertedToVersionIndex( int )) );
    }
}

QString ProcessByteArrayAdaptor::title() const { return mByteArrayDocument->title(); }
QString ProcessByteArrayAdaptor::id()    const { return mByteArrayDocument->id(); }
int ProcessByteArrayAdaptor::versionIndex() const { return mByteArrayDocument->versionIndex(); }
int ProcessByteArrayAdaptor::versionCount() const { return mByteArrayDocument->versionCount(); }

QList<KHECore::ByteArrayChange> ProcessByteArrayAdaptor::changes( int firstVersionIndex, int lastVersionIndex ) const
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    KHECore::ChangeHistory* changeHistory =
        byteArrayModel ? qobject_cast<KHECore::ChangeHistory*>( byteArrayModel ) : 0;

    return changeHistory ? changeHistory->changes(firstVersionIndex,lastVersionIndex) : QList<KHECore::ByteArrayChange>();
}

void ProcessByteArrayAdaptor::onChangesDone( const QList<KHECore::ByteArrayChange>& changes,
                                             int oldVersionIndex, int newVersionIndex )
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    KHECore::ChangeHistory* changeHistory =
        byteArrayModel ? qobject_cast<KHECore::ChangeHistory*>( byteArrayModel ) : 0;
    if( changeHistory )
        changeHistory->doChanges( changes, oldVersionIndex, newVersionIndex );
}

void ProcessByteArrayAdaptor::onRevertedToVersionIndex( int versionIndex )
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument ? mByteArrayDocument->content() : 0;
    KHECore::Versionable* versionable =
        byteArrayModel ? qobject_cast<KHECore::Versionable*>( byteArrayModel ) : 0;
    if( versionable )
        versionable->revertToVersionByIndex( versionIndex );
}

QByteArray ProcessByteArrayAdaptor::baseData() const
{
    KHECore::KAbstractByteArrayModel* byteArrayModel = mByteArrayDocument->content();
    KHECore::ChangeHistory* changeHistory =
        byteArrayModel ? qobject_cast<KHECore::ChangeHistory*>( byteArrayModel ) : 0;

    return changeHistory ? changeHistory->initialData() : QByteArray();
}
