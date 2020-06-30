/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemsynctoremotejob_p.hpp"

// KF
#include <KIO/FileCopyJob>
#include <KJobWidgets>
// Qt
#include <QFileInfo>
#include <QDateTime>

namespace Kasten {

AbstractFileSystemSyncToRemoteJobPrivate::~AbstractFileSystemSyncToRemoteJobPrivate() = default;

void AbstractFileSystemSyncToRemoteJobPrivate::syncToRemote()
{
    Q_Q(AbstractFileSystemSyncToRemoteJob);

    bool isWorkFileOk;
    const QUrl url = mSynchronizer->url();

    if (url.isLocalFile()) {
        mWorkFilePath = url.toLocalFile();
        mFile = new QFile(mWorkFilePath);
        isWorkFileOk = mFile->open(QIODevice::WriteOnly);

        mSynchronizer->pauseFileWatching();
    } else {
        auto* temporaryFile = new QTemporaryFile;
        isWorkFileOk = temporaryFile->open();

        mWorkFilePath = temporaryFile->fileName();
        mFile = temporaryFile;
    }

    if (isWorkFileOk) {
        q->startWriteToFile();
    } else {
        q->setError(KJob::KilledJobError);
        q->setErrorText(mFile->errorString());
        delete mFile;
        q->emitResult();
    }
}

void AbstractFileSystemSyncToRemoteJobPrivate::completeWrite(bool success)
{
    Q_Q(AbstractFileSystemSyncToRemoteJob);

    if (success) {
        mFile->close(); // TODO: when is new time written, on close?
        QFileInfo fileInfo(*mFile);
        mSynchronizer->setFileDateTimeOnSync(fileInfo.lastModified());

        const QUrl url = mSynchronizer->url();
        const bool isLocalFile = url.isLocalFile();

        if (!isLocalFile) {
            KIO::FileCopyJob* fileCopyJob =
                KIO::file_copy(QUrl::fromLocalFile(mWorkFilePath), url, -1, KIO::Overwrite);
            KJobWidgets::setWindow(fileCopyJob, /*mWidget*/ nullptr);

            success = fileCopyJob->exec();
            if (!success) {
                q->setError(KJob::KilledJobError);
                q->setErrorText(fileCopyJob->errorString());
            } else {
                mSynchronizer->setRemoteState(RemoteUnknown);
            }
        } else {
            mSynchronizer->unpauseFileWatching();
            mSynchronizer->setRemoteState(RemoteInSync);
        }
    } else {
        q->setError(KJob::KilledJobError);
        q->setErrorText(mFile->errorString());
    }

    delete mFile;

    q->emitResult();
}

}
