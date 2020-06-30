/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2011-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayviewprofilelock.hpp"

// library
#include <oktetakastengui.hpp>
// Qt
#include <QLockFile>
#include <QSharedPointer>

namespace Kasten {

class ByteArrayViewProfileLockPrivate : public QSharedData
{
public:
    ByteArrayViewProfileLockPrivate(const QString& fileName,
                                    const ByteArrayViewProfile::Id& viewProfileId);

public:
    QSharedPointer<QLockFile> lockFile;
    ByteArrayViewProfile::Id viewProfileId;
};

static QString
viewProfileFileLockPath(const QString& viewProfileFilePath)
{
    // TODO: just ".lock" conflicts with KConfig(?) using the same
    return viewProfileFilePath + QLatin1String(".olock");
}

ByteArrayViewProfileLockPrivate::ByteArrayViewProfileLockPrivate(const QString& fileName,
                                                                 const ByteArrayViewProfile::Id& id)
    : lockFile(new QLockFile(fileName.isEmpty() ? fileName : viewProfileFileLockPath(fileName)))
    , viewProfileId(id)
{
    if (!fileName.isEmpty()) {
        if (!lockFile->tryLock(1000)) {
            qCWarning(LOG_KASTEN_OKTETA_GUI)
                << "Failed to acquire lock file" << fileName
                << "error =" << lockFile->error();
        }
    }
}

ByteArrayViewProfileLock::ByteArrayViewProfileLock(const QString& fileName,
                                                   const ByteArrayViewProfile::Id& viewProfileId)
    : d(new ByteArrayViewProfileLockPrivate(fileName, viewProfileId))
{
}

ByteArrayViewProfileLock::ByteArrayViewProfileLock(const ByteArrayViewProfileLock& other) = default;

ByteArrayViewProfileLock::~ByteArrayViewProfileLock() = default;

ByteArrayViewProfileLock& ByteArrayViewProfileLock::operator=(const ByteArrayViewProfileLock& other) = default;

void
ByteArrayViewProfileLock::unlock()
{
    d->lockFile->unlock();
}

bool
ByteArrayViewProfileLock::isLocked() const
{
    return d->lockFile->isLocked();
}

ByteArrayViewProfile::Id
ByteArrayViewProfileLock::viewProfileId() const
{
    return d->viewProfileId;
}

}
