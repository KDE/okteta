/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "testdocumentfilesynchronizer.h"

// lib
#include "testdocument.h"
// KDE
#include <KUrl>
// Qt
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QFile>


TestDocumentFileSynchronizer::TestDocumentFileSynchronizer( const KUrl &url, const QByteArray &header )
 : mHeader( header )
{
    KAbstractDocument *document = loadFromUrl( url );
    mDocument = document ? qobject_cast<TestDocument*>( document ) : 0;
    if( mDocument )
    {
        mDocument->setTitle( url.fileName() );
    }
}

TestDocumentFileSynchronizer::TestDocumentFileSynchronizer( KAbstractDocument *document, const KUrl &url,
                                                            KAbstractDocumentSynchronizer::ConnectOption option,
                                                            const QByteArray &header )
 : mHeader( header )
{
    // TODO: is synchronizer->document() really a good signal for success? see also above
    mDocument = document ? qobject_cast<TestDocument*>( document ) : 0;
    if( mDocument )
    {
        if( !syncWithUrl(url,option) )
            mDocument = 0;
    }
//     if( mDocument )
//         onUrlChange( url );
//     connect( this, SIGNAL(urlChanged(const KUrl&)), SLOT(onUrlChange( const KUrl & )) );
}

KAbstractDocument *TestDocumentFileSynchronizer::document() const { return mDocument; }


KAbstractDocument *TestDocumentFileSynchronizer::loadFromFile( const QString &localFileName )
{
    TestDocument *document = 0;

    QFile file( localFileName );
    file.open( QIODevice::ReadOnly );
    QDataStream inStream( &file );
    int fileSize = file.size();

    // test header
    const int headerSize = mHeader.size();
    QByteArray header( headerSize, ' ' );
    const int headerResult = inStream.readRawData( header.data(), headerSize );
    if( headerResult == -1 || header != mHeader )
        return false;

    QByteArray byteArray( fileSize, ' ' );

    inStream.readRawData( byteArray.data(), fileSize );

//     byteArray->setModified( false );

    //registerDiskModifyTime( file ); TODO move into synchronizer

    const bool streamIsOk = ( inStream.status() == QDataStream::Ok );
//     if( success )
//         *success = streamIsOk ? 0 : 1;
    if( streamIsOk )
        document = new TestDocument( byteArray );

    return document;
}

bool TestDocumentFileSynchronizer::reloadFromFile( const QString &localFileName )
{
    QFile file( localFileName );
    file.open( QIODevice::ReadOnly );
    QDataStream inStream( &file );
    int fileSize = file.size();

    // test header
    const int headerSize = mHeader.size();
    QByteArray header( headerSize, ' ' );
    const int headerResult = inStream.readRawData( header.data(), headerSize );
    if( headerResult == -1 || header != mHeader )
        return false;

    // TODO: should the decoder know this?
    QByteArray newData( fileSize, ' ' );
    inStream.readRawData( newData.data(), fileSize );

    //registerDiskModifyTime( file ); TODO move into synchronizer

    const bool streamIsOk = ( inStream.status() == QDataStream::Ok );
//     if( success )
//         *success = streamIsOk ? 0 : 1;
    if( streamIsOk )
    {
        mDocument->setData( newData );
    }

    return streamIsOk;
}

bool TestDocumentFileSynchronizer::writeToFile( const QString &localFilePath )
{
    const QByteArray *byteArray = mDocument->data();

    QFile file( localFilePath );
    file.open( QIODevice::WriteOnly );

    QDataStream outStream( &file );
    outStream.writeRawData( mHeader.data(), mHeader.size() );
    outStream.writeRawData( byteArray->data(), byteArray->size() );

//     byteArray->setModified( false );

    //registerDiskModifyTime( file );TODO move into synchronizer

    return outStream.status() == QDataStream::Ok;
}

bool TestDocumentFileSynchronizer::syncWithFile( const QString &localFilePath,
                                                 KAbstractDocumentSynchronizer::ConnectOption option )
{
Q_UNUSED( option )
    return writeToFile( localFilePath );
}
