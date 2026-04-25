/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "revertablepiecetabletest.hpp"

// test object
#include <piecetable/revertablepiecetable.hpp>
// Qt
#include <QVector>
#include <QTest>

namespace KPieceTable {
struct StorageDataTestData
{
    static StorageDataTestData invalid(Address dataOffset)
    { return {dataOffset, false, 0, Piece::OriginalStorage}; }
    static StorageDataTestData valid(Address dataOffset, Address storageOffset, Piece::StorageType storageId)
    { return {dataOffset, true, storageOffset, storageId}; }

    StorageDataTestData() = default;

    Address dataOffset() const { return m_dataOffset; }
    bool expectedResult() const { return m_result; }
    Address expectedStorageOffset() const { return m_storageOffset; }
    Piece::StorageType expectedStorageId() const { return m_storageId; }

private:
    StorageDataTestData(Address dataOffset, bool result, Address storageOffset, Piece::StorageType storageId)
        : m_dataOffset(dataOffset), m_result(result), m_storageOffset(storageOffset), m_storageId(storageId)
    {}

private:
    const Address m_dataOffset = 0;
    const bool m_result = false;
    const Address m_storageOffset = 0;
    const Piece::StorageType m_storageId = Piece::OriginalStorage;
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

static constexpr Address ChangeStart = 0;
static constexpr Address ChangeEnd = ChangeStart + Width - 1;

void RevertablePieceTableTest::testSimpleConstructor()
{
    RevertablePieceTable pieceTable;
    QCOMPARE(pieceTable.size(), 0);
    QCOMPARE(pieceTable.changesCount(), 0);
    QCOMPARE(pieceTable.appliedChangesCount(), 0);
}

void RevertablePieceTableTest::testInit()
{
    RevertablePieceTable pieceTable;

    // tested action
    pieceTable.init(BaseSize);

    // check result
    QCOMPARE(pieceTable.size(), BaseSize);
    QCOMPARE(pieceTable.changesCount(), 0);
    QCOMPARE(pieceTable.appliedChangesCount(), 0);

    const QVector<StorageDataTestData> testData {
        StorageDataTestData::valid(0, 0, Piece::OriginalStorage),
        StorageDataTestData::valid(Start, Start, Piece::OriginalStorage),
        StorageDataTestData::valid(End, End, Piece::OriginalStorage),
        StorageDataTestData::valid(BaseSize - 1, BaseSize - 1, Piece::OriginalStorage),
        StorageDataTestData::invalid(BaseSize),
    };
    for (const auto& testDataEntry : testData) {
        Piece::StorageType storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset());

        QCOMPARE(result, testDataEntry.expectedResult());
        if (result) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset());
            QCOMPARE(storageId, testDataEntry.expectedStorageId());
        }
    }
}

static void fillWithSize(RevertablePieceTable* pieceTable, int count)
{
    int dummy;
    pieceTable->init(0);
    for (int i = 0; i < count; ++i) {
        pieceTable->insert(0, BaseSize, &dummy);
    }
}

