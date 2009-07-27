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

#include "kbytearraydocument.h"

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

KByteArrayDocument::KByteArrayDocument( const QString &initDescription )
: mByteArray( new Okteta::PieceTableByteArrayModel() ),
  mInitDescription( initDescription )
{
    connect( mByteArray, SIGNAL(modificationChanged( bool )), SLOT(onModelModification( bool )) );
    connect( mByteArray, SIGNAL(readOnlyChanged( bool )), SIGNAL(readOnlyChanged( bool )) );
    connect( mByteArray, SIGNAL(revertedToVersionIndex( int )), SIGNAL(revertedToVersionIndex( int )) );
    connect( mByteArray, SIGNAL(headVersionChanged( int )), SIGNAL(headVersionChanged( int )) );
    connect( mByteArray, SIGNAL(headVersionDescriptionChanged( const QString& )),
             SLOT(onHeadVersionDescriptionChanged( const QString& )) );
}

KByteArrayDocument::KByteArrayDocument( Okteta::PieceTableByteArrayModel *byteArray, const QString &initDescription )
: mByteArray( byteArray ),
  mInitDescription( initDescription )
{
    connect( mByteArray, SIGNAL(modificationChanged( bool )), SLOT(onModelModification( bool )) );
    connect( mByteArray, SIGNAL(readOnlyChanged( bool )), SIGNAL(readOnlyChanged( bool )) );
    connect( mByteArray, SIGNAL(revertedToVersionIndex( int )), SIGNAL(revertedToVersionIndex( int )) );
    connect( mByteArray, SIGNAL(headVersionChanged( int )), SIGNAL(headVersionChanged( int )) );
    connect( mByteArray, SIGNAL(headVersionDescriptionChanged( const QString& )),
             SLOT(onHeadVersionDescriptionChanged( const QString& )) );
}
#if 0
KByteArrayDocument::KByteArrayDocument( const QString &filePath )
: mByteArray( new Okteta::ByteArrayModel() )
{
    if( load(filePath) )
    {
        mLocalFilePath = filePath;
    }
}
#endif
Okteta::AbstractByteArrayModel* KByteArrayDocument::content() const { return mByteArray; }

QString KByteArrayDocument::title() const { return mTitle; }
QString KByteArrayDocument::mimeType() const { return QLatin1String("KByteArrayDocument"); }
QString KByteArrayDocument::typeName() const { return i18nc( "name of the data type", "Byte Array" ); }

bool KByteArrayDocument::isModifiable() const { return true; }
bool KByteArrayDocument::isReadOnly()   const { return mByteArray->isReadOnly(); }
void KByteArrayDocument::setReadOnly( bool isReadOnly ) { mByteArray->setReadOnly( isReadOnly ); }

AbstractDocument::SynchronizationStates KByteArrayDocument::synchronizationStates() const
{
    return mByteArray->isModified() ? LocalHasChanges : InSync;
}

void KByteArrayDocument::setTitle( const QString &title )
{
    mTitle = title;
    emit titleChanged( mTitle );
}


int KByteArrayDocument::versionIndex() const { return mByteArray->versionIndex(); }
int KByteArrayDocument::versionCount() const { return mByteArray->versionCount(); }
DocumentVersionData KByteArrayDocument::versionData( int versionIndex ) const
{
    const QString changeComment = ( versionIndex == 0 ) ? mInitDescription : mByteArray->versionDescription(versionIndex);
    return DocumentVersionData( versionIndex, changeComment );
}

void KByteArrayDocument::revertToVersionByIndex( int versionIndex ) { mByteArray->revertToVersionByIndex( versionIndex ); }

void KByteArrayDocument::onModelModification( bool newState )
{
    emit modified( newState ? LocalHasChanges : InSync );
}

void KByteArrayDocument::onHeadVersionDescriptionChanged( const QString &newDescription )
{
    const DocumentVersionData data( mByteArray->versionIndex(), newDescription );
    emit headVersionDataChanged( data );
}

Person KByteArrayDocument::owner() const
{
    return mUserList.size()>0 ? mUserList.at( 0 ) : Person();
}

QList<Person> KByteArrayDocument::userList() const
{
    return mUserList;
}

void KByteArrayDocument::setOwner( const Person& owner )
{
    mUserList.append( owner );
}

void KByteArrayDocument::addUsers( const QList<Person>& users )
{
    foreach( const Person& user, users )
        mUserList.append( user );

    emit usersAdded( users );
}

void KByteArrayDocument::removeUsers( const QList<Person>& users )
{
    foreach( const Person& user, users )
        mUserList.removeOne( user );

    emit usersRemoved( users );
}

KByteArrayDocument::~KByteArrayDocument()
{
    delete mByteArray;
}

}
