/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfileloadthread.hpp"

// Kasten core
// #include <Kasten/Person>
// Okteta core
#include <Okteta/PieceTableByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QCoreApplication>
#include <QDataStream>
#include <QFile>
// C++
#include <limits>

namespace Kasten {

ByteArrayRawFileLoadThread::~ByteArrayRawFileLoadThread() = default;

void ByteArrayRawFileLoadThread::run()
{
    const qint64 fileSize = mFile->size();

    // check if the file content can be addressed with Okteta::Address
    const Okteta::Address maxAddress = std::numeric_limits<Okteta::Address>::max();

    bool success = (fileSize <= maxAddress);

    if (success) {
        // allocate working memory
        QByteArray data;
        data.resize(fileSize);
        bool success = (data.size() == fileSize);

        if (success) {
            QDataStream inStream(mFile);
            inStream.readRawData(data.data(), fileSize);

            success = (inStream.status() == QDataStream::Ok);

            if (success) {
                auto byteArray = std::make_unique<Okteta::PieceTableByteArrayModel>(data);
                byteArray->setModified(false);
                // TODO: make PieceTableByteArrayModel a child by constructor argument parent
                byteArray->moveToThread(QCoreApplication::instance()->thread());

                mDocument = std::make_unique<ByteArrayDocument>(std::move(byteArray), i18nc("destination of the byte array", "Loaded from file."));
//                 mDocument->setOwner(Person::createEgo());
                mDocument->moveToThread(QCoreApplication::instance()->thread());
            } else {
                mErrorString = mFile->errorString();
            }
        } else {
            mErrorString = i18n("There is not enough free working memory to load this file.");
        }
    } else {
        mErrorString = i18n("Support to load files larger than 2 GiB has not yet been implemented.");
    }

    Q_EMIT documentRead(mDocument.get());
}

}

#include "moc_bytearrayrawfileloadthread.cpp"
