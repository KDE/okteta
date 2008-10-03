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

#include "changelistimpulsivestreamreader.h"

// Qt
#include <QtCore/QIODevice>


static const int BaseDataBlockSize = 4096; // TODO: what would be a good block size?
static const int ArrayChangeMetricsSize = sizeof(KHE::ArrayChangeMetrics);

ChangeListImpulsiveStreamReader::ChangeListImpulsiveStreamReader()
: AbstractImpulsiveStreamReader(),
  mChangeList()
{}

int ChangeListImpulsiveStreamReader::nextBytesNeeded() const
{
    int result = mSizeReader.nextBytesNeeded();
    if( result == 0 )
        result = mByteArrayChangeReader.nextBytesNeeded();
    return result;
}

bool ChangeListImpulsiveStreamReader::readFrom( QIODevice* ioDevice )
{
    bool success;

    if( !mSizeReader.isDone() )
    {
        success = mSizeReader.readFrom( ioDevice );
    }
    else
    {
        success = mByteArrayChangeReader.readFrom( ioDevice );

        if( mByteArrayChangeReader.isDone() )
        {
            mChangeList.append( mByteArrayChangeReader.byteArrayChange() );
            const bool moreToRead = ( mChangeList.count() < mSizeReader.data() );
            if( moreToRead )
                mByteArrayChangeReader.reset();
        }
    }

    return success;
}

void ChangeListImpulsiveStreamReader::reset()
{
    mChangeList.clear();
    mByteArrayChangeReader.reset();
}


ChangeListImpulsiveStreamReader::~ChangeListImpulsiveStreamReader() {}
