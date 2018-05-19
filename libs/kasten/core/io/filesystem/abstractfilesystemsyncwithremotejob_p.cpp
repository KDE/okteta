/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008-2009,2011,2014 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "abstractfilesystemsyncwithremotejob_p.h"

// library
#include "abstractmodelfilesystemsynchronizer.h"
#include <abstractdocument.h>
// KF5
#include <KIO/FileCopyJob>
#include <KJobWidgets>
// Qt
#include <QFileInfo>
#include <QDateTime>
#include <QTemporaryFile>

namespace Kasten {

void AbstractFileSystemSyncWithRemoteJobPrivate::syncWithRemote()
{
    Q_Q(AbstractFileSystemSyncWithRemoteJob);

    bool isWorkFileOk;
    if (mOption == AbstractModelSynchronizer::ReplaceRemote) {
        if (mUrl.isLocalFile()) {
            mWorkFilePath = mUrl.toLocalFile();
            mFile = new QFile(mWorkFilePath);
            isWorkFileOk = mFile->open(QIODevice::WriteOnly);
        } else {
            QTemporaryFile* temporaryFile = new QTemporaryFile;
            isWorkFileOk = temporaryFile->open();

            mWorkFilePath = temporaryFile->fileName();
            mTempFilePath = mWorkFilePath;
            mFile = temporaryFile;
        }
        if (!isWorkFileOk) {
            q->setErrorText(mFile->errorString());
        }
    } else {
        if (mUrl.isLocalFile()) {
            // file protocol. We do not need the network
            mWorkFilePath = mUrl.toLocalFile();
            isWorkFileOk = true;
        } else {
            QTemporaryFile tmpFile;
            tmpFile.setAutoRemove(false);
            tmpFile.open();

            mWorkFilePath = tmpFile.fileName();
            mTempFilePath = mWorkFilePath;

            KIO::FileCopyJob* fileCopyJob =
                KIO::file_copy(mUrl, QUrl::fromLocalFile(mWorkFilePath), -1, KIO::Overwrite);
            KJobWidgets::setWindow(fileCopyJob, /*mWidget*/ nullptr);

            isWorkFileOk = fileCopyJob->exec();
            if (!isWorkFileOk) {
                q->setErrorText(fileCopyJob->errorString());
            }
        }

        if (isWorkFileOk) {
            mFile = new QFile(mWorkFilePath);
            isWorkFileOk = mFile->open(QIODevice::ReadWrite);
            if (!isWorkFileOk) {
                q->setErrorText(mFile->errorString());
            }
        }
    }

    if (isWorkFileOk) {
        const QUrl oldUrl = mSynchronizer->url();
        if (oldUrl.isLocalFile()) {
            mSynchronizer->stopFileWatching();
        } else {
            mSynchronizer->stopNetworkWatching();
        }

        q->startSyncWithRemote();
    } else {
        q->setError(KJob::KilledJobError);
        delete mFile;
        // TODO: should we rather skip completeSync in successthe API?
        q->emitResult();
    }
}

void AbstractFileSystemSyncWithRemoteJobPrivate::completeSync(bool success)
{
    Q_Q(AbstractFileSystemSyncWithRemoteJob);

    if (success) {
        mFile->close(); // TODO: when is new time written, on close?
        QFileInfo fileInfo(*mFile);
        mSynchronizer->setFileDateTimeOnSync(fileInfo.lastModified());

        mSynchronizer->setUrl(mUrl);

        if (!mUrl.isLocalFile()) {
            KIO::FileCopyJob* fileCopyJob =
                KIO::file_copy(QUrl::fromLocalFile(mWorkFilePath), mUrl, -1, KIO::Overwrite);
            KJobWidgets::setWindow(fileCopyJob, /*mWidget*/ nullptr);

            const bool uploaded = fileCopyJob->exec();
            if (!uploaded) {
                q->setError(KJob::KilledJobError);
                q->setErrorText(fileCopyJob->errorString());
            } else {
                mSynchronizer->startNetworkWatching();
                mSynchronizer->setRemoteState(RemoteUnknown);
            }
        } else {
            mSynchronizer->startFileWatching();
            mSynchronizer->setRemoteState(RemoteInSync);
        }

    } else {
        q->setError(KJob::KilledJobError);
        q->setErrorText(mFile->errorString());
    }

    delete mFile;

    if (!mTempFilePath.isEmpty()) {
        QFile::remove(mTempFilePath);
    }

    q->emitResult();
}

}
