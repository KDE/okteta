/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractfilesystemexportjob_p.hpp"

// KF
#include <KIO/FileCopyJob>
#include <KJobWidgets>
// Qt
#include <QTemporaryFile>

namespace Kasten {

void AbstractFileSystemExportJobPrivate::exportToFile()
{
    Q_Q(AbstractFileSystemExportJob);

    bool isWorkFileOk;
    if (mUrl.isLocalFile()) {
        mWorkFilePath = mUrl.toLocalFile();
        mFile = new QFile(mWorkFilePath);
        isWorkFileOk = mFile->open(QIODevice::WriteOnly);
    } else {

        auto* temporaryFile = new QTemporaryFile();
        isWorkFileOk = temporaryFile->open();

        mWorkFilePath = temporaryFile->fileName();
        mFile = temporaryFile;
    }

    if (isWorkFileOk) {
        q->startExportToFile();
    } else {
        q->setError(KJob::KilledJobError);
        q->setErrorText(mFile->errorString());

        q->completeExport(false);
    }
}

void AbstractFileSystemExportJobPrivate::completeExport(bool success)
{
    Q_Q(AbstractFileSystemExportJob);

    if (success) {
        if (!mUrl.isLocalFile()) {
            KIO::FileCopyJob* fileCopyJob =
                KIO::file_copy(QUrl::fromLocalFile(mWorkFilePath), mUrl, -1, KIO::Overwrite);
            KJobWidgets::setWindow(fileCopyJob, /*mWidget*/ nullptr);

            success = fileCopyJob->exec();
            if (!success) {
                q->setError(KJob::KilledJobError);
                q->setErrorText(fileCopyJob->errorString());
            }
        }
    } else {
        q->setError(KJob::KilledJobError);
        q->setErrorText(mFile->errorString());
    }

    delete mFile;

    q->emitResult();
}

}
