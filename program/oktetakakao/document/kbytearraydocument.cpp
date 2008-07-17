/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

// Okteta core
#include <kpiecetablebytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QLatin1String>


KByteArrayDocument::KByteArrayDocument( const QString &initDescription )
: mByteArray( new KHECore::KPieceTableByteArrayModel() ),
  mInitDescription( initDescription )
{
    connect( mByteArray, SIGNAL(modificationChanged( bool )), SLOT(onModelModification( bool )) );
    connect( mByteArray, SIGNAL(readOnlyChanged( bool )), SIGNAL(readOnlyChanged( bool )) );
    connect( mByteArray, SIGNAL(revertedToVersionIndex( int )), SIGNAL(revertedToVersionIndex( int )) );
    connect( mByteArray, SIGNAL(headVersionChanged( int )), SIGNAL(headVersionChanged( int )) );
    connect( mByteArray, SIGNAL(headVersionDescriptionChanged( const QString& )),
             SLOT(onHeadVersionDescriptionChanged( const QString& )) );
}

KByteArrayDocument::KByteArrayDocument( KHECore::KPieceTableByteArrayModel *byteArray, const QString &initDescription )
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
: mByteArray( new KHECore::KByteArrayModel() )
{
    if( load(filePath) )
    {
        mLocalFilePath = filePath;
    }
}
#endif
KHECore::KAbstractByteArrayModel* KByteArrayDocument::content() const { return mByteArray; }

QString KByteArrayDocument::title() const { return mTitle; }
QString KByteArrayDocument::mimeType() const { return QLatin1String("KByteArrayDocument"); }
QString KByteArrayDocument::typeName() const { return i18nc( "name of the data type", "Byte Array" ); }

bool KByteArrayDocument::isModifiable() const { return true; }
bool KByteArrayDocument::isReadOnly()   const { return mByteArray->isReadOnly(); }
void KByteArrayDocument::setReadOnly( bool isReadOnly ) { mByteArray->setReadOnly( isReadOnly ); }

KAbstractDocument::SynchronizationStates KByteArrayDocument::synchronizationStates() const
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
KDocumentVersionData KByteArrayDocument::versionData( int versionIndex ) const
{
    const QString changeComment = ( versionIndex == 0 ) ? mInitDescription : mByteArray->versionDescription(versionIndex);
    return KDocumentVersionData( 0, changeComment );
}

void KByteArrayDocument::revertToVersionByIndex( int versionIndex ) { mByteArray->revertToVersionByIndex( versionIndex ); }

void KByteArrayDocument::onModelModification( bool newState )
{
    emit modified( newState ? LocalHasChanges : InSync );
}

void KByteArrayDocument::onHeadVersionDescriptionChanged( const QString &newDescription )
{
    const KDocumentVersionData data( 0, newDescription );
    emit headVersionDataChanged( data );
}

KByteArrayDocument::~KByteArrayDocument()
{
    delete mByteArray;
}

#include "kbytearraydocument.moc"
