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

#include "qbytearrayimpulsivestreamreader.h"

// Qt
#include <QtCore/QIODevice>


static const int BaseDataBlockSize = 4096; // TODO: what would be a good block size?


QByteArrayImpulsiveStreamReader::QByteArrayImpulsiveStreamReader()
: AbstractImpulsiveStreamReader(),
  mReadSize( 0 )
{}

int QByteArrayImpulsiveStreamReader::nextBytesNeeded() const
{
    int result = mSizeReader.nextBytesNeeded();
    if( result == 0 )
        result = nextDataBytesNeeded();
    return result;
}

int QByteArrayImpulsiveStreamReader::nextDataBytesNeeded() const
{
    const int leftSize = mSizeReader.data() - mReadSize;
    const int result = ( BaseDataBlockSize < leftSize ) ? BaseDataBlockSize :leftSize;
    return result;
}

bool QByteArrayImpulsiveStreamReader::readFrom( QIODevice* ioDevice )
{
    bool success;

    if( !mSizeReader.isDone() )
    {
        success = mSizeReader.readFrom( ioDevice );
        mByteArray = QByteArray( mSizeReader.data(), 0 );
    }
    else
    {
        const qint64 readCount = ioDevice->read( &mByteArray.data()[mReadSize], nextDataBytesNeeded() );

        success = ( readCount != -1 );
        if( success )
            mReadSize += readCount;
    }
    return success;
}


void QByteArrayImpulsiveStreamReader::reset()
{
    mSizeReader.reset();
    mByteArray = QByteArray();
    mReadSize = 0;
}


QByteArrayImpulsiveStreamReader::~QByteArrayImpulsiveStreamReader() {}
