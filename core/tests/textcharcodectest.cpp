/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "textcharcodectest.hpp"

// test object
#include <codecs/textcharcodec.hpp>
// KF
#include <QTest>

namespace Okteta {

//---------------------------------------------------------------------------- Tests -----

void TextCharCodecTest::testCreateCodec_data()
{
    QTest::addColumn<QString>("codecName");

    for (const QString& codecName : TextCharCodec::codecNames()) {
        QTest::newRow(codecName.toLatin1().constData()) << codecName;
    }
}

void TextCharCodecTest::testCreateCodec()
{
    QFETCH(QString, codecName);

    CharCodec* codec = TextCharCodec::createCodec(codecName);

    QVERIFY(codec != nullptr);
    QCOMPARE(codec->name(), codecName);

    delete codec;
}

void TextCharCodecTest::testCreateLocalCodec()
{
    TextCharCodec* codec = TextCharCodec::createLocalCodec();
    QVERIFY(codec != nullptr);
    delete codec;
}

void TextCharCodecTest::testCreateNonexistingCodec()
{
    TextCharCodec* codec = TextCharCodec::createCodec(QStringLiteral("NonexistingCode"));
    QVERIFY(codec == nullptr);
}

}

QTEST_GUILESS_MAIN(Okteta::TextCharCodecTest)
