/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// Okteta core
#include <piecetablebytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QDataStream>
#include <QtCore/QFile>
// C++
#include <limits>


namespace Kasten2
{

ByteArrayRawFileReloadThread::ByteArrayRawFileReloadThread( QObject* parent, QFile* file )
  : QThread( parent ),
    mFile( file ),
    mSuccess( false )
{
}

void ByteArrayRawFileReloadThread::run()
{
    const qint64 fileSize = mFile->size();

    // check if the file content can be addressed with int
    const qint32 maxInt32 = std::numeric_limits<qint32>::max();
    const int maxInt = std::numeric_limits<int>::max();

    mSuccess = ( maxInt > maxInt32 || fileSize <= maxInt32 );

    if( mSuccess )
    {
        mData.resize( fileSize );
        mSuccess = ( mData.size() == fileSize );

        if( mSuccess )
        {
            QDataStream inStream( mFile );
            inStream.readRawData( mData.data(), fileSize );

            mSuccess = ( inStream.status() == QDataStream::Ok );

            if( ! mSuccess )
                mErrorString = mFile->errorString();
        }
        else
            mErrorString = i18n( "There is not enough free working memory to load this file." );
    }
    else
        mErrorString = i18n( "Support to load files larger than 2 GiB on 32-bit systems has not yet been implemented." );

    emit documentReloaded( mSuccess );
}

ByteArrayRawFileReloadThread::~ByteArrayRawFileReloadThread() {}

}
