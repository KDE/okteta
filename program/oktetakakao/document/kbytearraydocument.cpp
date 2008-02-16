/***************************************************************************
                          kbytearraydocument.h  -  description
                             -------------------
    begin                : Fri Jun 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


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
QString KByteArrayDocument::typeName() const { return i18n( "Byte Array" ); }

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
