/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfilereloadjob.hpp"

// lib
// #include "externalbookmarkstorage.hpp"
#include "bytearrayrawfilesynchronizer.hpp"
#include "bytearrayrawfilereloadthread.hpp"
#include "bytearraydocument.hpp"
// Okteta core
#include <Okteta/PieceTableByteArrayModel>
// Qt
#include <QCoreApplication>

namespace Kasten {

ByteArrayRawFileReloadJob::ByteArrayRawFileReloadJob(ByteArrayRawFileSynchronizer* synchronizer)
    : AbstractFileSystemSyncFromRemoteJob(synchronizer)
{}

ByteArrayRawFileReloadJob::~ByteArrayRawFileReloadJob() = default;

void ByteArrayRawFileReloadJob::startReadFromFile()
{
    auto* document = qobject_cast<ByteArrayDocument*>(synchronizer()->document());
    auto* reloadThread = new ByteArrayRawFileReloadThread(this, /*document, */ file());
    reloadThread->start();
    while (!reloadThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    bool success = reloadThread->success();
    // TODO: moved this here to avoid marshalling the change signals out of the thread. Good idea?
    if (success) {
        auto* byteArray = qobject_cast<Okteta::PieceTableByteArrayModel*>(document->content());
        byteArray->setData(reloadThread->data());

//         ExternalBookmarkStorage().readBookmarks( document, synchronizer()->url() );
    } else {
        setError(KJob::KilledJobError);
        setErrorText(reloadThread->errorString());
    }

    delete reloadThread;

    completeRead(success);
}

}
