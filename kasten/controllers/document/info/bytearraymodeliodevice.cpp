/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraymodeliodevice.h"

// Okteta core
#include <abstractbytearraymodel.h>


namespace Okteta
{

ByteArrayModelIoDevice::ByteArrayModelIoDevice( AbstractByteArrayModel* byteArrayModel, QObject* parent )
  : QIODevice( parent ),
    mByteArrayModel( byteArrayModel ),
    mReadOffset( 0 )
{
    open( ReadOnly ); // krazy:exclude=syscalls
}

qint64 ByteArrayModelIoDevice::size() const
{
    return mByteArrayModel->size();
}

bool ByteArrayModelIoDevice::canReadLine() const
{
    return
        isOpen()
        && ( mByteArrayModel->indexOf("\n", 1, pos() ) != -1
             || QIODevice::canReadLine() );
}

bool ByteArrayModelIoDevice::open( OpenMode openMode )
{
    QIODevice::open( openMode );

    openMode ^= WriteOnly | Append;
    setOpenMode( openMode );

    if( ! isReadable() )
        return false;

    seek( 0 );

    return true;
}

bool ByteArrayModelIoDevice::seek( qint64 pos )
{
    if( pos > mByteArrayModel->size() || pos < 0 )
        return false;

    mReadOffset = pos;

    return QIODevice::seek( pos );
}

qint64 ByteArrayModelIoDevice::readData( char* data, qint64 maxlength )
{
    const Size copiedLength =
        mByteArrayModel->copyTo( reinterpret_cast<Byte*>(data), mReadOffset, maxlength );

    mReadOffset += copiedLength;

    return copiedLength;
}

qint64 ByteArrayModelIoDevice::writeData( const char* data, qint64 length )
{
    Q_UNUSED( data );
    Q_UNUSED( length );
    return -1;
}

ByteArrayModelIoDevice::~ByteArrayModelIoDevice() {}

}
