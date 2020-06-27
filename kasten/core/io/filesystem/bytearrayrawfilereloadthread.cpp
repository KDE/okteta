/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfilereloadthread.hpp"

// Okteta core
#include <Okteta/PieceTableByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QDataStream>
#include <QFile>
// C++
#include <limits>

namespace Kasten {

ByteArrayRawFileReloadThread::ByteArrayRawFileReloadThread(QObject* parent, QFile* file)
    : QThread(parent)
    , mFile(file)
{
}

ByteArrayRawFileReloadThread::~ByteArrayRawFileReloadThread() = default;

void ByteArrayRawFileReloadThread::run()
{
    const qint64 fileSize = mFile->size();

    // check if the file content can be addressed with int
    // check if the file content can be addressed with Okteta::Address
    const Okteta::Address maxAddress = std::numeric_limits<Okteta::Address>::max();

    mSuccess = (fileSize <= maxAddress);

    if (mSuccess) {
        mData.resize(fileSize);
        mSuccess = (mData.size() == fileSize);

        if (mSuccess) {
            QDataStream inStream(mFile);
            inStream.readRawData(mData.data(), fileSize);

            mSuccess = (inStream.status() == QDataStream::Ok);

            if (!mSuccess) {
                mErrorString = mFile->errorString();
            }
        } else {
            mErrorString = i18n("There is not enough free working memory to load this file.");
        }
    } else {
        mErrorString = i18n("Support to load files larger than 2 GiB has not yet been implemented.");
    }

    emit documentReloaded(mSuccess);
}

}
