/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfilewritethread.hpp"

// lib
#include "bytearraydocument.hpp"
// Okteta core
#include <Okteta/PieceTableByteArrayModel>
// Qt
#include <QDataStream>
#include <QFile>

namespace Kasten {

ByteArrayRawFileWriteThread::~ByteArrayRawFileWriteThread() = default;

void ByteArrayRawFileWriteThread::run()
{
    auto* byteArray = qobject_cast<Okteta::PieceTableByteArrayModel*>(mDocument->content());

    QDataStream outStream(mFile);

    // TODO: this was
//     outStream.writeRawData( byteArray->data(), byteArray->size() );
    // make it quicker again by writing spans -> spaniterator

    for (int i = 0; i < byteArray->size(); ++i) {
        const Okteta::Byte byte = byteArray->byte(i);
        outStream.writeRawData(reinterpret_cast<const char*>(&byte), 1);
    }

    byteArray->setModified(false);

    mSuccess = (outStream.status() == QDataStream::Ok);

    emit documentWritten(mSuccess);
}

}
