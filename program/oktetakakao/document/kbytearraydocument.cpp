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
#include <kbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QLatin1String>


KByteArrayDocument::KByteArrayDocument()
: mByteArray( new KHECore::KByteArrayModel() )
{
    connect( mByteArray, SIGNAL(modificationChanged( bool )), SLOT(onModelModification( bool )) );
}

KByteArrayDocument::KByteArrayDocument( KHECore::KByteArrayModel *byteArray )
: mByteArray( byteArray )
{
    connect( mByteArray, SIGNAL(modificationChanged( bool )), SLOT(onModelModification( bool )) );
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

KAbstractDocument::SynchronizationStates KByteArrayDocument::synchronizationStates() const
{
    return mByteArray->isModified() ? LocalHasChanges : InSync;
}

void KByteArrayDocument::setTitle( const QString &title )
{
    mTitle = title;
    emit titleChanged( mTitle );
}


void KByteArrayDocument::onModelModification( bool newState )
{
    emit modified( newState ? LocalHasChanges : InSync );
}

KByteArrayDocument::~KByteArrayDocument()
{
    delete mByteArray;
}

#include "kbytearraydocument.moc"
