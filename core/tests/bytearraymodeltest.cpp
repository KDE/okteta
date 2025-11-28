/*
    SPDX-FileCopyrightText: 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraymodeltest.hpp"

// test object
#include <bytearraymodel.hpp>
// lib
#include <arraychangemetricslist.hpp>
// Qt
#include <QSignalSpy>
#include <QTest>

Q_DECLARE_METATYPE(Okteta::ArrayChangeMetricsList)
// work-around, see addRow()
Q_DECLARE_METATYPE(std::shared_ptr<Okteta::ByteArrayModel>)

namespace Okteta {

static const Byte constRawEmpty[1] = {}; // ISO C++ forbids zero-size array, use unused 1 length
static const Byte constRawOne[1] = {'\0'};
static const Byte constRawFive[5] = {'\0', '\0', '\0', '\0', '\0'};
static const Byte constRawTen[10] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
static const QByteArray constByteArrayEmpty;
static const QByteArray constByteArrayOne = QByteArray(1, '\0');
static const QByteArray constByteArrayFive = QByteArray(5, '\0');
static const QByteArray constByteArrayTen = QByteArray(10, '\0');
static const std::vector<Okteta::Byte> constVectorEmpty;
static const std::vector<Okteta::Byte> constVectorOne = {'\0'};
static const std::vector<Okteta::Byte> constVectorFive = {'\0', '\0', '\0', '\0', '\0'};
static const std::vector<Okteta::Byte> constVectorTen = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
static const std::array<Okteta::Byte, 0> constArrayEmpty;
static const std::array<Okteta::Byte, 1> constArrayOne = {'\0'};
static const std::array<Okteta::Byte, 5> constArrayFive = {'\0', '\0', '\0', '\0', '\0'};
static const std::array<Okteta::Byte, 10> constArrayTen = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

static Byte rawEmpty[1] = {}; // ISO C++ forbids zero-size array, use unused 1 length
static Byte rawOne[1] = {'\0'};
static Byte rawFive[5] = {'\0', '\0', '\0', '\0', '\0'};
static Byte rawTen[10] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
static std::vector<Okteta::Byte> vectorEmpty;
static std::vector<Okteta::Byte> vectorOne = {'\0'};
static std::vector<Okteta::Byte> vectorFive = {'\0', '\0', '\0', '\0', '\0'};
static std::vector<Okteta::Byte> vectorTen = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
static std::array<Okteta::Byte, 0> arrayEmpty;
static std::array<Okteta::Byte, 1> arrayOne = {'\0'};
static std::array<Okteta::Byte, 5> arrayFive = {'\0', '\0', '\0', '\0', '\0'};
static std::array<Okteta::Byte, 10> arrayTen = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};

void ByteArrayModelTest::init()
{
    qRegisterMetaType<ArrayChangeMetricsList>("Okteta::ArrayChangeMetricsList");
}

enum MaxSizeBound { NoMaxSizeBound, CapacityMaxSizeBound };
void testMaxSize_dataRows(const char* name, MaxSizeBound maxSizeBound,
                          std::unique_ptr<ByteArrayModel>&& empty, std::unique_ptr<ByteArrayModel>&& one,
                          std::unique_ptr<ByteArrayModel>&& five, std::unique_ptr<ByteArrayModel>&& ten)
{
    const bool isBounded = (maxSizeBound == CapacityMaxSizeBound);
    // work-around for QTest::newRow only supporting QMetaType-registered types, though needing a copy constructor
    // using shared pointer to transport to test and have life-time still managed
    QTest::newRow(QStringLiteral("%1-on-empty").arg(QLatin1String(name)).toLatin1().constData())
        << std::shared_ptr<ByteArrayModel>(std::move(empty)) << 5 << (isBounded ? 0 : 5);
    QTest::newRow(QStringLiteral("%1-larger").arg(QLatin1String(name)).toLatin1().constData())
        << std::shared_ptr<ByteArrayModel>(std::move(one)) << 5 << (isBounded ? 1 : 5);
    QTest::newRow(QStringLiteral("%1-same").arg(QLatin1String(name)).toLatin1().constData())
        << std::shared_ptr<ByteArrayModel>(std::move(five)) << 5 << 5;
    QTest::newRow(QStringLiteral("%1-smaller").arg(QLatin1String(name)).toLatin1().constData())
        << std::shared_ptr<ByteArrayModel>(std::move(ten)) << 5 << 5;
}

void ByteArrayModelTest::testMaxSize_data()
{
    QTest::addColumn<std::shared_ptr<ByteArrayModel>>("byteArrayModel");
    QTest::addColumn<int>("toSetMaxSize");
    QTest::addColumn<int>("expectedMaxSize");

    testMaxSize_dataRows("const-raw", CapacityMaxSizeBound,
                         std::make_unique<ByteArrayModel>(constRawEmpty, 0),
                         std::make_unique<ByteArrayModel>(constRawOne, 1),
                         std::make_unique<ByteArrayModel>(constRawFive, 5),
                         std::make_unique<ByteArrayModel>(constRawTen, 10));

    testMaxSize_dataRows("const-qbytearray", CapacityMaxSizeBound,
                         std::make_unique<ByteArrayModel>(constByteArrayEmpty),
                         std::make_unique<ByteArrayModel>(constByteArrayOne),
                         std::make_unique<ByteArrayModel>(constByteArrayFive),
                         std::make_unique<ByteArrayModel>(constByteArrayTen)),

    testMaxSize_dataRows("const-vector", CapacityMaxSizeBound,
                         std::make_unique<ByteArrayModel>(constVectorEmpty),
                         std::make_unique<ByteArrayModel>(constVectorOne),
                         std::make_unique<ByteArrayModel>(constVectorFive),
                         std::make_unique<ByteArrayModel>(constVectorTen));

    testMaxSize_dataRows("const-array", CapacityMaxSizeBound,
                         std::make_unique<ByteArrayModel>(constArrayEmpty),
                         std::make_unique<ByteArrayModel>(constArrayOne),
                         std::make_unique<ByteArrayModel>(constArrayFive),
                         std::make_unique<ByteArrayModel>(constArrayTen));

    testMaxSize_dataRows("raw", CapacityMaxSizeBound,
                         std::make_unique<ByteArrayModel>(rawEmpty, 0),
                         std::make_unique<ByteArrayModel>(rawOne, 1),
                         std::make_unique<ByteArrayModel>(rawFive, 5),
                         std::make_unique<ByteArrayModel>(rawTen, 10));

    testMaxSize_dataRows("vector", CapacityMaxSizeBound,
                         std::make_unique<ByteArrayModel>(vectorEmpty),
                         std::make_unique<ByteArrayModel>(vectorOne),
                         std::make_unique<ByteArrayModel>(vectorFive),
                         std::make_unique<ByteArrayModel>(vectorTen));

    testMaxSize_dataRows("array", CapacityMaxSizeBound,
                         std::make_unique<ByteArrayModel>(arrayEmpty),
                         std::make_unique<ByteArrayModel>(arrayOne),
                         std::make_unique<ByteArrayModel>(arrayFive),
                         std::make_unique<ByteArrayModel>(arrayTen));

    testMaxSize_dataRows("owned", NoMaxSizeBound,
                         std::make_unique<ByteArrayModel>(std::make_unique<Okteta::Byte[]>(0), 0),
                         std::make_unique<ByteArrayModel>(std::make_unique<Okteta::Byte[]>(1), 1),
                         std::make_unique<ByteArrayModel>(std::make_unique<Okteta::Byte[]>(5), 5),
                         std::make_unique<ByteArrayModel>(std::make_unique<Okteta::Byte[]>(10), 10));

    testMaxSize_dataRows("create", NoMaxSizeBound,
                         std::make_unique<ByteArrayModel>(),
                         std::make_unique<ByteArrayModel>(1),
                         std::make_unique<ByteArrayModel>(5),
                         std::make_unique<ByteArrayModel>(10));
}

void ByteArrayModelTest::testMaxSize()
{
    QFETCH(std::shared_ptr<ByteArrayModel>, byteArrayModel);
    QFETCH(int, toSetMaxSize);
    QFETCH(int, expectedMaxSize);

    QSignalSpy contentsChangeListSpy(byteArrayModel.get(), &AbstractByteArrayModel::contentsChanged);
    const int sizeDiff = (byteArrayModel->size() - expectedMaxSize);
    const bool isSizeReduced = (sizeDiff > 0);

    // before
    QCOMPARE(byteArrayModel->maxSize(), -1);

    // set limit
    byteArrayModel->setMaxSize(toSetMaxSize);

    QCOMPARE(byteArrayModel->maxSize(), expectedMaxSize);

    QVERIFY(contentsChangeListSpy.isValid());
    if (isSizeReduced) {
        QCOMPARE(contentsChangeListSpy.size(), 1);
        const QList<QVariant> arguments = contentsChangeListSpy.takeFirst();
        const ArrayChangeMetrics changeMetrics = (qvariant_cast<ArrayChangeMetricsList>(arguments.at(0))).at(0);
        QCOMPARE(changeMetrics.offset(), expectedMaxSize);
        QCOMPARE(changeMetrics.removeLength(), sizeDiff);
        QCOMPARE(changeMetrics.insertLength(), 0);
    } else {
        QCOMPARE(contentsChangeListSpy.size(), 0);
    }

    // unset limit
    byteArrayModel->setMaxSize(-1);

    QCOMPARE(byteArrayModel->maxSize(), -1);
}

void testReleaseData_dataRow(const char* name, std::unique_ptr<ByteArrayModel>&& model)
{
    // work-around for QTest::newRow only supporting QMetaType-registered types, though needing a copy constructor
    // using shared pointer to transport to test and have life-time still managed
    QTest::newRow(name) << std::shared_ptr<ByteArrayModel>(std::move(model));
}

static constexpr Byte startCanary = 'a';
static constexpr int startCanaryOffset = 0;

void ByteArrayModelTest::testReleaseData_data()
{
    QTest::addColumn<std::shared_ptr<ByteArrayModel>>("byteArrayModel");

    // create raw data, mark a byte at begin
    auto rawData = std::make_unique<Okteta::Byte[]>(10);
    rawData[startCanaryOffset] = startCanary;
    testReleaseData_dataRow("owned", std::make_unique<ByteArrayModel>(std::move(rawData), 10));

    // use mark for all bytes, so also first one
    testReleaseData_dataRow("create", std::make_unique<ByteArrayModel>(10, startCanary));
}

void ByteArrayModelTest::testReleaseData()
{
    QFETCH(std::shared_ptr<ByteArrayModel>, byteArrayModel);

    QCOMPARE(byteArrayModel->size(), 10);
    QCOMPARE(byteArrayModel->capacity(), 10);
    QCOMPARE(byteArrayModel->maxSize(), -1);

    // mark a byte via the model
    const Byte endCanary = 'o';
    const int endCanaryOffset = 9;
    byteArrayModel->setByte(endCanaryOffset, endCanary);

    // setup more test utils
    QSignalSpy contentsChangeListSpy(byteArrayModel.get(), &AbstractByteArrayModel::contentsChanged);

    // now do the tested action
    auto takenData = byteArrayModel->releaseData();

    // check new state of byteArrayModel
    QCOMPARE(byteArrayModel->size(), 0);
    QCOMPARE(byteArrayModel->capacity(), 0);
    QCOMPARE(byteArrayModel->maxSize(), -1);
    QCOMPARE(byteArrayModel->isModified(), false);

    // check change signals
    QVERIFY(contentsChangeListSpy.isValid());
    QCOMPARE(contentsChangeListSpy.size(), 1);
    const QList<QVariant> arguments = contentsChangeListSpy.takeFirst();
    const ArrayChangeMetrics changeMetrics = (qvariant_cast<ArrayChangeMetricsList>(arguments.at(0))).at(0);
    QCOMPARE(changeMetrics.offset(), 0);
    QCOMPARE(changeMetrics.removeLength(), 10);
    QCOMPARE(changeMetrics.insertLength(), 0);

    // check taken data
    QVERIFY(takenData);
    QCOMPARE(takenData[startCanaryOffset], startCanary);
    QCOMPARE(takenData[endCanaryOffset], endCanary);
}

}

QTEST_GUILESS_MAIN(Okteta::ByteArrayModelTest)

#include "moc_bytearraymodeltest.cpp"
