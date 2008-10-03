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

#include "qstringimpulsivestreamreader.h"

// Qt
#include <QtCore/QIODevice>


static const int StringBlockSize = 4096; // TODO: what would be a good block size?
static const int QCharSize = sizeof( QChar );


QStringImpulsiveStreamReader::QStringImpulsiveStreamReader()
: AbstractImpulsiveStreamReader(),
  mReadSize( 0 )
{}

int QStringImpulsiveStreamReader::nextBytesNeeded() const
{
    int result = mSizeReader.nextBytesNeeded();
    if( result == 0 )
        result = nextCharsNeeded() * QCharSize;
    return result;
}

int QStringImpulsiveStreamReader::nextCharsNeeded() const
{
    const int leftSize = mSizeReader.data() - mReadSize;
    const int result = ( StringBlockSize < leftSize ) ? StringBlockSize :leftSize;
    return result;
}

bool QStringImpulsiveStreamReader::readFrom( QIODevice* ioDevice )
{
    bool success;

    if( !mSizeReader.isDone() )
    {
        success = mSizeReader.readFrom( ioDevice );
        mString = QString( mSizeReader.data(), 0 );
    }
    else
    {
        const qint64 readCount = ioDevice->read( (char*)&mString.data()[mReadSize], nextCharsNeeded()*QCharSize );

        success = ( readCount != -1 );
        if( success )
            mReadSize += readCount/QCharSize;
    }
    return success;
}


void QStringImpulsiveStreamReader::reset()
{
    mSizeReader.reset();
    mString = QString();
    mReadSize = 0;
}


QStringImpulsiveStreamReader::~QStringImpulsiveStreamReader() {}
