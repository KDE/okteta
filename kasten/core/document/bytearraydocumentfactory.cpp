/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraydocumentfactory.hpp"

// lib
#include "bytearraydocument.hpp"
// Okteta core
#include <Okteta/PieceTableByteArrayModel>
// Kasten core
// #include <Kasten/Person>
// KF
#include <KLocalizedString>
// Qt
#include <QMimeData>

namespace Kasten {

static int newByteArrayDocumentCounter = 0;

bool ByteArrayDocumentFactory::canCreateFromData(const QMimeData* mimeData)
{
    Q_UNUSED(mimeData);

    // we currently take everything, see createFromData()
    return true;
}

AbstractDocument* ByteArrayDocumentFactory::create()
{
    ByteArrayDocument* document = new ByteArrayDocument(i18nc("The byte array was new created.", "New created."));

    ++newByteArrayDocumentCounter;

    // TODO: use document->typeName() ?
    document->setTitle(
        i18ncp("numbered title for a created document without a filename",
               "[New Byte Array]", "[New Byte Array %1]", newByteArrayDocumentCounter));

//     document->setOwner(Person::createEgo());

    return document;
}

AbstractDocument* ByteArrayDocumentFactory::createFromData(const QMimeData* mimeData, bool setModified)
{
    if (!mimeData || mimeData->formats().isEmpty()) {
        return create();
    }

    // SYNC: with abstractbytearrayview_p.cpp
    // if there is a octet stream, use it, otherwise take the dump of the format
    // with the highest priority
    // TODO: this may not be, what is expected, think about it, if we just
    // take byte array descriptions, like encodings in chars or values
    // would need the movement of the encoders into the core library
    const QString octetStreamFormatName = QStringLiteral("application/octet-stream");
    const QString dataFormatName = (mimeData->hasFormat(octetStreamFormatName)) ?
                                   octetStreamFormatName :
                                   mimeData->formats()[0];

    const QByteArray data = mimeData->data(dataFormatName);

    auto* byteArray = new Okteta::PieceTableByteArrayModel(data);
    byteArray->setModified(setModified);

    // TODO: pass name of generator
    ByteArrayDocument* document = new ByteArrayDocument(byteArray, i18nc("origin of the byte array", "Created from data."));

    ++newByteArrayDocumentCounter;

    // TODO: use document->typeName() ?
    document->setTitle(
        i18ncp("numbered title for a created document without a filename",
               "[New Byte Array]", "[New Byte Array %1]", newByteArrayDocumentCounter));

//     document->setOwner(Person::createEgo());

    return document;
}

}