void RevertablePieceTableTest::testInsert_data()
{
    QTest::addColumn<Size>("initTableSize");
    QTest::addColumn<Address>("insertDataOffset");
    QTest::addColumn<Size>("insertLength");
    QTest::addColumn<Address>("expectedInsertStorageOffset");
    QTest::addColumn<Size>("expectedTableSize");
    QTest::addColumn<int>("expectedChangesCount");
    QTest::addColumn<int>("expectedAppliedChangesCount");
    QTest::addColumn<QVector<StorageDataTestData>>("testData");

    QTest::newRow("inserting-to-empty")
        << 0
        << 0 << Width
        << ChangeStart
        << Width << 1 << 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, ChangeStart, Piece::ChangeStorage),
            StorageDataTestData::valid(Width - 1, ChangeEnd, Piece::ChangeStorage),
            StorageDataTestData::invalid(Width)};

    QTest::newRow("inserting-one-at-begin")
        << BaseSize
        << 0 << Width
        << ChangeStart
        << BaseSize + Width << 1 << 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, ChangeStart, Piece::ChangeStorage),
            StorageDataTestData::valid(Width - 1, ChangeEnd, Piece::ChangeStorage),
            StorageDataTestData::valid(Width, 0, Piece::OriginalStorage),
            StorageDataTestData::valid(BaseSize + Width - 1, BaseSize - 1, Piece::OriginalStorage),
            StorageDataTestData::invalid(BaseSize + Width)};

    QTest::newRow("inserting-one-in-middle")
        << BaseSize
        << Start << Width
        << ChangeStart
        << BaseSize + Width << 1 << 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(Start - 1, Start - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Start, ChangeStart, Piece::ChangeStorage),
            StorageDataTestData::valid(End, ChangeEnd, Piece::ChangeStorage),
            StorageDataTestData::valid(End + 1, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(BaseSize + Width - 1, BaseSize - 1, Piece::OriginalStorage),
            StorageDataTestData::invalid(BaseSize + Width)};

    QTest::newRow("inserting-one-at-end")
        << BaseSize
        << BaseSize << Width
        << ChangeStart
        << BaseSize + Width << 1 << 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(BaseSize - 1, BaseSize - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(BaseSize, ChangeStart, Piece::ChangeStorage),
            StorageDataTestData::valid(BaseSize + Width - 1, ChangeEnd, Piece::ChangeStorage),
            StorageDataTestData::invalid(BaseSize + Width)};
}

void RevertablePieceTableTest::testInsert()
{
    QFETCH(const Size, initTableSize);
    QFETCH(const Address, insertDataOffset);
    QFETCH(const Size, insertLength);
    QFETCH(const Address, expectedInsertStorageOffset);
    QFETCH(const Size, expectedTableSize);
    QFETCH(const int, expectedChangesCount);
    QFETCH(const int, expectedAppliedChangesCount);
    QFETCH(const QVector<StorageDataTestData>, testData);

    RevertablePieceTable pieceTable;
    pieceTable.init(initTableSize);

    // tested action
    Address insertStorageOffset;
    pieceTable.insert(insertDataOffset, insertLength, &insertStorageOffset);

    // check result
    QCOMPARE(insertStorageOffset, expectedInsertStorageOffset);
    QCOMPARE(pieceTable.size(), expectedTableSize);
    QCOMPARE(pieceTable.changesCount(), expectedChangesCount);
    QCOMPARE(pieceTable.appliedChangesCount(), expectedAppliedChangesCount);

    for (const auto& testDataEntry : testData) {
        Piece::StorageType storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset());

        QCOMPARE(result, testDataEntry.expectedResult());
        if (testDataEntry.expectedResult()) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset());
            QCOMPARE(storageId, testDataEntry.expectedStorageId());
        }
    }
}

