/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemloadjob_p.hpp"

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

void AbstractFileSystemLoadJobPrivate::load()
{
    Q_Q(AbstractFileSystemLoadJob);

    bool isWorkFileOk;

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
        isWorkFileOk = mFile->open(QIODevice::ReadOnly);
        if (!isWorkFileOk) {
            q->setErrorText(mFile->errorString());
        }
    }

    if (isWorkFileOk) {
        q->startLoadFromFile();
    } else {
        q->setError(KJob::KilledJobError);
        // TODO: should we rather skip setDocument in the API?
        q->AbstractLoadJob::setDocument(nullptr);
    }
}

void AbstractFileSystemLoadJobPrivate::setDocument(AbstractDocument* document)
{
    Q_Q(AbstractFileSystemLoadJob);

    if (document) {
        const bool isLocalFile = mUrl.isLocalFile();
        mFile->close(); // TODO: when is new time written, on close?

        // TODO: reading the fileinfo here separated from the content reading without a lock
        // asks for a race-condition to happen where the file is modified in between
        // TODO: how to handle remote+temp?
        QFileInfo fileInfo(*mFile);
        mSynchronizer->setFileDateTimeOnSync(fileInfo.lastModified());
        mSynchronizer->setUrl(mUrl);
        if (isLocalFile) {
            mSynchronizer->startFileWatching();
        } else {
            mSynchronizer->startNetworkWatching();
        }
        mSynchronizer->setRemoteState(isLocalFile ? RemoteInSync : RemoteUnknown);

        document->setSynchronizer(mSynchronizer);
    } else {
        delete mSynchronizer;
    }

    delete mFile;

    if (!mTempFilePath.isEmpty()) {
        QFile::remove(mTempFilePath);
    }

    q->AbstractLoadJob::setDocument(document);
}

}
