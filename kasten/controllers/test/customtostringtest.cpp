/*
 *  This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *  SPDX-FileCopyrightText: 2013 Alex Richardson <alex.richardson@gmx.de>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QTest>
#include <QScriptEngine>

#include "view/structures/script/scriptengineinitializer.hpp"
#include "view/structures/parsers/scriptvalueconverter.hpp"
#include "view/structures/datatypes/topleveldatainformation.hpp"
#include "testutils.hpp"
#include <Okteta/ByteArrayModel>

namespace Okteta {
using TextStreamFunction = QTextStream& (*)(QTextStream&);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
constexpr TextStreamFunction hex = Qt::hex;
#else
constexpr TextStreamFunction hex = ::hex;
#endif
}

class CustomToStringTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testUuid_data();
    void testUuid();
};

static uchar uuid1[16] =
{
    0x55, 0x0e, 0x84, 0x00, 0xe2, 0x9b, 0x41, 0xd4, 0xa7, 0x16, 0x44, 0x66, 0x55, 0x44, 0x00, 0x00
};

static uchar uuid2[16] =
{
    0x3f, 0x25, 0x04, 0xe0, 0x4f, 0x89, 0x11, 0xd3, 0x9a, 0x0c, 0x03, 0x05, 0xe8, 0x2c, 0x33, 0x01
};

static uchar nullUuid[16] =
{
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

void CustomToStringTest::initTestCase()
{
    // needed so that imports can be resolved
    QString examples = QFINDTESTDATA("../view/structures/examples");
    QVERIFY2(!examples.isEmpty(), "Test data must exist!");
    qputenv("XDG_DATA_DIRS", QFile::encodeName(QFileInfo(examples).absoluteFilePath()));
}

void CustomToStringTest::testUuid_data()
{
    QTest::addColumn<bool>("isGUID");
    QTest::addColumn<QString>("uuidString");
    QTest::addColumn<QByteArray>("data");
    QTest::newRow("uuid1") << false << "{550e8400-e29b-41d4-a716-446655440000}"
                           << QByteArray::fromRawData(reinterpret_cast<char*>(uuid1), sizeof(uuid1));
    QTest::newRow("uuid2") << false << "{3f2504e0-4f89-11d3-9a0c-0305e82c3301}"
                           << QByteArray::fromRawData(reinterpret_cast<char*>(uuid2), sizeof(uuid2));
    QTest::newRow("null uuid") << false << "{00000000-0000-0000-0000-000000000000}"
                               << QByteArray::fromRawData(reinterpret_cast<char*>(nullUuid), sizeof(nullUuid));

    // now the same just as a Microsoft GUID
    QTest::newRow("guid1") << true << "{00840e55-9be2-d441-a716-446655440000}"
                           << QByteArray::fromRawData(reinterpret_cast<char*>(uuid1), sizeof(uuid1));
    QTest::newRow("guid2") << true << "{e004253f-894f-d311-9a0c-0305e82c3301}"
                           << QByteArray::fromRawData(reinterpret_cast<char*>(uuid2), sizeof(uuid2));
    QTest::newRow("null guid") << true << "{00000000-0000-0000-0000-000000000000}"
                               << QByteArray::fromRawData(reinterpret_cast<char*>(nullUuid), sizeof(nullUuid));
}

void CustomToStringTest::testUuid()
{
    QFETCH(QByteArray, data);
    QFETCH(QString, uuidString);
    QFETCH(bool, isGUID);
    QScriptEngine* eng = ScriptEngineInitializer::newEngine();
    auto* logger = new ScriptLogger();
    logger->setLogToStdOut(true);
    DataInformation* structure = nullptr;
    if (isGUID) {
        structure = Utils::evalAndParse(eng, "var u = importScript('uuid.js'); u.GUID();", logger);
    } else {
        structure = Utils::evalAndParse(eng, "var u = importScript('uuid.js'); u.UUID();", logger);
    }
#if defined(Q_OS_MACOS) || defined(Q_OS_WINDOWS)
  QEXPECT_FAIL("", "QStandardPaths::GenericDataLocation can't be modified on macOS/Windows", Abort);
#endif
    QVERIFY(structure);
    TopLevelDataInformation top(structure, logger, eng);
    QCOMPARE(structure->childCount(), 4u);

    QVERIFY(structure->toStringFunction().isFunction());
    Okteta::ByteArrayModel model(reinterpret_cast<const uchar*>(data.constData()), data.size());
    model.setAutoDelete(false);
    top.read(&model, 0, Okteta::ArrayChangeMetricsList(), true);

    QCOMPARE(structure->childAt(0)->effectiveByteOrder(),
             isGUID ? QSysInfo::LittleEndian : QSysInfo::BigEndian);
    QCOMPARE(structure->childAt(1)->effectiveByteOrder(),
             isGUID ? QSysInfo::LittleEndian : QSysInfo::BigEndian);
    QCOMPARE(structure->childAt(2)->effectiveByteOrder(),
             isGUID ? QSysInfo::LittleEndian : QSysInfo::BigEndian);
    bool ok;
    quint32 val1 = uuidString.midRef(1, 8).toUInt(&ok, 16);
    QVERIFY(ok);
    quint16 val2 = uuidString.midRef(10, 4).toUShort(&ok, 16);
    QVERIFY(ok);
    quint16 val3 = uuidString.midRef(15, 4).toUShort(&ok, 16);
    QVERIFY(ok);
    qDebug() << Okteta::hex << val1 << val2 << val3;
    QCOMPARE(structure->childAt(0)->asPrimitive()->value().value<quint32>(), val1);
    QCOMPARE(structure->childAt(1)->asPrimitive()->value().value<quint16>(), val2);
    QCOMPARE(structure->childAt(2)->asPrimitive()->value().value<quint16>(), val3);

    QString typeStr = isGUID ? QStringLiteral("GUID") : QStringLiteral("UUID");
    QCOMPARE(structure->typeName(), typeStr);
    QCOMPARE(structure->valueString(), uuidString);
}

QTEST_GUILESS_MAIN(CustomToStringTest)

#include "customtostringtest.moc"
