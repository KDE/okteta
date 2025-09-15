/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfileloadthread.hpp"

// Qt
#include <QDataStream>
#include <QFile>
#include <QCoreApplication>

namespace Kasten {

TestDocumentFileLoadThread::~TestDocumentFileLoadThread() = default;

void TestDocumentFileLoadThread::run()
{
    QDataStream inStream(mFile);
    const int fileSize = mFile->size();

    // test header
    const int headerSize = mHeader.size();
    QByteArray header(headerSize, ' ');
    const int headerResult = inStream.readRawData(header.data(), headerSize);
    if ((headerResult != -1) && (header == mHeader)) {
        QByteArray byteArray(fileSize, ' ');

        inStream.readRawData(byteArray.data(), fileSize);

        // registerDiskModifyTime( file ); TODO move into synchronizer

        const bool streamIsOk = (inStream.status() == QDataStream::Ok);
        //     if( success )
        //         *success = streamIsOk ? 0 : 1;
        if (streamIsOk) {
            mDocument = std::make_unique<TestDocument>(byteArray);
            mDocument->moveToThread(QCoreApplication::instance()->thread());
        }
    }

    Q_EMIT documentRead(mDocument.get());
}

}

#include "moc_testdocumentfileloadthread.cpp"