void RevertablePieceTableTest::testInsertMulti()
{
    RevertablePieceTable pieceTable;

    Address storageOffset;

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
    const int Mid = BaseSize / 2;
    Address changeStarts[13];
    pieceTable.insert(BaseSize, BaseSize, &storageOffset);   // 1
    changeStarts[1] = storageOffset;
    pieceTable.insert(BaseSize + BaseSize, BaseSize, &storageOffset); // 2
    changeStarts[2] = storageOffset;
    pieceTable.insert(BaseSize + Mid, BaseSize, &storageOffset); // 3
    changeStarts[3] = storageOffset;
    pieceTable.insert(BaseSize, BaseSize, &storageOffset);   // 4
    changeStarts[4] = storageOffset;

    pieceTable.insert(Mid, BaseSize, &storageOffset);   // 5
    changeStarts[5] = storageOffset;
    pieceTable.insert(Mid + BaseSize, BaseSize, &storageOffset); // 6
    changeStarts[6] = storageOffset;
    pieceTable.insert(Mid + Mid, BaseSize, &storageOffset); // 7
    changeStarts[7] = storageOffset;
    pieceTable.insert(Mid, BaseSize, &storageOffset);   // 8
    changeStarts[8] = storageOffset;

    pieceTable.insert(0, BaseSize, &storageOffset);   // 9
    changeStarts[9] = storageOffset;
    pieceTable.insert(BaseSize, BaseSize, &storageOffset);   // 10
    changeStarts[10] = storageOffset;
    pieceTable.insert(Mid, BaseSize, &storageOffset);   // 11
    changeStarts[11] = storageOffset;
    pieceTable.insert(0, BaseSize, &storageOffset);   // 12
    changeStarts[12] = storageOffset;

    // test
    QCOMPARE(pieceTable.size(), BaseSize + 12 * BaseSize);
    QCOMPARE(pieceTable.changesCount(), 9);   // 9+10, 5+6, 1+2 are merged
    QCOMPARE(pieceTable.appliedChangesCount(), 9);
    QCOMPARE(storageOffset, 11 * BaseSize);

    QVector<StorageDataTestData> testData;

    // all borders
    // 12: begin
    Address byteArrayOffset = 0;
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[12], Piece::ChangeStorage));

    // 12: end
    byteArrayOffset += BaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[12] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[9], Piece::ChangeStorage));

    // 11: begin
    byteArrayOffset += HalfBaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[9] + HalfBaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[11], Piece::ChangeStorage));

    // 11: end
    byteArrayOffset += BaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[11] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[9] + HalfBaseSize, Piece::ChangeStorage));

    // 10: begin
    byteArrayOffset += HalfBaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[9] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[10], Piece::ChangeStorage));

    // 10: end
    byteArrayOffset += BaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[10] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, 0, Piece::OriginalStorage));

    // 8: begin
    byteArrayOffset += HalfBaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, HalfBaseSize - 1, Piece::OriginalStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[8], Piece::ChangeStorage));

    // 8: end
    byteArrayOffset += BaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[8] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[5], Piece::ChangeStorage));

    // 7: begin
    byteArrayOffset += HalfBaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[5] + HalfBaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[7], Piece::ChangeStorage));

    // 7: end
    byteArrayOffset += BaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[7] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[5] + HalfBaseSize, Piece::ChangeStorage));

    // 6: begin
    byteArrayOffset += HalfBaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[5] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[6], Piece::ChangeStorage));

    // 6: end
    byteArrayOffset += BaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[6] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, HalfBaseSize, Piece::OriginalStorage));

    // 4: begin
    byteArrayOffset += HalfBaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, BaseSize - 1, Piece::OriginalStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[4], Piece::ChangeStorage));

    // 4: end
    byteArrayOffset += BaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[4] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[1], Piece::ChangeStorage));

    // 3: begin
    byteArrayOffset += HalfBaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[1] + HalfBaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[3], Piece::ChangeStorage));

    // 3: end
    byteArrayOffset += BaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[3] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[1] + HalfBaseSize, Piece::ChangeStorage));

    // 2: begin
    byteArrayOffset += HalfBaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[1] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::valid(byteArrayOffset, changeStarts[2], Piece::ChangeStorage));

    // 2: end
    byteArrayOffset += BaseSize;
    testData.append(StorageDataTestData::valid(byteArrayOffset - 1, changeStarts[2] + BaseSize - 1, Piece::ChangeStorage));
    testData.append(StorageDataTestData::invalid(byteArrayOffset));

    for (const auto& testDataEntry : testData) {
        Piece::StorageType storageId;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset());

        QCOMPARE(result, testDataEntry.expectedResult());
        if (testDataEntry.expectedResult()) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset());
            QCOMPARE(storageId, testDataEntry.expectedStorageId());
        }
    }
}

