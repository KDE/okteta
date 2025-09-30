/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraychar8stringcodecroundtriptest.hpp"

// test object
#include <bytearraychar8stringdecoder.hpp>
#include <bytearraychar8stringencoder.hpp>
// Okteta core
#include <Okteta/CharCodec>
// Qt
#include <QTest>
#include <QByteArray>

namespace Okteta {

QByteArray ByteArrayChar8StringCodecRoundtripTest::createFullByteSequence()
{
    static QByteArray sequence;
    if (sequence.isEmpty()) {
        sequence.resize(256);
        for (int i = 0; i < 256; ++i) {
            sequence[i] = static_cast<char>(i);
        }
    }
    return sequence;
}

void ByteArrayChar8StringCodecRoundtripTest::testCodecRoundtrip_data()
{
    QTest::addColumn<QString>("charCodecName");

    for (const QString& codecName : Okteta::CharCodec::codecNames()) {
        QTest::newRow(codecName.toLatin1().constData()) << codecName;
    }
}
void ByteArrayChar8StringCodecRoundtripTest::testCodecRoundtrip()
{
    QFETCH(const QString, charCodecName);

    Okteta::ByteArrayChar8StringDecoder decoder;
    Okteta::ByteArrayChar8StringEncoder encoder;

    decoder.setCharCodec(charCodecName);
    const auto* const charCodec = decoder.charCodec();

    const QByteArray sequence = createFullByteSequence();

    // to string
    const QString text = encoder.encodeAsString(sequence, charCodec);

    // and back
    QByteArray byteArray;
    int usedStringSize = -1;
    Okteta::ByteArrayChar8StringDecoder::CodeState decodeState = decoder.decode(&byteArray, text, 0, -1, &usedStringSize);

    QCOMPARE(decodeState, Okteta::ByteArrayChar8StringDecoder::CodeAcceptable);
    QCOMPARE(byteArray, sequence);
    QCOMPARE(usedStringSize, text.size());
}

}

QTEST_GUILESS_MAIN(Okteta::ByteArrayChar8StringCodecRoundtripTest)

#include "moc_bytearraychar8stringcodecroundtriptest.cpp"
