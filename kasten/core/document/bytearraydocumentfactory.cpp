/*
    SPDX-FileCopyrightText: 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraydocumentfactory.hpp"

// lib
#include "bytearraydocument.hpp"
// liboktetawidgets
#include <bytearraychar8stringdecoder.hpp>
#include <bytearrayvaluestringdecoder.hpp>
// Okteta core
#include <Okteta/PieceTableByteArrayModel>
#include <Okteta/ValueCodec>
// Kasten core
// #include <Kasten/Person>
// KF
#include <KLocalizedString>
// Qt
#include <QMimeData>
// Std
#include <utility>

namespace Kasten {

static int newByteArrayDocumentCounter = 0;

bool ByteArrayDocumentFactory::canCreateFromData(const QMimeData* mimeData)
{
    Q_UNUSED(mimeData)

    // we currently take everything, see createFromData()
    return true;
}

std::unique_ptr<AbstractDocument> ByteArrayDocumentFactory::create()
{
    auto document = std::make_unique<ByteArrayDocument>(i18nc("The byte array was new created.", "New created."));

    ++newByteArrayDocumentCounter;

    // TODO: use document->typeName() ?
    document->setTitle(
        i18ncp("numbered title for a created document without a filename",
               "[New Byte Array]", "[New Byte Array %1]", newByteArrayDocumentCounter));

//     document->setOwner(Person::createEgo());

    return document;
}

std::unique_ptr<AbstractDocument> ByteArrayDocumentFactory::createFromData(const QMimeData* mimeData, bool setModified)
{
    if (!mimeData || mimeData->formats().isEmpty()) {
        return create();
    }

    // SYNC: with abstractbytearrayview_p.cpp
    // If there is an octet stream, use it, otherwise try to decode values or chars,
    // assuming byte array descriptions by text.
    // If that fails, take the dump of the format with the highest priority.
    // TODO: instead of guessing, ask user for the decoding to try.
    // would need the movement of the encoders into the core library
    QByteArray bytes;

    const QString octetStreamFormatName = QStringLiteral("application/octet-stream");
    if (mimeData->hasFormat(octetStreamFormatName)) {
        bytes = mimeData->data(octetStreamFormatName);
    } else {
        // try to parse any encoding in text
        if (mimeData->hasText()) {
            const QString text = mimeData->text();
            // first try values
            // for now hard-code support only for hexadecimal codes
            // TODO: query user for format to try, or perhaps do auto-detect where feasible?
            auto valueCodec = Okteta::ValueCodec::createCodec(Okteta::HexadecimalCoding);

            Okteta::ByteArrayValueStringDecoder valueStringDecoder(valueCodec.get());
            const Okteta::ByteArrayValueStringDecoder::CodeState valuesEvalResult = valueStringDecoder.decode(&bytes, text);
            if (valuesEvalResult != Okteta::ByteArrayValueStringDecoder::CodeAcceptable) {
                bytes.clear();
            }

            // then try escaped text
            // for now hard-code support only for local 8-bit/ISO-8859-1 charset
            if (bytes.isEmpty()) {
                Okteta::ByteArrayChar8StringDecoder char8StringDecoder;
                int usedTextSize = -1;
                const Okteta::ByteArrayChar8StringDecoder::CodeState charEvalResult = char8StringDecoder.decode(&bytes, text, 0, -1, &usedTextSize);
                if ((charEvalResult != Okteta::ByteArrayChar8StringDecoder::CodeAcceptable) ||
                    (usedTextSize != text.size())) {
                    bytes.clear();
                }
            }
        }

        // nothing decoded (for now empty -> failed)? fall back to raw data of first format
        if (bytes.isEmpty()) {
            const QString firstDataFormatName = mimeData->formats()[0];
            bytes = mimeData->data(firstDataFormatName);
        }
    }

    if (bytes.isEmpty()) {
        return nullptr;
    }

    auto byteArray = std::make_unique<Okteta::PieceTableByteArrayModel>(bytes);
    byteArray->setModified(setModified);

    // TODO: pass name of generator
    auto document = std::make_unique<ByteArrayDocument>(std::move(byteArray), i18nc("origin of the byte array", "Created from data."));

    ++newByteArrayDocumentCounter;

    // TODO: use document->typeName() ?
    document->setTitle(
        i18ncp("numbered title for a created document without a filename",
               "[New Byte Array]", "[New Byte Array %1]", newByteArrayDocumentCounter));

//     document->setOwner(Person::createEgo());

    return document;
}

}

#include "moc_bytearraydocumentfactory.cpp"