void RevertablePieceTableTest::testRemove_data()
{
    QTest::addColumn<int>("multiFillCount");
    QTest::addColumn<AddressRange>("removeRange");
    QTest::addColumn<Size>("expectedTableSize");
    QTest::addColumn<int>("expectedChangesCount");
    QTest::addColumn<int>("expectedAppliedChangesCount");
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

    AddressRange removeRange = AddressRange(0, Start - 1);
    QTest::newRow("removing-at-begin")
        << 0 << removeRange
        << BaseSize - Start << 1 << 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(BaseSize - Start - 1, BaseSize - 1, Piece::OriginalStorage),
            StorageDataTestData::invalid(BaseSize - Start)};

    removeRange = AddressRange(Start, End);
    QTest::newRow("removing-at-middle")
        << 0 << removeRange
        << BaseSize - Width << 1 << 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(Start - 1, Start - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Start, End + 1, Piece::OriginalStorage),
            StorageDataTestData::valid(BaseSize - Width - 1, BaseSize - 1, Piece::OriginalStorage),
            StorageDataTestData::invalid(BaseSize - Width)};

    removeRange = AddressRange(End + 1, BaseSize - 1);
    QTest::newRow("removing-at-end")
        << 0 << removeRange
        << End + 1 << 1 << 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(End, End, Piece::OriginalStorage),
            StorageDataTestData::invalid(End + 1)};

    removeRange = AddressRange::fromWidth(BaseSize);
    QTest::newRow("removing-all")
        << 0 << removeRange
        << 0 << 1 << 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::invalid(0)};

    removeRange = AddressRange::fromWidth(midPieceOffset + Start, Width);
    QTest::newRow("removing-inside-a-piece-in-middle")
        << pieceCount << removeRange
        << fullSize - Width << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(midPieceOffset + Start - 1, changeStarts[mid] + Start - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(midPieceOffset + Start, changeStarts[mid] + End + 1, Piece::ChangeStorage),
            StorageDataTestData::valid(fullSize - Width - 1, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - Width)};

    removeRange = AddressRange::fromWidth(midPieceOffset, Start);
    QTest::newRow("removing-start-of-a-piece-in-middle")
        << pieceCount << removeRange
        << fullSize - Start << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(midPieceOffset - 1, changeStarts[mid - 1] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(midPieceOffset, changeStarts[mid] + Start, Piece::ChangeStorage),
            StorageDataTestData::valid(fullSize - Start - 1, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - Start)};

    removeRange = AddressRange::fromWidth(midPieceOffset + End + 1, BaseSize - (End + 1));
    QTest::newRow("removing-end-of-a-piece-in-middle")
        << pieceCount << removeRange
        << fullSize - (BaseSize - End - 1) << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(midPieceOffset + End, changeStarts[mid] + End, Piece::ChangeStorage),
            StorageDataTestData::valid(midPieceOffset + End + 1, changeStarts[mid + 1], Piece::ChangeStorage),
            StorageDataTestData::valid(fullSize - (BaseSize - End - 1) - 1, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - (BaseSize - End - 1))};

    removeRange = AddressRange::fromWidth(midPieceOffset, BaseSize);
    QTest::newRow("removing-whole-piece-in-middle")
        << pieceCount << removeRange
        << fullSize - BaseSize << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(midPieceOffset - 1, changeStarts[mid - 1] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(midPieceOffset, changeStarts[mid + 1], Piece::ChangeStorage),
            StorageDataTestData::valid(fullSize - BaseSize - 1, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - BaseSize)};

    removeRange = AddressRange::fromWidth(midPieceOffset, BaseSize + Start);
    QTest::newRow("removing-whole-piece-and-start-of-next-in-middle")
        << pieceCount << removeRange
        << fullSize - BaseSize - Start << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(midPieceOffset - 1, changeStarts[mid - 1] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(midPieceOffset, changeStarts[mid + 1] + Start, Piece::ChangeStorage),
            StorageDataTestData::valid(fullSize - BaseSize - Start - 1, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - BaseSize - Start)};

    removeRange = AddressRange::fromWidth(midPieceOffset - (BaseSize - End - 1), BaseSize + BaseSize - (End + 1));
    QTest::newRow("removing-whole-piece-and-end-of-previous-in-middle")
        << pieceCount << removeRange
        << fullSize - BaseSize - (BaseSize - End - 1) << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(midPieceOffset - (BaseSize - End - 1) - 1, changeStarts[mid - 1] + End, Piece::ChangeStorage),
            StorageDataTestData::valid(midPieceOffset - (BaseSize - End - 1), changeStarts[mid + 1], Piece::ChangeStorage),
            StorageDataTestData::valid(fullSize - BaseSize - (BaseSize - End - 1) - 1, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - BaseSize - (BaseSize - End - 1))};

    removeRange = AddressRange::fromWidth(midPieceOffset - (BaseSize - End - 1), Start + BaseSize + BaseSize - (End + 1));
    QTest::newRow("removing-end-of-previous-whole-and-start-of-next-in-middle")
        << pieceCount << removeRange
        << fullSize - BaseSize - (BaseSize - End - 1) - Start << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(midPieceOffset - (BaseSize - End - 1) - 1, changeStarts[mid - 1] + BaseSize - (BaseSize - End - 1) - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(midPieceOffset - (BaseSize - End - 1), changeStarts[mid + 1] + Start, Piece::ChangeStorage),
            StorageDataTestData::valid(fullSize - BaseSize - (BaseSize - End - 1) - Start - 1, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - BaseSize - (BaseSize - End - 1) - Start)};

    removeRange = AddressRange::fromWidth(Start);
    QTest::newRow("removing-start-of-piece-at-start")
        << pieceCount << removeRange
        << fullSize - Start << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, changeStarts[1] + Start, Piece::ChangeStorage),
            StorageDataTestData::valid(fullSize - Start - 1, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - Start)};

    removeRange = AddressRange::fromWidth(BaseSize);
    QTest::newRow("removing-whole-piece-at-start")
        << pieceCount << removeRange
        << fullSize - BaseSize << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, changeStarts[2], Piece::ChangeStorage),
            StorageDataTestData::valid(fullSize - BaseSize - 1, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - BaseSize)};

    removeRange = AddressRange::fromWidth(BaseSize + Start);
    QTest::newRow("removing-whole-piece-and-start-of-next-at-start")
        << pieceCount << removeRange
        << fullSize - BaseSize - Start << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, changeStarts[2] + Start, Piece::ChangeStorage),
            StorageDataTestData::valid(fullSize - BaseSize - Start - 1, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - BaseSize - Start)};

    removeRange = AddressRange::fromWidth(fullSize - BaseSize + End + 1, BaseSize - (End + 1));
    QTest::newRow("removing-end-of-piece-at-end")
        << pieceCount << removeRange
        << fullSize - (BaseSize - End - 1) << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(fullSize - BaseSize + End, changeStarts[pieceCount] + End, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - (BaseSize - End - 1))};

    removeRange = AddressRange::fromWidth(fullSize - BaseSize, BaseSize);
    QTest::newRow("removing-whole-piece-at-end")
        << pieceCount << removeRange
        << fullSize - BaseSize << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(fullSize - BaseSize - 1, changeStarts[pieceCount - 1] + BaseSize - 1, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - BaseSize)};

    removeRange = AddressRange::fromWidth(fullSize - BaseSize - (BaseSize - End - 1), BaseSize + BaseSize - (End + 1));
    QTest::newRow("removing-whole-piece-and-end-of-previous-at-end")
        << pieceCount << removeRange
        << fullSize - BaseSize - (BaseSize - End - 1) << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(fullSize - BaseSize - (BaseSize - End - 1) - 1, changeStarts[pieceCount - 1] + End, Piece::ChangeStorage),
            StorageDataTestData::invalid(fullSize - BaseSize - (BaseSize - End - 1))};

    removeRange = AddressRange::fromWidth(fullSize);
    QTest::newRow("removing-all")
        << pieceCount << removeRange
        << 0 << pieceCount + 1 << pieceCount + 1
        << QVector<StorageDataTestData> {
            StorageDataTestData::invalid(0)};
}

