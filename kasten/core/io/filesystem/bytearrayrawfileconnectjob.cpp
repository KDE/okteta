/*
    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfileconnectjob.hpp"

// lib
// #include "externalbookmarkstorage.hpp"
#include "bytearrayrawfilesynchronizer.hpp"
#include "bytearrayrawfilewritethread.hpp"
#include "bytearraydocument.hpp"
// Qt
#include <QCoreApplication>
// Std
#include <utility>

namespace Kasten {

ByteArrayRawFileConnectJob::ByteArrayRawFileConnectJob(std::unique_ptr<ByteArrayRawFileSynchronizer>&& synchronizer,
                                                       AbstractDocument* document,
                                                       const QUrl& url,
                                                       AbstractModelSynchronizer::ConnectOption option)
    : AbstractFileSystemConnectJob(std::move(synchronizer), document, url, option)
{
}

ByteArrayRawFileConnectJob::~ByteArrayRawFileConnectJob() = default;

void ByteArrayRawFileConnectJob::startConnectWithFile()
{
    auto* const byteArrayDocument = qobject_cast<ByteArrayDocument*>(document());
    auto writeThread = std::make_unique<ByteArrayRawFileWriteThread>(this, byteArrayDocument, file());
    writeThread->start();
    while (!writeThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    qobject_cast<ByteArrayRawFileSynchronizer*>(synchronizer())->setDocument(byteArrayDocument);
    const bool success = writeThread->success();
    writeThread.reset();

//     if( success )
//         ExternalBookmarkStorage().writeBookmarks( byteArrayDocument, synchronizer()->url() );

    complete(success);
}

}

#include "moc_bytearrayrawfileconnectjob.cpp"
