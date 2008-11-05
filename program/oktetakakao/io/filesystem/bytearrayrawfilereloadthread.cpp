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

#include "bytearrayrawfilereloadthread.h"

// lib
#include "kbytearraydocument.h"
// Okteta core
#include <piecetablebytearraymodel.h>
// KDE
#include <KUrl>
#include <KLocale>
// Qt
#include <QtGui/QApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QString>

ByteArrayRawFileReloadThread::ByteArrayRawFileReloadThread( QObject *parent,
    /*KByteArrayDocument *document,*/ const QString &filePath )
 : QThread( parent ), /*mDocument( document ),*/ mFilePath( filePath ), mSuccess( false )
{
//     mDocument->content()->moveToThread( this );
//     mDocument->moveToThread( this );
}

void ByteArrayRawFileReloadThread::run()
{
    QFile file( mFilePath );
    file.open( QIODevice::ReadOnly );
    QDataStream inStream( &file );
    mSize = file.size();

    // TODO: should the decoder know this?
    mData = new char[mSize];
    inStream.readRawData( mData, mSize );

    //registerDiskModifyTime( file ); TODO move into synchronizer

    mSuccess = ( inStream.status() == QDataStream::Ok );
//     if( success )
//         *success = streamIsOk ? 0 : 1;
//     mDocument->content()->moveToThread( QApplication::instance()->thread() );
//     mDocument->moveToThread( QApplication::instance()->thread() );
    if( mSuccess )
    {
//         KHECore::PieceTableByteArrayModel *byteArray = qobject_cast<KHECore::PieceTableByteArrayModel*>( mDocument->content() );
//         byteArray->setData( mData, mSize, false );
    }
    else
        delete [] mData;

    emit documentReloaded( mSuccess );
}

ByteArrayRawFileReloadThread::~ByteArrayRawFileReloadThread() {}

#include "bytearrayrawfilereloadthread.moc"
