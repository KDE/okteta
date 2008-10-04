/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#include "int32impulsivestreamwriter.h"

// lib
#include "int32impulsivestreamwriter.h"
// Qt
#include <QtCore/QIODevice>


static const int Int32Size = sizeof(qint32);


Int32ImpulsiveStreamWriter::Int32ImpulsiveStreamWriter( qint32 data )
 : mData( data )
{}

bool Int32ImpulsiveStreamWriter::writeTo( QIODevice* ioDevice )
{
    bool success;

    const qint64 writtenBytes = ioDevice->write( (const char*)&mData, Int32Size );
    success = ( writtenBytes == Int32Size );

    return success;
}

Int32ImpulsiveStreamWriter::~Int32ImpulsiveStreamWriter() {}
