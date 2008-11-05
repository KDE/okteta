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

#include "bytearrayrawfilewritethread.h"

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


void ByteArrayRawFileWriteThread::run()
{
    KHECore::PieceTableByteArrayModel *byteArray = qobject_cast<KHECore::PieceTableByteArrayModel*>( mDocument->content() );

    QFile file( mFilePath );
    if( file.open(QIODevice::WriteOnly) )
    {
        QDataStream outStream( &file );

    //TODO: this was
//     outStream.writeRawData( byteArray->data(), byteArray->size() );
    // make it quicker again by writing spans -> spaniterator

        for( int i = 0; i<byteArray->size(); ++i )
        {
            const char datum = byteArray->datum(i);
            outStream.writeRawData( &datum, 1 );
        }

        byteArray->setModified( false );

        //registerDiskModifyTime( file );TODO move into synchronizer

        mSuccess = ( outStream.status() == QDataStream::Ok );
    }
    else
        mSuccess = false;

    emit documentWritten( mSuccess );
}

ByteArrayRawFileWriteThread::~ByteArrayRawFileWriteThread() {}

#include "bytearrayrawfilewritethread.moc"
