/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraydocument.h"

//
#include <person.h>
// Okteta core
#include <piecetablebytearraymodel.h>
// KDE
#include <KLocale>
#include <KIcon>
// Qt
#include <QtCore/QLatin1String>


namespace Kasten
{

ByteArrayDocument::ByteArrayDocument( const QString &initDescription )
  : mByteArray( new Okteta::PieceTableByteArrayModel() ),
    mInitDescription( initDescription ),
    mRemoteState( InSync )
{
    connect( mByteArray, SIGNAL(modifiedChanged( bool )), SLOT(onModelModified( bool )) );
    connect( mByteArray, SIGNAL(readOnlyChanged( bool )), SIGNAL(readOnlyChanged( bool )) );
    connect( mByteArray, SIGNAL(revertedToVersionIndex( int )), SIGNAL(revertedToVersionIndex( int )) );
    connect( mByteArray, SIGNAL(headVersionChanged( int )), SIGNAL(headVersionChanged( int )) );
    connect( mByteArray, SIGNAL(headVersionDescriptionChanged( const QString& )),
             SLOT(onHeadVersionDescriptionChanged( const QString& )) );
}

ByteArrayDocument::ByteArrayDocument( Okteta::PieceTableByteArrayModel *byteArray, const QString &initDescription )
  : mByteArray( byteArray ),
    mInitDescription( initDescription ),
    mRemoteState( InSync )
{
    connect( mByteArray, SIGNAL(modifiedChanged( bool )), SLOT(onModelModified( bool )) );
    connect( mByteArray, SIGNAL(readOnlyChanged( bool )), SIGNAL(readOnlyChanged( bool )) );
    connect( mByteArray, SIGNAL(revertedToVersionIndex( int )), SIGNAL(revertedToVersionIndex( int )) );
    connect( mByteArray, SIGNAL(headVersionChanged( int )), SIGNAL(headVersionChanged( int )) );
    connect( mByteArray, SIGNAL(headVersionDescriptionChanged( const QString& )),
             SLOT(onHeadVersionDescriptionChanged( const QString& )) );
}
#if 0
ByteArrayDocument::ByteArrayDocument( const QString &filePath )
: mByteArray( new Okteta::ByteArrayModel() )
{
    if( load(filePath) )
    {
        mLocalFilePath = filePath;
    }
}
#endif
Okteta::AbstractByteArrayModel* ByteArrayDocument::content() const { return mByteArray; }

QString ByteArrayDocument::title() const { return mTitle; }
QString ByteArrayDocument::mimeType() const { return QLatin1String("ByteArrayDocument"); }
QString ByteArrayDocument::typeName() const { return i18nc( "name of the data type", "Byte Array" ); }

bool ByteArrayDocument::isModifiable() const { return true; }
bool ByteArrayDocument::isReadOnly()   const { return mByteArray->isReadOnly(); }
void ByteArrayDocument::setReadOnly( bool isReadOnly ) { mByteArray->setReadOnly( isReadOnly ); }

AbstractDocument::SyncStates ByteArrayDocument::syncStates() const
{
    return mRemoteState | (mByteArray->isModified() ? LocalHasChanges : InSync);
}

void ByteArrayDocument::setTitle( const QString &title )
{
    mTitle = title;
    emit titleChanged( mTitle );
}

void ByteArrayDocument::setRemoteState( AbstractDocument::SyncState remoteState )
{
    if( mRemoteState == remoteState )
        return;

    mRemoteState = remoteState;

    emit syncStatesChanged( syncStates() );
}


int ByteArrayDocument::versionIndex() const { return mByteArray->versionIndex(); }
int ByteArrayDocument::versionCount() const { return mByteArray->versionCount(); }
DocumentVersionData ByteArrayDocument::versionData( int versionIndex ) const
{
    const QString changeComment = ( versionIndex == 0 ) ? mInitDescription : mByteArray->versionDescription(versionIndex);
    return DocumentVersionData( versionIndex, changeComment );
}

void ByteArrayDocument::revertToVersionByIndex( int versionIndex ) { mByteArray->revertToVersionByIndex( versionIndex ); }

void ByteArrayDocument::onModelModified( bool isModified )
{
    emit syncStatesChanged( mRemoteState | (isModified ? LocalHasChanges : InSync) );
}

void ByteArrayDocument::onHeadVersionDescriptionChanged( const QString &newDescription )
{
    const DocumentVersionData data( mByteArray->versionIndex(), newDescription );
    emit headVersionDataChanged( data );
}

Person ByteArrayDocument::owner() const
{
    return mUserList.size()>0 ? mUserList.at( 0 ) : Person();
}

QList<Person> ByteArrayDocument::userList() const
{
    return mUserList;
}

void ByteArrayDocument::setOwner( const Person& owner )
{
    mUserList.append( owner );
}

void ByteArrayDocument::addUsers( const QList<Person>& users )
{
    foreach( const Person& user, users )
        mUserList.append( user );

    emit usersAdded( users );
}

void ByteArrayDocument::removeUsers( const QList<Person>& users )
{
    foreach( const Person& user, users )
        mUserList.removeOne( user );

    emit usersRemoved( users );
}

ByteArrayDocument::~ByteArrayDocument()
{
    delete mByteArray;
}

}
