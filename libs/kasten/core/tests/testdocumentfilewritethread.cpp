/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilewritethread.hpp"

// lib
#include "testdocument.hpp"
// Qt
#include <QDataStream>
#include <QFile>

namespace Kasten {

TestDocumentFileWriteThread::~TestDocumentFileWriteThread() = default;

void TestDocumentFileWriteThread::run()
{
    const QByteArray* byteArray = mDocument->data();

    QDataStream outStream(mFile);
    outStream.writeRawData(mHeader.data(), mHeader.size());
    outStream.writeRawData(byteArray->data(), byteArray->size());

//     byteArray->setModified( false );

    // registerDiskModifyTime( file );TODO move into synchronizer

    mSuccess = (outStream.status() == QDataStream::Ok);

    emit documentWritten(mSuccess);
}

}
