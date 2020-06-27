/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfileloadthread.hpp"

// lib
#include "bytearraydocument.hpp"
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
                auto* byteArray = new Okteta::PieceTableByteArrayModel(data);
                byteArray->setModified(false);

                mDocument = new ByteArrayDocument(byteArray, i18nc("destination of the byte array", "Loaded from file."));
//                 mDocument->setOwner(Person::createEgo());
                // TODO: make PieceTableByteArrayModel a child by constructor argument parent
                byteArray->moveToThread(QCoreApplication::instance()->thread());
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

    if (!success) {
        mDocument = nullptr;
    }

    emit documentRead(mDocument);
}

}
