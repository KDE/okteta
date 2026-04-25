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

    pieceTable.init(BaseSize);
    Piece::StorageType storageId;
    Address storageOffset;

    QCOMPARE(pieceTable.size(), BaseSize);
    QCOMPARE(pieceTable.changesCount(), 0);
    QCOMPARE(pieceTable.appliedChangesCount(), 0);

    bool result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(result);
    QCOMPARE(storageOffset, 0);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Start);
    QVERIFY(result);
    QCOMPARE(storageOffset, Start);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, End);
    QVERIFY(result);
    QCOMPARE(storageOffset, End);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize);
    QVERIFY(!result);
}

static void fillWithSize(RevertablePieceTable* pieceTable, int count)
{
    int dummy;
    pieceTable->init(0);
    for (int i = 0; i < count; ++i) {
        pieceTable->insert(0, BaseSize, &dummy);
    }
}

void RevertablePieceTableTest::testInsert()
{
    RevertablePieceTable pieceTable;

    Piece::StorageType storageId;
    Address storageOffset;
    bool result;

    // inserting to empty
    pieceTable.init(0);
    pieceTable.insert(0, Width, &storageOffset);

    QCOMPARE(pieceTable.size(), Width);
    QCOMPARE(pieceTable.changesCount(), 1);
    QCOMPARE(pieceTable.appliedChangesCount(), 1);
    QCOMPARE(storageOffset, ChangeStart);

    result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeStart);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeEnd);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Width);
    QVERIFY(!result);

    // inserting one at the begin
    pieceTable.init(BaseSize);
    pieceTable.insert(0, Width, &storageOffset);

    QCOMPARE(pieceTable.size(), BaseSize + Width);
    QCOMPARE(pieceTable.changesCount(), 1);
    QCOMPARE(pieceTable.appliedChangesCount(), 1);
    QCOMPARE(storageOffset, ChangeStart);

    result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeStart);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeEnd);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Width);
    QVERIFY(result);
    QCOMPARE(storageOffset, 0);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width);
    QVERIFY(!result);

    // inserting one in the middle
    pieceTable.init(BaseSize);
    pieceTable.insert(Start, Width, &storageOffset);

    QCOMPARE(pieceTable.size(), BaseSize + Width);
    QCOMPARE(pieceTable.changesCount(), 1);
    QCOMPARE(pieceTable.appliedChangesCount(), 1);
    QCOMPARE(storageOffset, ChangeStart);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Start - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, Start - 1);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Start);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeStart);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, End);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeEnd);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, End + 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, Start);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width);
    QVERIFY(!result);

    // inserting one at the end
    pieceTable.init(BaseSize);
    pieceTable.insert(BaseSize, Width, &storageOffset);

    QCOMPARE(pieceTable.size(), BaseSize + Width);
    QCOMPARE(pieceTable.changesCount(), 1);
    QCOMPARE(pieceTable.appliedChangesCount(), 1);
    QCOMPARE(storageOffset, ChangeStart);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeStart);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize + Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, ChangeEnd);
    QCOMPARE(storageId, Piece::ChangeStorage);

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

    // all borders
    // 12: begin
    Address byteArrayOffset = 0;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[12]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 12: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[12] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[9]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 11: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[9] + HalfBaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[11]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 11: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[11] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[9] + HalfBaseSize);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 10: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[9] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[10]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 10: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[10] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, 0);
    QCOMPARE(storageId, Piece::OriginalStorage);

    // 8: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, HalfBaseSize - 1);
    QCOMPARE(storageId, Piece::OriginalStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[8]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 8: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[8] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[5]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 7: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[5] + HalfBaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[7]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 7: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[7] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[5] + HalfBaseSize);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 6: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[5] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[6]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 6: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[6] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, HalfBaseSize);
    QCOMPARE(storageId, Piece::OriginalStorage);

    // 4: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, Piece::OriginalStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[4]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 4: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[4] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[1]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 3: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[1] + HalfBaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[3]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 3: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[3] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[1] + HalfBaseSize);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 2: begin
    byteArrayOffset += HalfBaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[1] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[2]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    // 2: end
    byteArrayOffset += BaseSize;
    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[2] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, byteArrayOffset);
    QVERIFY(!result);
}

