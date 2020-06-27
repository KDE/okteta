/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "testdocumentfilereloadthread.hpp"

// lib
#include "testdocument.hpp"
// KF
#include <KLocalizedString>
// Qt
#include <QUrl>
#include <QDataStream>
#include <QFile>

namespace Kasten {

TestDocumentFileReloadThread::TestDocumentFileReloadThread(QObject* parent, const QByteArray& header,
                                                           /*TestDocument* document,*/ QFile* file)
    : QThread(parent)
//     , mDocument(document)
    , mHeader(header)
    , mFile(file)
{
//     mDocument->content()->moveToThread( this );
//     mDocument->moveToThread( this );
}

TestDocumentFileReloadThread::~TestDocumentFileReloadThread() = default;

void TestDocumentFileReloadThread::run()
{
    QDataStream inStream(mFile);
    const int fileSize = mFile->size();

    // test header
    const int headerSize = mHeader.size();
    QByteArray header(headerSize, ' ');
    const int headerResult = inStream.readRawData(header.data(), headerSize);
    if (headerResult == -1 || header != mHeader) {
        mSuccess = false;
    } else {
        mByteArray = QByteArray(fileSize, ' ');

        inStream.readRawData(mByteArray.data(), fileSize);

        mSuccess = (inStream.status() == QDataStream::Ok);
    }

    emit documentReloaded(mSuccess);
}

}
