/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetabletest.hpp"

// test object
#include <piecetable/piecetable.hpp>
// Qt
#include <QVector>
#include <QTest>

namespace KPieceTable {
struct StorageDataTestData
{
    Address dataOffset;

    bool expectedResult;
    Address expectedStorageOffset;
    int expectedStorageId;
};
}

Q_DECLARE_METATYPE(QVector<KPieceTable::StorageDataTestData>)
Q_DECLARE_METATYPE(KPieceTable::AddressRange)

namespace KPieceTable {

// local variables
static constexpr Address Start = 15;
static constexpr Address End = 27;
static constexpr Size HalfBaseSize = 50;
static constexpr Size BaseSize = 2 * HalfBaseSize;

static constexpr Size Width = End - Start + 1;

static constexpr Address ChangeStart = 8;
static constexpr Address ChangeEnd = ChangeStart + Width - 1;

void PieceTableTest::testSimpleConstructor()
{
    PieceTable pieceTable;
    QCOMPARE(pieceTable.size(), 0);
}

void PieceTableTest::testInit()
{
    PieceTable pieceTable;

    pieceTable.init(BaseSize);
    int storageId;
    Address storageOffset;

    bool result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(result);
    QCOMPARE(storageOffset, 0);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Start);
    QVERIFY(result);
    QCOMPARE(storageOffset, Start);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, End);
    QVERIFY(result);
    QCOMPARE(storageOffset, End);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize);
    QVERIFY(!result);
}

static void fillWithSize(PieceTable* pieceTable, int count)
{
    pieceTable->init(0);
    for (int i = 0; i < count; ++i) {
        pieceTable->insert(0, BaseSize, BaseSize * i);
    }

}

void PieceTableTest::testInsert()
{
    PieceTable pieceTable;

    int storageId;
    Address storageOffset;
    bool result;

    // inserting to empty
    pieceTable.init(0);
    pieceTable.insert(0, Width, ChangeStart);

    QCOMPARE(pieceTable.size(), Width);
    result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeStart);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeEnd);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Width);
    QVERIFY(!result);

    // inserting one at the begin
    pieceTable.init(BaseSize);
    pieceTable.insert(0, Width, ChangeStart);

    QCOMPARE(pieceTable.size(), BaseSize + Width);
    result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeStart);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeEnd);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Width);
    QVERIFY(result);
    QCOMPARE(storageOffset, 0);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width);
    QVERIFY(!result);

    // inserting one in the middle
    pieceTable.init(BaseSize);
    pieceTable.insert(Start, Width, ChangeStart);

    QCOMPARE(pieceTable.size(), BaseSize + Width);
    result = pieceTable.getStorageData(&storageId, &storageOffset, Start - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, Start - 1);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Start);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeStart);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, End);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeEnd);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, End + 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, Start);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width);
    QVERIFY(!result);

    // inserting one at the end
    pieceTable.init(BaseSize);
    pieceTable.insert(BaseSize, Width, ChangeStart);

    QCOMPARE(pieceTable.size(), BaseSize + Width);
    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeStart);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeEnd);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width);
    QVERIFY(!result);

    // inserting a lot:
    // inserting before, after and in another inserted section, two recursions
    // 121110  8 7 6  4 3 2
    // \/\/\/ \/\/\/ \/\/\/
    //  |-9-|  |-5-|  |-1-|
    //  \  /   \  /   \  /
    //   \/     \/     \/
    //    |-----0-------|
    // TODO: do this also in forward sequences
    pieceTable.init(BaseSize);   // 0
    const Address Mid = BaseSize / 2;
    Address changeStarts[13];
    Address changeStart = 0;
    changeStarts[1] = changeStart;
    pieceTable.insert(BaseSize, BaseSize, changeStart);   // 1
    changeStart += BaseSize;
    changeStarts[2] = changeStart;
    pieceTable.insert(BaseSize + BaseSize, BaseSize, changeStart); // 2
    changeStart += BaseSize;
    changeStarts[3] = changeStart;
    pieceTable.insert(BaseSize + Mid, BaseSize, changeStart); // 3
    changeStart += BaseSize;
    changeStarts[4] = changeStart;
    pieceTable.insert(BaseSize, BaseSize, changeStart);   // 4

    changeStart += BaseSize;
    changeStarts[5] = changeStart;
    pieceTable.insert(Mid, BaseSize, changeStart);   // 5
    changeStart += BaseSize;
    changeStarts[6] = changeStart;
    pieceTable.insert(Mid + BaseSize, BaseSize, changeStart); // 6
    changeStart += BaseSize;
    changeStarts[7] = changeStart;
    pieceTable.insert(Mid + Mid, BaseSize, changeStart); // 7
    changeStart += BaseSize;
    changeStarts[8] = changeStart;
    pieceTable.insert(Mid, BaseSize, changeStart);   // 8

    changeStart += BaseSize;
    changeStarts[9] = changeStart;
    pieceTable.insert(0, BaseSize, changeStart);   // 9
    changeStart += BaseSize;
    changeStarts[10] = changeStart;
    pieceTable.insert(BaseSize, BaseSize, changeStart);   // 10
    changeStart += BaseSize;
    changeStarts[11] = changeStart;
    pieceTable.insert(Mid, BaseSize, changeStart);   // 11
    changeStart += BaseSize;
    changeStarts[12] = changeStart;
    pieceTable.insert(0, BaseSize, changeStart);   // 12

    // test
    QCOMPARE(pieceTable.size(), BaseSize + 12 * BaseSize);
    // all borders
    // 12: begin
    Address byteArrayOffset = 0;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[12]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 12: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[12] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[9]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 11: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[9] + HalfBaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[11]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 11: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[11] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[9] + HalfBaseSize);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 10: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[9] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[10]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 10: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[10] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, 0);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    // 8: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, HalfBaseSize - 1);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[8]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 8: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[8] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[5]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 7: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[5] + HalfBaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[7]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 7: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[7] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[5] + HalfBaseSize);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 6: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[5] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[6]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 6: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[6] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, HalfBaseSize);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);

    // 4: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::OriginalStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[4]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 4: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[4] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[1]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 3: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[1] + HalfBaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[3]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 3: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[3] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[1] + HalfBaseSize);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 2: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[1] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[2]);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    // 2: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[2] + BaseSize - 1);
    QCOMPARE(storageId, (int)Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(!result);
}

