/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfilewritejob.hpp"

// lib
// #include "externalbookmarkstorage.hpp"
#include "bytearrayrawfilesynchronizer.hpp"
#include "bytearrayrawfilewritethread.hpp"
#include "bytearraydocument.hpp"
// Qt
#include <QCoreApplication>

namespace Kasten {

ByteArrayRawFileWriteJob::ByteArrayRawFileWriteJob(ByteArrayRawFileSynchronizer* synchronizer)
    : AbstractFileSystemSyncToRemoteJob(synchronizer)
{}

ByteArrayRawFileWriteJob::~ByteArrayRawFileWriteJob() = default;

void ByteArrayRawFileWriteJob::startWriteToFile()
{
    auto* document = qobject_cast<ByteArrayDocument*>(synchronizer()->document());
    auto* writeThread = new ByteArrayRawFileWriteThread(this, document, file());
    writeThread->start();
    while (!writeThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    const bool success = writeThread->success();
    delete writeThread;

//     if( success )
//         ExternalBookmarkStorage().writeBookmarks( document, synchronizer()->url() );

    completeWrite(success);
}

}
