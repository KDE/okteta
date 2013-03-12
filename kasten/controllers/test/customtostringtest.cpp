/*
 *  This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *  Copyright 2013  Alex Richardson <alex.richardson@gmx.de>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include <QtTest/QtTest>
#include <QScriptEngine>
#include <KGlobal>
#include <KStandardDirs>
#include "view/structures/script/scriptengineinitializer.h"
#include "view/structures/parsers/scriptvalueconverter.h"
#include "view/structures/datatypes/topleveldatainformation.h"
#include "testutils.h"
#include <bytearraymodel.h>

class CustomToStringTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testUuid_data();
    void testUuid();
};

void CustomToStringTest::initTestCase()
{
    //needed so that imports can be resolved
    QVERIFY(KGlobal::dirs()->addResourceDir("data", QLatin1String(SRCDIR "/resources")));
}

static char uuid1[16] =
{
    0x55,0x0e,0x84,0x00,0xe2,0x9b,0x41,0xd4,0xa7,0x16,0x44,0x66,0x55,0x44,0x00,0x00
};

static char uuid2[16] =
{
    0x3f,0x25,0x04,0xe0,0x4f,0x89,0x11,0xd3,0x9a,0x0c,0x03,0x05,0xe8,0x2c,0x33,0x01
};

static char nullUuid[16] =
{
    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
};

void CustomToStringTest::testUuid_data()
{
    QTest::addColumn<QString>("type");
    QTest::addColumn<QString>("uuidString");
    QTest::addColumn<QByteArray>("data");
    QTest::newRow("uuid1") << "UUID" << "{550e8400-e29b-41d4-a716-446655440000}" << QByteArray::fromRawData(uuid1, 16); //from wikipedia
    QTest::newRow("uuid2") << "UUID" << "{3f2504e0-4f89-11d3-9a0c-0305e82c3301}"<< QByteArray::fromRawData(uuid2, 16); //from wikipedia
    QTest::newRow("null uuid") << "UUID" << "{00000000-0000-0000-0000-000000000000}"<< QByteArray::fromRawData(nullUuid, 16);

    //now the same just as a Microsoft GUID
    QTest::newRow("guid1") << "GUID" << "{550e8400-e29b-41d4-a716-446655440000}" << QByteArray::fromRawData(uuid1, 16); //from wikipedia
    QTest::newRow("guid2") << "GUID" << "{3f2504e0-4f89-11d3-9a0c-0305e82c3301}"<< QByteArray::fromRawData(uuid2, 16); //from wikipedia
    QTest::newRow("null guid") << "GUID" << "{00000000-0000-0000-0000-000000000000}"<< QByteArray::fromRawData(nullUuid, 16);
}

void CustomToStringTest::testUuid()
{
    QFETCH(QByteArray, data);
    QFETCH(QString, uuidString);
    QFETCH(QString, type);
    QUuid uuid = QUuid::fromRfc4122(data);
    QCOMPARE(uuid.toString(), uuidString);
    QScriptEngine* eng = ScriptEngineInitializer::newEngine();
    ScriptLogger* logger = new ScriptLogger();
    logger->setLogToStdOut(true);
    const QString codeBase = QLatin1String("var u = importScript('uuid.js'); u.%1();");
    DataInformation* structure = Utils::evalAndParse(eng, codeBase.arg(type), logger);
    QVERIFY(structure);
    TopLevelDataInformation top(structure, logger, eng);
    QCOMPARE(structure->childCount(), 4u);
    QVERIFY(structure->toStringFunction().isFunction());
    Okteta::ByteArrayModel model(reinterpret_cast<const uchar*>(data.constData()), data.size());
    model.setAutoDelete(false);
    top.read(&model, 0, Okteta::ArrayChangeMetricsList(), true);
    QCOMPARE(structure->valueString(), uuidString);
    QCOMPARE(structure->typeName(), type);
}

QTEST_MAIN(CustomToStringTest)

#include "customtostringtest.moc"
