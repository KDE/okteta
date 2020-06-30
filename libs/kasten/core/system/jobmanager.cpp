/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "jobmanager.hpp"

// KF
#include <KJob>
#include <KMessageBox>
// Qt
#include <QGuiApplication>

namespace Kasten {

bool JobManager::executeJob(KJob* job)
{
    if (!job) {
        return false;
    }

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);

    job->exec();
    const bool success = (job->error() == KJob::NoError);

    QGuiApplication::restoreOverrideCursor();

    if (!success) {
        KMessageBox::error(nullptr, job->errorText());   // TODO: feed into notification system

    }
    return success;
}

}
