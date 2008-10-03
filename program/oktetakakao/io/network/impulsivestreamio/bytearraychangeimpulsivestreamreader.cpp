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

#include "bytearraychangeimpulsivestreamreader.h"

// Qt
#include <QtCore/QIODevice>


static const int BaseDataBlockSize = 4096; // TODO: what would be a good block size?
static const int ArrayChangeMetricsSize = sizeof(KHE::ArrayChangeMetrics);

ByteArrayChangeImpulsiveStreamReader::ByteArrayChangeImpulsiveStreamReader()
: AbstractImpulsiveStreamReader()
{}


KHECore::ByteArrayChange ByteArrayChangeImpulsiveStreamReader::byteArrayChange() const
{
    return KHECore::ByteArrayChange( mMetricsReader.metrics(), mByteArrayReader.byteArray() );
}


int ByteArrayChangeImpulsiveStreamReader::nextBytesNeeded() const
{
    int result = mMetricsReader.nextBytesNeeded();
    if( result == 0 )
        result = mByteArrayReader.nextBytesNeeded();
    return result;
}

bool ByteArrayChangeImpulsiveStreamReader::readFrom( QIODevice* ioDevice )
{
    bool result;

    if( !mMetricsReader.isDone() )
        result = mMetricsReader.readFrom( ioDevice );
    else
        result = mByteArrayReader.readFrom( ioDevice );

    return result;
}


void ByteArrayChangeImpulsiveStreamReader::reset()
{
    mMetricsReader.reset();
    mByteArrayReader.reset();
}

ByteArrayChangeImpulsiveStreamReader::~ByteArrayChangeImpulsiveStreamReader() {}
