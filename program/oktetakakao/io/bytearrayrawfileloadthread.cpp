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

#include "bytearrayrawfileloadthread.h"

// lib
#include "kbytearraydocument.h"
// Okteta core
#include <kpiecetablebytearraymodel.h>
// KDE
#include <KUrl>
#include <KLocale>
// Qt
#include <QtGui/QApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QString>


void ByteArrayRawFileLoadThread::run()
{
    QFile file( mFilePath );
    file.open( QIODevice::ReadOnly );
    QDataStream inStream( &file );
    int fileSize = file.size();

    // TODO: should the decoder know this?
    // it is in the api now (constructor)

    char *data = new char[fileSize];
    inStream.readRawData( data, fileSize );
    KHECore::KPieceTableByteArrayModel *byteArray = new KHECore::KPieceTableByteArrayModel( data, fileSize );

    byteArray->setModified( false );

    //registerDiskModifyTime( file ); TODO move into synchronizer

    const bool streamIsOk = ( inStream.status() == QDataStream::Ok );
//     if( success )
//         *success = streamIsOk ? 0 : 1;
    if( streamIsOk )
    {
        mDocument = new KByteArrayDocument( byteArray, i18nc("destination of the byte array", "Loaded from file.") );
        // TODO: make KPieceTableByteArrayModel a child by constructor argument parent
        byteArray->moveToThread( QApplication::instance()->thread() );
        mDocument->moveToThread( QApplication::instance()->thread() );
    }
    else
    {
        mDocument = 0;
        delete byteArray;
    }

    emit documentRead( mDocument );
}

ByteArrayRawFileLoadThread::~ByteArrayRawFileLoadThread() {}

#include "bytearrayrawfileloadthread.moc"
