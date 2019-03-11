/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008-2009,2014 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractfilesystemsyncfromremotejob_p.hpp"

// library
#include "abstractmodelfilesystemsynchronizer.hpp"
// KF
#include <KIO/FileCopyJob>
#include <KJobWidgets>
// Qt
#include <QFileInfo>
#include <QDateTime>
#include <QUrl>
#include <QTemporaryFile>

namespace Kasten {

void AbstractFileSystemSyncFromRemoteJobPrivate::syncFromRemote()
{
    Q_Q(AbstractFileSystemSyncFromRemoteJob);

    const QUrl url = mSynchronizer->url();

    bool isWorkFileOk;

    if (url.isLocalFile()) {
        // file protocol. We do not need the network
        mWorkFilePath = url.toLocalFile();
        isWorkFileOk = true;
    } else {
        QTemporaryFile tmpFile;
        tmpFile.setAutoRemove(false);
        tmpFile.open();

        mWorkFilePath = tmpFile.fileName();
        mTempFilePath = mWorkFilePath;

        KIO::FileCopyJob* fileCopyJob =
            KIO::file_copy(url, QUrl::fromLocalFile(mWorkFilePath), -1, KIO::Overwrite);
        KJobWidgets::setWindow(fileCopyJob, /*mWidget*/ nullptr);

        isWorkFileOk = fileCopyJob->exec();
        if (!isWorkFileOk) {
            q->setErrorText(fileCopyJob->errorString());
        }
    }

    if (isWorkFileOk) {
        mFile = new QFile(mWorkFilePath);
        isWorkFileOk = mFile->open(QIODevice::ReadOnly);
        if (!isWorkFileOk) {
            q->setErrorText(mFile->errorString());
        }
    }

    if (isWorkFileOk) {
        q->startReadFromFile();
    } else {
        q->setError(KJob::KilledJobError);
        delete mFile;
        q->emitResult();
    }
}

void AbstractFileSystemSyncFromRemoteJobPrivate::completeRead(bool success)
{
    Q_Q(AbstractFileSystemSyncFromRemoteJob);

    if (success) {
        const QUrl url = mSynchronizer->url();
        const bool isLocalFile = url.isLocalFile();

        QFileInfo fileInfo(mWorkFilePath);
        mSynchronizer->setFileDateTimeOnSync(fileInfo.lastModified());
        mSynchronizer->setRemoteState(isLocalFile ? RemoteInSync : RemoteUnknown);
    }

    delete mFile;

    if (!mTempFilePath.isEmpty()) {
        QFile::remove(mTempFilePath);
    }

    q->emitResult();
}

}
