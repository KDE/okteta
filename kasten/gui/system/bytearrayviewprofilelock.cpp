/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2011-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayviewprofilelock.h"

// KDE
#include <KLockFile>


namespace Kasten2
{

class ByteArrayViewProfileLockPrivate : public QSharedData
{
public:
    ByteArrayViewProfileLockPrivate( const QString& fileName,
                                     const ByteArrayViewProfile::Id& viewProfileId );
public:
    KLockFile lockFile;
    ByteArrayViewProfile::Id viewProfileId;
};


static QString
viewProfileFileLockPath( const QString& viewProfileFilePath)
{
    // TODO: just ".lock" conflicts with KConfig(?) using the same
    return viewProfileFilePath + QLatin1String(".olock");
}

ByteArrayViewProfileLockPrivate::ByteArrayViewProfileLockPrivate( const QString& fileName,
                                                                  const ByteArrayViewProfile::Id& id )
  : lockFile(fileName.isEmpty() ? fileName : viewProfileFileLockPath(fileName)),
    viewProfileId( id )
{
    if( not fileName.isEmpty() )
        lockFile.lock( KLockFile::NoBlockFlag | KLockFile::ForceFlag );
}


ByteArrayViewProfileLock::ByteArrayViewProfileLock( const QString& fileName,
                                                    const ByteArrayViewProfile::Id& viewProfileId )
  : d( new ByteArrayViewProfileLockPrivate(fileName, viewProfileId) )
{
}

ByteArrayViewProfileLock::ByteArrayViewProfileLock( const ByteArrayViewProfileLock& other )
  : d( other.d )
{
}

ByteArrayViewProfileLock&
ByteArrayViewProfileLock::operator=( const ByteArrayViewProfileLock& other )
{
    d = other.d;

    return *this;
}

void
ByteArrayViewProfileLock::unlock()
{
    d->lockFile.unlock();
}

bool
ByteArrayViewProfileLock::isLocked() const
{
    return d->lockFile.isLocked();
}

ByteArrayViewProfile::Id
ByteArrayViewProfileLock::viewProfileId() const
{
    return d->viewProfileId;
}


ByteArrayViewProfileLock::~ByteArrayViewProfileLock()
{
}

}
