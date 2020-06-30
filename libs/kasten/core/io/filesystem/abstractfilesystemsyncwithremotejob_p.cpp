/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemsyncwithremotejob_p.hpp"

// library
#include "abstractmodelfilesystemsynchronizer.hpp"
#include <abstractdocument.hpp>
// KF
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
            auto* temporaryFile = new QTemporaryFile;
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
        // TODO: should we rather skip completeSync in success API?
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
