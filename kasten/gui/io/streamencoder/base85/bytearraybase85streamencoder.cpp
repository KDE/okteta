/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraybase85streamencoder.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QTextStream>

namespace Kasten {

static inline void streamEncoded(QTextStream& textStream, int& outputBytesPerLine,
                                 quint32 tuple, int inputByteCount)
{
    // radix85 values, most significant first
    char data[5];

    for (int i = 4; i >= 0; --i) {
        // TODO: find an efficient bit manipulating algorithm
        data[i] = tuple % 85;
        tuple /= 85;
    }

    // output inputByteCount+1 from radix85 values
    for (int i = 0; i <= inputByteCount; ++i) {
        textStream << (char)(data[i] + 33);
        ++outputBytesPerLine;
        if (outputBytesPerLine >= ByteArrayBase85StreamEncoder::maxOutputBytesPerLine) {
            textStream << '\n';
            outputBytesPerLine = 0;
        }
    }
}

// TODO: for now this is just the Adobe/Ascii85 implementation, so present as that
// later also add btoa with different version, e.g. 4.2 added a "y" for 4 spaces
ByteArrayBase85StreamEncoder::ByteArrayBase85StreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Ascii85"), QStringLiteral("text/x-ascii85"))
{}

ByteArrayBase85StreamEncoder::~ByteArrayBase85StreamEncoder() = default;

bool ByteArrayBase85StreamEncoder::encodeDataToStream(QIODevice* device,
                                                      const ByteArrayView* byteArrayView,
                                                      const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                      const Okteta::AddressRange& range)
{
    Q_UNUSED(byteArrayView);

    bool success = true;

    // encode
    QTextStream textStream(device);

    // prepare
    InputByteIndex inputByteIndex = InputByteIndex::First;
    quint32 tuple = 0;

    // header
    int outputBytesPerLine = 2;
    textStream << "<~";

    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        const Okteta::Byte byte = byteArrayModel->byte(i);

        switch (inputByteIndex)
        {
        case InputByteIndex::First:
            tuple |= (byte << 24);
            inputByteIndex = InputByteIndex::Second;
            break;
        case InputByteIndex::Second:
            tuple |= (byte << 16);
            inputByteIndex = InputByteIndex::Third;
            break;
        case InputByteIndex::Third:
            tuple |= (byte <<  8);
            inputByteIndex = InputByteIndex::Fourth;
            break;
        case InputByteIndex::Fourth:
            tuple |= byte;
            if (tuple == 0) {
                textStream << 'z';
                ++outputBytesPerLine;
                if (outputBytesPerLine >= maxOutputBytesPerLine) {
                    textStream << '\n';
                    outputBytesPerLine = 0;
                }
            } else {
                streamEncoded(textStream, outputBytesPerLine, tuple, static_cast<int>(inputByteIndex) + 1);
            }
            tuple = 0;
            inputByteIndex = InputByteIndex::First;
            break;
        }
    }

    const bool hasBitsLeft = (inputByteIndex != InputByteIndex::First);
    if (hasBitsLeft) {
        streamEncoded(textStream, outputBytesPerLine, tuple, static_cast<int>(inputByteIndex));
    }

    // footer
    if (outputBytesPerLine + 2 > maxOutputBytesPerLine) {
        textStream << '\n';
    }
    textStream << "~>\n";

    return success;
}

}
