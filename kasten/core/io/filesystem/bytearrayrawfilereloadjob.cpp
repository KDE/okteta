/*
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
    auto* const document = qobject_cast<ByteArrayDocument*>(synchronizer()->document());
    auto reloadThread = std::make_unique<ByteArrayRawFileReloadThread>(this, /*document, */ file());
    reloadThread->start();
    while (!reloadThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    bool success = reloadThread->success();
    // TODO: moved this here to avoid marshalling the change signals out of the thread. Good idea?
    if (success) {
        auto* const byteArray = qobject_cast<Okteta::PieceTableByteArrayModel*>(document->content());
        byteArray->setData(reloadThread->data());

//         ExternalBookmarkStorage().readBookmarks( document, synchronizer()->url() );
    } else {
        setError(KJob::KilledJobError);
        setErrorText(reloadThread->errorString());
    }

    reloadThread.reset();

    completeRead(success);
}

}

#include "moc_bytearrayrawfilereloadjob.cpp"