void RevertablePieceTableTest::testRemove()
{
    RevertablePieceTable pieceTable;

    Address changeStarts[6];
    Piece::StorageType storageId;
    Address storageOffset;
    bool result;

    // removing at begin
    pieceTable.init(BaseSize);
    pieceTable.remove(AddressRange(0, Start - 1));

    QCOMPARE(pieceTable.size(), BaseSize - Start);
    QCOMPARE(pieceTable.changesCount(), 1);
    QCOMPARE(pieceTable.appliedChangesCount(), 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(result);
    QCOMPARE(storageOffset, Start);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize - Start - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize - Start);
    QVERIFY(!result);

    // removing at middle
    pieceTable.init(BaseSize);
    pieceTable.remove(AddressRange(Start, End));

    QCOMPARE(pieceTable.size(), BaseSize - Width);
    QCOMPARE(pieceTable.changesCount(), 1);
    QCOMPARE(pieceTable.appliedChangesCount(), 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Start - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, Start - 1);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, Start);
    QVERIFY(result);
    QCOMPARE(storageOffset, End + 1);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize - Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, BaseSize - 1);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, BaseSize - Width);
    QVERIFY(!result);

    // removing at end
    pieceTable.init(BaseSize);
    pieceTable.remove(AddressRange(End + 1, BaseSize - 1));

    QCOMPARE(pieceTable.size(), End + 1);
    QCOMPARE(pieceTable.changesCount(), 1);
    QCOMPARE(pieceTable.appliedChangesCount(), 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, End);
    QVERIFY(result);
    QCOMPARE(storageOffset, End);
    QCOMPARE(storageId, Piece::OriginalStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, End + 1);
    QVERIFY(!result);

    // removing all
    pieceTable.init(BaseSize);
    pieceTable.remove(AddressRange::fromWidth(BaseSize));

    QCOMPARE(pieceTable.size(), 0);
    QCOMPARE(pieceTable.changesCount(), 1);
    QCOMPARE(pieceTable.appliedChangesCount(), 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(!result);

    // removing a lot:
    const int pieceCount = 5;
    const int mid = (pieceCount + 1) / 2;
    const Address midPieceOffset = BaseSize * (mid - 1);
    const Size fullSize = pieceCount * BaseSize;
    // for this five equally sized pieces are inserted, reverse to offset in ChangeStore
    for (int i = 0; i < pieceCount; ++i) {
        changeStarts[pieceCount - i] = BaseSize * i;
    }

    // removing inside a piece in the middle
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(midPieceOffset + Start, Width));

    QCOMPARE(pieceTable.size(), fullSize - Width);
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset + Start - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid] + Start - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset + Start);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid] + End + 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - Width - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - Width);
    QVERIFY(!result);

    // removing start of a piece in the middle
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(midPieceOffset, Start));

    QCOMPARE(pieceTable.size(), fullSize - Start);
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid - 1] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid] + Start);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - Start - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - Start);
    QVERIFY(!result);

    // removing end of a piece in the middle
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(midPieceOffset + End + 1, BaseSize - (End + 1)));

    QCOMPARE(pieceTable.size(), fullSize - (BaseSize - End - 1));
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset + End);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid] + End);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset + End + 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid + 1]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - (BaseSize - End - 1) - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - (BaseSize - End - 1));
    QVERIFY(!result);

    // removing whole piece in the middle
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(midPieceOffset, BaseSize));

    QCOMPARE(pieceTable.size(), fullSize - BaseSize);
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid - 1] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid + 1]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize);
    QVERIFY(!result);

    // removing whole piece and start of next in the middke
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(midPieceOffset, BaseSize + Start));

    QCOMPARE(pieceTable.size(), fullSize - BaseSize - Start);
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid - 1] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid + 1] + Start);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - Start - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - Start);
    QVERIFY(!result);

    // removing whole piece and end of previous in the middle
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(midPieceOffset - (BaseSize - End - 1), BaseSize + BaseSize - (End + 1)));

    QCOMPARE(pieceTable.size(), fullSize - BaseSize - (BaseSize - End - 1));
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset - (BaseSize - End - 1) - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid - 1] + End);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset - (BaseSize - End - 1));
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid + 1]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - (BaseSize - End - 1) - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - (BaseSize - End - 1));
    QVERIFY(!result);

    // removing end of previous, whole and start of next in the middle
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(midPieceOffset - (BaseSize - End - 1), Start + BaseSize + BaseSize - (End + 1)));

    QCOMPARE(pieceTable.size(), fullSize - BaseSize - (BaseSize - End - 1) - Start);
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset - (BaseSize - End - 1) - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid - 1] + BaseSize - (BaseSize - End - 1) - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, midPieceOffset - (BaseSize - End - 1));
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[mid + 1] + Start);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - (BaseSize - End - 1) - Start - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - (BaseSize - End - 1) - Start);
    QVERIFY(!result);

    // removing start of piece at start
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(Start));

    QCOMPARE(pieceTable.size(), fullSize - Start);
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[1] + Start);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - Start - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - Start);
    QVERIFY(!result);

    // removing whole piece at start
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(BaseSize));

    QCOMPARE(pieceTable.size(), fullSize - BaseSize);
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[2]);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize);
    QVERIFY(!result);

    // removing whole piece and start of next at start
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(BaseSize + Start));

    QCOMPARE(pieceTable.size(), fullSize - BaseSize - Start);
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[2] + Start);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - Start - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - Start);
    QVERIFY(!result);

    // removing end of piece at end
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(fullSize - BaseSize + End + 1, BaseSize - (End + 1)));

    QCOMPARE(pieceTable.size(), fullSize - (BaseSize - End - 1));
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize + End);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount] + End);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - (BaseSize - End - 1));
    QVERIFY(!result);

    // removing whole piece at end
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(fullSize - BaseSize, BaseSize));

    QCOMPARE(pieceTable.size(), fullSize - BaseSize);
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount - 1] + BaseSize - 1);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize);
    QVERIFY(!result);

    // removing whole piece and end of previous at end
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(fullSize - BaseSize - (BaseSize - End - 1), BaseSize + BaseSize - (End + 1)));

    QCOMPARE(pieceTable.size(), fullSize - BaseSize - (BaseSize - End - 1));
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - (BaseSize - End - 1) - 1);
    QVERIFY(result);
    QCOMPARE(storageOffset, changeStarts[pieceCount - 1] + End);
    QCOMPARE(storageId, Piece::ChangeStorage);

    result = pieceTable.getStorageData(&storageId, &storageOffset, fullSize - BaseSize - (BaseSize - End - 1));
    QVERIFY(!result);

    // removing all
    fillWithSize(&pieceTable, pieceCount);
    pieceTable.remove(AddressRange::fromWidth(fullSize));

    QCOMPARE(pieceTable.size(), 0);
    QCOMPARE(pieceTable.changesCount(), pieceCount + 1);
    QCOMPARE(pieceTable.appliedChangesCount(), pieceCount + 1);

    result = pieceTable.getStorageData(&storageId, &storageOffset, 0);
    QVERIFY(!result);
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
