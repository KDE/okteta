/***************************************************************************
                          kbytearrayrawfilesynchronizer.h  -  description
                             -------------------
    begin                : Mon Nov 12 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kbytearrayrawfilesynchronizer.h"

// lib
#include "kbytearraydocument.h"
// Okteta core
#include <kbytearraymodel.h>
// KDE
#include <KUrl>
// Qt
#include <QtCore/QDataStream>
#include <QtCore/QFile>

// TODO: this function seems to be always the same. Make macro
// or, if there is only one place which calls this, move there
KAbstractDocument *loadByteArrayFromRawFile( const KUrl &originUrl )
{
    KByteArrayRawFileSynchronizer *synchronizer = new KByteArrayRawFileSynchronizer( originUrl );
    KAbstractDocument *document = synchronizer->document();
    if( !document )
        delete synchronizer;

    return document;
}

bool synchByteArrayWithRawFile( KAbstractDocument *document, const KUrl &originUrl,
                                KAbstractDocumentSynchronizer::ConnectOption option )
{
    KByteArrayRawFileSynchronizer *synchronizer = new KByteArrayRawFileSynchronizer( document, originUrl, option );
    // TODO: is synchronizer->document() really a good signal for success? see also below
    const bool success = ( synchronizer->document() != 0 );
    if( !success )
        delete synchronizer;

    return success;
}

KByteArrayRawFileSynchronizer::KByteArrayRawFileSynchronizer( const KUrl &url )
{
    KAbstractDocument *document = loadFromUrl( url );
    mDocument = document ? qobject_cast<KByteArrayDocument*>( document ) : 0;
    if( mDocument )
        onUrlChange( url );
    connect( this, SIGNAL(urlChanged(const KUrl&)), SLOT(onUrlChange( const KUrl & )) );
}

KByteArrayRawFileSynchronizer::KByteArrayRawFileSynchronizer( KAbstractDocument *document, const KUrl &url,
                                                              KAbstractDocumentSynchronizer::ConnectOption option )
{
    // TODO: is synchronizer->document() really a good signal for success? see also above
    mDocument = document ? qobject_cast<KByteArrayDocument*>( document ) : 0;
    if( mDocument )
    {
        if( !synchWithUrl(url,option) )
            mDocument = 0;
    }
    if( mDocument )
        onUrlChange( url );
    connect( this, SIGNAL(urlChanged(const KUrl&)), SLOT(onUrlChange( const KUrl & )) );
}

KAbstractDocument *KByteArrayRawFileSynchronizer::document() const { return mDocument; }

KAbstractDocument *KByteArrayRawFileSynchronizer::loadFromFile( const QString &localFileName/*, int *success*/ )
{
    KByteArrayDocument *document = 0;

    QFile file( localFileName );
    file.open( QIODevice::ReadOnly );
    QDataStream inStream( &file );
    int fileSize = file.size();

    // TODO: should the decoder know this?
    // it is in the api now (constructor)
    KHECore::KByteArrayModel *byteArray = new KHECore::KByteArrayModel( fileSize );

    inStream.readRawData( byteArray->data(), fileSize );

    byteArray->setModified( false );

    //registerDiskModifyTime( file ); TODO move into synchronizer

    const bool streamIsOk = ( inStream.status() == QDataStream::Ok );
//     if( success )
//         *success = streamIsOk ? 0 : 1;
    if( streamIsOk )
        document = new KByteArrayDocument( byteArray );
    else
        delete byteArray;

    return document;
}

bool KByteArrayRawFileSynchronizer::reloadFromFile( const QString &localFileName )
{
    QFile file( localFileName );
    file.open( QIODevice::ReadOnly );
    QDataStream inStream( &file );
    int fileSize = file.size();

    // TODO: should the decoder know this?
    char *newData = new char[fileSize];
    inStream.readRawData( newData, fileSize );

    //registerDiskModifyTime( file ); TODO move into synchronizer

    const bool streamIsOk = ( inStream.status() == QDataStream::Ok );
//     if( success )
//         *success = streamIsOk ? 0 : 1;
    if( streamIsOk )
    {
        KHECore::KByteArrayModel *byteArray = mDocument->content();
        byteArray->setData( newData, fileSize, fileSize, false );
    }
    else
        delete newData;

    return streamIsOk;
}

bool KByteArrayRawFileSynchronizer::writeToFile( const QString &localFilePath )
{
    KHECore::KByteArrayModel *byteArray = mDocument->content();

    QFile file( localFilePath );
    file.open( QIODevice::WriteOnly );

    QDataStream outStream( &file );
    outStream.writeRawData( byteArray->data(), byteArray->size() );

    byteArray->setModified( false );

    //registerDiskModifyTime( file );TODO move into synchronizer

    return outStream.status() == QDataStream::Ok;
}

bool KByteArrayRawFileSynchronizer::synchWithFile( const QString &localFilePath,
                                                   KAbstractDocumentSynchronizer::ConnectOption option )
{
Q_UNUSED( option );
    return writeToFile( localFilePath );
}

void KByteArrayRawFileSynchronizer::onUrlChange( const KUrl &url )
{
    mDocument->setTitle( url.fileName() );
}
