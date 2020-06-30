/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycharsstreamencoder.hpp"

// lib
#include <bytearrayview.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/Character>
#include <Okteta/CharCodec>
// KF
#include <KLocalizedString>
// Qt
#include <QTextStream>

namespace Kasten {

CharsStreamEncoderSettings::CharsStreamEncoderSettings() = default;

ByteArrayCharsStreamEncoder::ByteArrayCharsStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Characters"), QStringLiteral("text/plain"))
{}

ByteArrayCharsStreamEncoder::~ByteArrayCharsStreamEncoder() = default;

bool ByteArrayCharsStreamEncoder::encodeDataToStream(QIODevice* device,
                                                     const ByteArrayView* byteArrayView,
                                                     const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                     const Okteta::AddressRange& range)
{
    bool success = true;

    // settings
    mSettings.codecName = byteArrayView->charCodingName();
    mSettings.undefinedChar = byteArrayView->undefinedChar();
    mSettings.substituteChar = byteArrayView->substituteChar();

    // encode
    QTextStream textStream(device);

    Okteta::CharCodec* charCodec = Okteta::CharCodec::createCodec(mSettings.codecName);
    const QChar tabChar = QLatin1Char('\t');
    const QChar returnChar = QLatin1Char('\n');

    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        const Okteta::Character byteChar = charCodec->decode(byteArrayModel->byte(i));

        const QChar streamChar = byteChar.isUndefined() ?      mSettings.undefinedChar :
                                 (!byteChar.isPrint()
                                  || byteChar == tabChar
                                  || byteChar == returnChar) ? mSettings.substituteChar :
                                                               (QChar)byteChar;
        textStream << streamChar;
    }

    // clean up
    delete charCodec;

    return success;
}

}
