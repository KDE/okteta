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
#include <KUrl>
// Qt
#include <QtCore/QFile>
#include <QtCore/QDataStream>

#include <KDebug>

KByteArrayDocument::KByteArrayDocument()
: mByteArray( new KHECore::KByteArrayModel() )
{
    connect( mByteArray, SIGNAL(modificationChanged( bool )), SLOT(onModelModification( bool )) );
}

KByteArrayDocument::KByteArrayDocument( const QString &filePath )
: mByteArray( new KHECore::KByteArrayModel() )
{
    if( load(filePath) )
    {
        mLocalFilePath = filePath;
    }
}

KHECore::KByteArrayModel* KByteArrayDocument::content() const { return mByteArray; }
QString KByteArrayDocument::url() const { return mUrl; }
QString KByteArrayDocument::localFilePath() const {  return mLocalFilePath; }

QString KByteArrayDocument::title() const
{
    return mTitle;
}

KAbstractDocument::SynchronizationStates KByteArrayDocument::synchronizationStates() const
{
    return mByteArray->isModified() ? LocalHasChanges : InSync;
}

void KByteArrayDocument::setTitle( const QString &title )
{
    mTitle = title;
    emit titleChanged( mTitle );
}

void KByteArrayDocument::setUrl( const QString &url )
{
    mUrl = url;
    setTitle( KUrl(mUrl).fileName() );
}

bool KByteArrayDocument::load( const QString &fileName )
{
    QFile file( fileName );
    file.open( QIODevice::ReadOnly );
    int fileSize = file.size();

    delete mByteArray;
    mByteArray = new KHECore::KByteArrayModel( fileSize );
    connect( mByteArray, SIGNAL(modificationChanged( bool )), SLOT(onModelModification( bool )) );

    QDataStream inStream( &file );
    inStream.readRawData( mByteArray->data(), fileSize );

    mByteArray->setModified( false );

    //registerDiskModifyTime( file ); TODO move into synchronizer

    return inStream.status() == QDataStream::Ok;
}


bool KByteArrayDocument::save( const QString &fileName )
{
    QFile file( fileName );
    file.open( QIODevice::WriteOnly );

    QDataStream outStream( &file );
    outStream.writeRawData( mByteArray->data(), mByteArray->size() );

    mByteArray->setModified( false );

    //registerDiskModifyTime( file );TODO move into synchronizer

    return outStream.status() == QDataStream::Ok;
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
