/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include "bytearraydocument.h"
// Kasten core
#include <person.h>
// Okteta core
#include <piecetablebytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>


namespace Kasten
{

void ByteArrayRawFileLoadThread::run()
{
    qint64 fileSize = mFile->size();

    // allocate working memory
    QByteArray data;
    data.resize( fileSize );
    bool success = ( data.size() == fileSize );

    if( success )
    {
        QDataStream inStream( mFile );
        inStream.readRawData( data.data(), fileSize );

        success = ( inStream.status() == QDataStream::Ok );

        if( success )
        {
            Okteta::PieceTableByteArrayModel* byteArray = new Okteta::PieceTableByteArrayModel( data );
            byteArray->setModified( false );

            mDocument = new ByteArrayDocument( byteArray, i18nc("destination of the byte array", "Loaded from file.") );
            mDocument->setOwner( Person::createEgo() );
            // TODO: make PieceTableByteArrayModel a child by constructor argument parent
            byteArray->moveToThread( QApplication::instance()->thread() );
            mDocument->moveToThread( QApplication::instance()->thread() );
        }
    }
    // TODO: else report file too large

    if( ! success )
        mDocument = 0;

    emit documentRead( mDocument );
}

ByteArrayRawFileLoadThread::~ByteArrayRawFileLoadThread() {}

}
