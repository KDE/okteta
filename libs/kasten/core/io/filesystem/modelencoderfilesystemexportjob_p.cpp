/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelencoderfilesystemexportjob_p.hpp"

// lib
#include "modelstreamencodethread.hpp"
// Qt
#include <QCoreApplication>
#include <QFile>

namespace Kasten {

void ModelEncoderFileSystemExportJobPrivate::startExportToFile()
{
    Q_Q(ModelEncoderFileSystemExportJob);

    auto* exportThread = new ModelStreamEncodeThread(q, file(), model(), selection(), mEncoder);
    exportThread->start();
    while (!exportThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    const bool success = exportThread->success();
    delete exportThread;

    q->completeExport(success);
}

}