void RevertablePieceTableTest::testRemove()
{
    QFETCH(const int, multiFillCount);
    QFETCH(const AddressRange, removeRange);
    QFETCH(const Size, expectedTableSize);
    QFETCH(const int, expectedChangesCount);
    QFETCH(const int, expectedAppliedChangesCount);
    QFETCH(const QVector<StorageDataTestData>, testData);

    RevertablePieceTable pieceTable;
    if (multiFillCount > 0) {
        fillWithSize(&pieceTable, multiFillCount);
    } else {
        pieceTable.init(BaseSize);
    }

    // tested action
    pieceTable.remove(removeRange);

    // check result
    QCOMPARE(pieceTable.size(), expectedTableSize);
    QCOMPARE(pieceTable.changesCount(), expectedChangesCount);
    QCOMPARE(pieceTable.appliedChangesCount(), expectedAppliedChangesCount);

    for (const auto& testDataEntry : testData) {
        Piece::StorageType storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset());

        QCOMPARE(result, testDataEntry.expectedResult());
        if (testDataEntry.expectedResult()) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset());
            QCOMPARE(storageId, testDataEntry.expectedStorageId());
        }
    }
}

void RevertablePieceTableTest::testSwap_data()
{
    QTest::addColumn<Address>("firstStart");
    QTest::addColumn<AddressRange>("secondRange");
    QTest::addColumn<QVector<StorageDataTestData>>("testData");

    const Address mid = (End + Start) / 2;

    AddressRange moveRange = AddressRange::fromWidth(End + 1, BaseSize - (End + 1));
    QTest::newRow("moving-end-at-begin")
        << 0 << moveRange
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, End + 1, Piece::OriginalStorage),
            StorageDataTestData::valid(BaseSize - End - 2, BaseSize - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(BaseSize - End - 1, 0, Piece::OriginalStorage),
            StorageDataTestData::valid(BaseSize - 1, End, Piece::OriginalStorage),
            StorageDataTestData::invalid(BaseSize)};

    moveRange = AddressRange::fromWidth(End + 1, BaseSize - (End + 1));
    QTest::newRow("moving-end-at-mid")
        << Start << moveRange
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(Start - 1, Start - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Start, End + 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Start + BaseSize - End - 2, BaseSize - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Start + BaseSize - End - 1, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(BaseSize - 1, End, Piece::OriginalStorage),
            StorageDataTestData::invalid(BaseSize)};

    moveRange = AddressRange::fromWidth(Start, Width);
    QTest::newRow("moving-mid-at-begin")
        << 0 << moveRange
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(Width - 1, End, Piece::OriginalStorage),
            StorageDataTestData::valid(Width, 0, Piece::OriginalStorage),
            StorageDataTestData::valid(End, Start - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(End + 1, End + 1, Piece::OriginalStorage)};

    moveRange = AddressRange::fromWidth(mid, End - mid + 1);
    QTest::newRow("moving-mid-at-mid")
        << Start << moveRange
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(Start - 1, Start - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Start, mid, Piece::OriginalStorage),
            StorageDataTestData::valid(Start + End - mid, End, Piece::OriginalStorage),
            StorageDataTestData::valid(Start + End - mid + 1, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(End, mid - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(End + 1, End + 1, Piece::OriginalStorage)};
}

void RevertablePieceTableTest::testSwap()
{
    QFETCH(const Address, firstStart);
    QFETCH(const AddressRange, secondRange);
    QFETCH(const QVector<StorageDataTestData>, testData);

    RevertablePieceTable pieceTable;
    pieceTable.init(BaseSize);

    // tested action
    pieceTable.swap(firstStart, secondRange);

    // check result
    QCOMPARE(pieceTable.size(), BaseSize);
    QCOMPARE(pieceTable.changesCount(), 1);
    QCOMPARE(pieceTable.appliedChangesCount(), 1);

    for (const auto& testDataEntry : testData) {
        Piece::StorageType storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset());

        QCOMPARE(result, testDataEntry.expectedResult());
        if (testDataEntry.expectedResult()) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset());
            QCOMPARE(storageId, testDataEntry.expectedStorageId());
        }
    }
}

}

QTEST_GUILESS_MAIN(KPieceTable::RevertablePieceTableTest)

#include "moc_revertablepiecetabletest.cpp"