void PieceTableTest::testRemove_data()
{
    QTest::addColumn<int>("multiFillCount");
    QTest::addColumn<AddressRange>("removeRange");
    QTest::addColumn<Size>("expectedTableSize");
    QTest::addColumn<QVector<StorageDataTestData>>("testData");

    // removing a lot:
    const int pieceCount = 5;
    const int mid = (pieceCount + 1) / 2;
    const Address midPieceOffset = BaseSize * (mid - 1);
    const Size fullSize = pieceCount * BaseSize;
    // for this five equally sized pieces are inserted, reverse to offset in ChangeStore
    Address changeStarts[6];
    for (int i = 0; i < pieceCount; ++i) {
        changeStarts[pieceCount - i] = BaseSize * i;
    }

    QTest::newRow("removing-at-begin")
        << 0 << AddressRange(0, Start - 1)
        << BaseSize - Start
        << QVector<StorageDataTestData> {
            {0, true, Start, Piece::OriginalStorage},
            {BaseSize - Start - 1, true, BaseSize - 1, Piece::OriginalStorage},
            {BaseSize - Start, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-at-middle")
        << 0 << AddressRange(Start, End)
        << BaseSize - Width
        << QVector<StorageDataTestData> {
            {Start - 1, true, Start - 1, Piece::OriginalStorage},
            {Start, true, End + 1, Piece::OriginalStorage},
            {BaseSize - Width - 1, true, BaseSize - 1, Piece::OriginalStorage},
            {BaseSize - Width, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-at-end")
        << 0 << AddressRange(End + 1, BaseSize - 1)
        << End + 1
        << QVector<StorageDataTestData> {
            {End, true, End, Piece::OriginalStorage},
            {End + 1, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-all")
        << 0 << AddressRange::fromWidth(BaseSize)
        << 0
        << QVector<StorageDataTestData> {
            {0, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-inside-piece-in-middle")
        << pieceCount << AddressRange::fromWidth(midPieceOffset + Start, Width)
        << fullSize - Width
        << QVector<StorageDataTestData> {
            {midPieceOffset + Start - 1, true, changeStarts[mid] + Start - 1, Piece::ChangeStorage},
            {midPieceOffset + Start, true, changeStarts[mid] + End + 1, Piece::ChangeStorage},
            {fullSize - Width - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - Width, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-start-of-piece-in-middle")
        << pieceCount << AddressRange::fromWidth(midPieceOffset, Start)
        << fullSize - Start
        << QVector<StorageDataTestData> {
            {midPieceOffset - 1, true, changeStarts[mid - 1] + BaseSize - 1, Piece::ChangeStorage},
            {midPieceOffset, true, changeStarts[mid] + Start, Piece::ChangeStorage},
            {fullSize - Start - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - Start, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-end-of-piece-in-middle")
        << pieceCount << AddressRange::fromWidth(midPieceOffset + End + 1, BaseSize - (End + 1))
        << fullSize - (BaseSize - End - 1)
        << QVector<StorageDataTestData> {
            {midPieceOffset + End, true, changeStarts[mid] + End, Piece::ChangeStorage},
            {midPieceOffset + End + 1, true, changeStarts[mid + 1], Piece::ChangeStorage},
            {fullSize - (BaseSize - End - 1) - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - (BaseSize - End - 1), false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-whole-piece-in-middle")
        << pieceCount << AddressRange::fromWidth(midPieceOffset, BaseSize)
        << fullSize - BaseSize
        << QVector<StorageDataTestData> {
            {midPieceOffset - 1, true, changeStarts[mid - 1] + BaseSize - 1, Piece::ChangeStorage},
            {midPieceOffset, true, changeStarts[mid + 1], Piece::ChangeStorage},
            {fullSize - BaseSize - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-whole-piece-and-start-of-next-in-middke")
        << pieceCount << AddressRange::fromWidth(midPieceOffset, BaseSize + Start)
        << fullSize - BaseSize - Start
        << QVector<StorageDataTestData> {
            {midPieceOffset - 1, true, changeStarts[mid - 1] + BaseSize - 1, Piece::ChangeStorage},
            {midPieceOffset, true, changeStarts[mid + 1] + Start, Piece::ChangeStorage},
            {fullSize - BaseSize - Start - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize - Start, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-whole-piece-and-end-of-previous-in-middle")
        << pieceCount << AddressRange::fromWidth(midPieceOffset - (BaseSize - End - 1), BaseSize + BaseSize - (End + 1))
        << fullSize - BaseSize - (BaseSize - End - 1)
        << QVector<StorageDataTestData> {
            {midPieceOffset - (BaseSize - End - 1) - 1, true, changeStarts[mid - 1] + End, Piece::ChangeStorage},
            {midPieceOffset - (BaseSize - End - 1), true, changeStarts[mid + 1], Piece::ChangeStorage},
            {fullSize - BaseSize - (BaseSize - End - 1) - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize - (BaseSize - End - 1), false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-end-of-previous-whole-and-start-of-next-in-middle")
        << pieceCount << AddressRange::fromWidth(midPieceOffset - (BaseSize - End - 1), Start + BaseSize + BaseSize - (End + 1))
        << fullSize - BaseSize - (BaseSize - End - 1) - Start
        << QVector<StorageDataTestData> {
            {midPieceOffset - (BaseSize - End - 1) - 1, true, changeStarts[mid - 1] + BaseSize - (BaseSize - End - 1) - 1, Piece::ChangeStorage},
            {midPieceOffset - (BaseSize - End - 1), true, changeStarts[mid + 1] + Start, Piece::ChangeStorage},
            {fullSize - BaseSize - (BaseSize - End - 1) - Start - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize - (BaseSize - End - 1) - Start, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-start-of-piece-at-start")
        << pieceCount << AddressRange::fromWidth(Start)
        << fullSize - Start
        << QVector<StorageDataTestData> {
            {0, true, changeStarts[1] + Start, Piece::ChangeStorage},
            {fullSize - Start - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - Start, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-whole-piece-at-start")
        << pieceCount << AddressRange::fromWidth(BaseSize)
        << fullSize - BaseSize
        << QVector<StorageDataTestData> {
            {0, true, changeStarts[2], Piece::ChangeStorage},
            {fullSize - BaseSize - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-whole-piece-and-start-of-next-at-start")
        << pieceCount << AddressRange::fromWidth(BaseSize + Start)
        << fullSize - BaseSize - Start
        << QVector<StorageDataTestData> {
            {0, true, changeStarts[2] + Start, Piece::ChangeStorage},
            {fullSize - BaseSize - Start - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize - Start, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-end-of-piece-at-end")
        << pieceCount << AddressRange::fromWidth(fullSize - BaseSize + End + 1, BaseSize - (End + 1))
        << fullSize - (BaseSize - End - 1)
        << QVector<StorageDataTestData> {
            {fullSize - BaseSize + End, true, changeStarts[pieceCount] + End, Piece::ChangeStorage},
            {fullSize - (BaseSize - End - 1), false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-whole-piece-at-end")
        << pieceCount << AddressRange::fromWidth(fullSize - BaseSize, BaseSize)
        << fullSize - BaseSize
        << QVector<StorageDataTestData> {
            {fullSize - BaseSize - 1, true, changeStarts[pieceCount - 1] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-whole-piece-and-end-of-previous-at-end")
        << pieceCount << AddressRange::fromWidth(fullSize - BaseSize - (BaseSize - End - 1), BaseSize + BaseSize - (End + 1))
        << fullSize - BaseSize - (BaseSize - End - 1)
        << QVector<StorageDataTestData> {
            {fullSize - BaseSize - (BaseSize - End - 1) - 1, true, changeStarts[pieceCount - 1] + End, Piece::ChangeStorage},
            {fullSize - BaseSize - (BaseSize - End - 1), false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-all-pieces")
        << pieceCount << AddressRange::fromWidth(fullSize)
        << 0
        << QVector<StorageDataTestData> {
            {0, false, 0, Piece::OriginalStorage}};
}

void PieceTableTest::testRemove()
{
    QFETCH(const int, multiFillCount);
    QFETCH(const AddressRange, removeRange);
    QFETCH(const Size, expectedTableSize);
    QFETCH(const QVector<StorageDataTestData>, testData);

    PieceTable pieceTable;
    if (multiFillCount > 0) {
        fillWithSize(&pieceTable, multiFillCount);
    } else {
        pieceTable.init(BaseSize);
    }

    // tested action
    pieceTable.remove(removeRange);

    // check result
    QCOMPARE(pieceTable.size(), expectedTableSize);

    for (const auto& testDataEntry : testData) {
        int storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset);

        QCOMPARE(result, testDataEntry.expectedResult);
        if (testDataEntry.expectedResult) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset);
            QCOMPARE(storageId, testDataEntry.expectedStorageId);
        }
    }
}

void PieceTableTest::testSwap_data()
{
    QTest::addColumn<Address>("firstStart");
    QTest::addColumn<AddressRange>("secondRange");
    QTest::addColumn<QVector<StorageDataTestData>>("testData");

    QTest::newRow("moving-end-at-begin")
        << 0 << AddressRange::fromWidth(End + 1, BaseSize - (End + 1))
        << QVector<StorageDataTestData> {
            {0, true, End + 1, Piece::OriginalStorage},
            {BaseSize - End - 2, true, BaseSize - 1, Piece::OriginalStorage},
            {BaseSize - End - 1, true, 0, Piece::OriginalStorage},
            {BaseSize - 1, true, End, Piece::OriginalStorage},
            {BaseSize, false, -1, Piece::OriginalStorage}};
    QTest::newRow("moving-end-at-mid")
        << Start << AddressRange::fromWidth(End + 1, BaseSize - (End + 1))
        << QVector<StorageDataTestData> {
            {Start - 1, true, Start - 1, Piece::OriginalStorage},
            {Start, true, End + 1, Piece::OriginalStorage},
            {Start + BaseSize - End - 2, true, BaseSize - 1, Piece::OriginalStorage},
            {Start + BaseSize - End - 1, true, Start, Piece::OriginalStorage},
            {BaseSize - 1, true, End, Piece::OriginalStorage},
            {BaseSize, false, -1, Piece::OriginalStorage}};
    QTest::newRow("moving-mid-at-begin")
        << 0 << AddressRange::fromWidth(Start, Width)
        << QVector<StorageDataTestData> {
            {0, true, Start, Piece::OriginalStorage},
            {Width - 1, true, End, Piece::OriginalStorage},
            {Width, true, 0, Piece::OriginalStorage},
            {End, true, Start - 1, Piece::OriginalStorage},
            {End + 1, true, End + 1, Piece::OriginalStorage}};
    const Address mid = (End + Start) / 2;
    QTest::newRow("moving-mid-at-mid")
        << Start << AddressRange::fromWidth(mid, End - mid + 1)
        << QVector<StorageDataTestData> {
            {Start - 1, true, Start - 1, Piece::OriginalStorage},
            {Start, true, mid, Piece::OriginalStorage},
            {Start + End - mid, true, End, Piece::OriginalStorage},
            {Start + End - mid + 1, true, Start, Piece::OriginalStorage},
            {End, true, mid - 1, Piece::OriginalStorage},
            {End + 1, true, End + 1, Piece::OriginalStorage}};
}

void PieceTableTest::testSwap()
{
    QFETCH(const Address, firstStart);
    QFETCH(const AddressRange, secondRange);
    QFETCH(const QVector<StorageDataTestData>, testData);

    PieceTable pieceTable;
    pieceTable.init(BaseSize);

    // tested action
    pieceTable.swap(firstStart, secondRange);

    // check result
    QCOMPARE(pieceTable.size(), BaseSize);

    for (const auto& testDataEntry : testData) {
        int storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset);

        QCOMPARE(result, testDataEntry.expectedResult);
        if (testDataEntry.expectedResult) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset);
            QCOMPARE(storageId, testDataEntry.expectedStorageId);
        }
    }
}

}

QTEST_GUILESS_MAIN(KPieceTable::PieceTableTest)

#include "moc_piecetabletest.cpp"
