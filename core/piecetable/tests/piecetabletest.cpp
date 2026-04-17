/*
    SPDX-FileCopyrightText: 2008, 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetabletest.hpp"

// test object
#include <piecetable/piecetable.hpp>
// Qt
#include <QVector>
#include <QTest>
// Std
#include <cstdio>

namespace KPieceTable {
struct StorageDataTestData
{
    Address dataOffset;

    bool expectedResult;
    Address expectedStorageOffset;
    Piece::StorageType expectedStorageId;
};
}

Q_DECLARE_METATYPE(QVector<KPieceTable::StorageDataTestData>)
Q_DECLARE_METATYPE(QVector<KPieceTable::Piece>)
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

void sprintf(char* buffer, Piece piece)
{
    const char* const storageId = (piece.storageId() == Piece::OriginalStorage) ? "Original" : "Change";

    ::sprintf(buffer, "%d-%d, %s", piece.start(), piece.end(), storageId);
}

QDebug operator<<(QDebug dbg, Piece piece)
{
    char text[256]; // TODO: estimate max. needed size
    sprintf(text, piece);
    dbg.nospace() << "Piece(" << text << ')';
    return dbg.space();
}

char* toString(Piece piece)
{
    char text[256]; // TODO: estimate max. needed size
    sprintf(text, piece);
    // TODO: Qt5's QTest::toString() needs casting to char*, char[] type not known in overload resolution
    return QTest::toString(static_cast<const char*>(text));
}

void compare(const PieceTable& pieceTable, const QVector<Piece>& expectedPieces)
{
    // TODO: find some useful output with QCOMPARE
    if (pieceTable.piecesSize() != expectedPieces.size()) {
        for (const auto& piece : expectedPieces) {
            qDebug() << "Expected:" << piece;
        }
        for (const auto& piece : pieceTable) {
            qDebug() << "Actual:" << piece;
        }
    }

    QCOMPARE(pieceTable.piecesSize(), expectedPieces.size());

    int i = 0;
    for (const auto& piece : pieceTable) {
        const auto& expectedPiece = expectedPieces[i++];
        QCOMPARE(piece, expectedPiece);
    }
}

void PieceTableTest::testSimpleConstructor()
{
    PieceTable pieceTable;
    QCOMPARE(pieceTable.size(), 0);
    QCOMPARE(pieceTable.piecesSize(), 0);
}

void PieceTableTest::testInit()
{
    PieceTable pieceTable;

    // tested action
    pieceTable.init(BaseSize);

    // check result
    const QVector<StorageDataTestData> testData {
        {0, true, 0, Piece::OriginalStorage},
        {Start, true, Start, Piece::OriginalStorage},
        {End, true, End, Piece::OriginalStorage},
        {BaseSize - 1, true, BaseSize - 1, Piece::OriginalStorage},
        {BaseSize, false, -1, Piece::OriginalStorage},
    };
    for (const auto& testDataEntry : testData) {
        Piece::StorageType storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset);

        QCOMPARE(result, testDataEntry.expectedResult);
        if (testDataEntry.expectedResult) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset);
            QCOMPARE(storageId, testDataEntry.expectedStorageId);
        }
    }

    const QVector<Piece> expectedPieces {
            {0, BaseSize, Piece::OriginalStorage}
    };
    compare(pieceTable, expectedPieces);
}

static void fillWithSize(PieceTable* pieceTable, int count)
{
    pieceTable->init(0);
    for (int i = 0; i < count; ++i) {
        pieceTable->insert(0, BaseSize, BaseSize * i);
    }

}

void PieceTableTest::testInsert_data()
{
    QTest::addColumn<Size>("initTableSize");
    QTest::addColumn<Address>("insertDataOffset");
    QTest::addColumn<Size>("insertLength");
    QTest::addColumn<Address>("insertStorageOffset");
    QTest::addColumn<Size>("expectedTableSize");
    QTest::addColumn<QVector<Piece>>("expectedPieces");
    QTest::addColumn<QVector<StorageDataTestData>>("testData");

    QTest::newRow("inserting-to-empty")
        << 0
        << 0 << Width << ChangeStart
        << Width
        << QVector<Piece> {
            {ChangeStart, Width, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {0, true, ChangeStart, Piece::ChangeStorage},
            {Width - 1, true, ChangeEnd, Piece::ChangeStorage},
            {Width, false, 0, Piece::OriginalStorage}};
    QTest::newRow("inserting-one-at-begin")
        << BaseSize
        << 0 << Width << ChangeStart
        << BaseSize + Width
        << QVector<Piece> {
            {ChangeStart, Width, Piece::ChangeStorage},
            {0, BaseSize, Piece::OriginalStorage}}
        << QVector<StorageDataTestData> {
            {0, true, ChangeStart, Piece::ChangeStorage},
            {Width - 1, true, ChangeEnd, Piece::ChangeStorage},
            {Width, true, 0, Piece::OriginalStorage},
            {BaseSize + Width - 1, true, BaseSize - 1, Piece::OriginalStorage},
            {BaseSize + Width, false, 0, Piece::OriginalStorage}};
    QTest::newRow("inserting-one-in-middle")
        << BaseSize
        << Start << Width << ChangeStart
        << BaseSize + Width
        << QVector<Piece> {
            {0, Start, Piece::OriginalStorage},
            {ChangeStart, Width, Piece::ChangeStorage},
            {Start, BaseSize - Start, Piece::OriginalStorage}}
        << QVector<StorageDataTestData> {
            {Start - 1, true, Start - 1, Piece::OriginalStorage},
            {Start, true, ChangeStart, Piece::ChangeStorage},
            {End, true, ChangeEnd, Piece::ChangeStorage},
            {End + 1, true, Start, Piece::OriginalStorage},
            {BaseSize + Width - 1, true, BaseSize - 1, Piece::OriginalStorage},
            {BaseSize + Width, false, 0, Piece::OriginalStorage}};
    QTest::newRow("inserting-one-at-end")
        << BaseSize
        << BaseSize << Width << ChangeStart
        << BaseSize + Width
        << QVector<Piece> {
            {0, BaseSize, Piece::OriginalStorage},
            {ChangeStart, Width, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {BaseSize - 1, true, BaseSize - 1, Piece::OriginalStorage},
            {BaseSize, true, ChangeStart, Piece::ChangeStorage},
            {BaseSize + Width - 1, true, ChangeEnd, Piece::ChangeStorage},
            {BaseSize + Width, false, 0, Piece::OriginalStorage}};
}

void PieceTableTest::testInsert()
{
    QFETCH(const Size, initTableSize);
    QFETCH(const Address, insertDataOffset);
    QFETCH(const Size, insertLength);
    QFETCH(const Address, insertStorageOffset);
    QFETCH(const Size, expectedTableSize);
    QFETCH(const QVector<Piece>, expectedPieces);
    QFETCH(const QVector<StorageDataTestData>, testData);

    PieceTable pieceTable;
    pieceTable.init(initTableSize);

    // tested action
    pieceTable.insert(insertDataOffset, insertLength, insertStorageOffset);

    // check result
    QCOMPARE(pieceTable.size(), expectedTableSize);

    for (const auto& testDataEntry : testData) {
        Piece::StorageType storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset);

        QCOMPARE(result, testDataEntry.expectedResult);
        if (testDataEntry.expectedResult) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset);
            QCOMPARE(storageId, testDataEntry.expectedStorageId);
        }
    }
    compare(pieceTable, expectedPieces);
}

void PieceTableTest::testInsertMulti()
{
    PieceTable pieceTable;
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

    QVector<StorageDataTestData> testData;

    // all borders
    // 12: begin
    Address byteArrayOffset = 0;
    testData.append({byteArrayOffset, true, changeStarts[12], Piece::ChangeStorage});

    // 12: end
    byteArrayOffset += BaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[12] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[9], Piece::ChangeStorage});

    // 11: begin
    byteArrayOffset += HalfBaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[9] + HalfBaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[11], Piece::ChangeStorage});

    // 11: end
    byteArrayOffset += BaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[11] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[9] + HalfBaseSize, Piece::ChangeStorage});

    // 10: begin
    byteArrayOffset += HalfBaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[9] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[10], Piece::ChangeStorage});

    // 10: end
    byteArrayOffset += BaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[10] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, 0, Piece::OriginalStorage});

    // 8: begin
    byteArrayOffset += HalfBaseSize;
    testData.append({byteArrayOffset - 1, true, HalfBaseSize - 1, Piece::OriginalStorage});
    testData.append({byteArrayOffset, true, changeStarts[8], Piece::ChangeStorage});

    // 8: end
    byteArrayOffset += BaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[8] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[5], Piece::ChangeStorage});

    // 7: begin
    byteArrayOffset += HalfBaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[5] + HalfBaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[7], Piece::ChangeStorage});

    // 7: end
    byteArrayOffset += BaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[7] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[5] + HalfBaseSize, Piece::ChangeStorage});

    // 6: begin
    byteArrayOffset += HalfBaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[5] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[6], Piece::ChangeStorage});

    // 6: end
    byteArrayOffset += BaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[6] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, HalfBaseSize, Piece::OriginalStorage});

    // 4: begin
    byteArrayOffset += HalfBaseSize;
    testData.append({byteArrayOffset - 1, true, BaseSize - 1, Piece::OriginalStorage});
    testData.append({byteArrayOffset, true, changeStarts[4], Piece::ChangeStorage});

    // 4: end
    byteArrayOffset += BaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[4] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[1], Piece::ChangeStorage});

    // 3: begin
    byteArrayOffset += HalfBaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[1] + HalfBaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[3], Piece::ChangeStorage});

    // 3: end
    byteArrayOffset += BaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[3] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[1] + HalfBaseSize, Piece::ChangeStorage});

    // 2: begin
    byteArrayOffset += HalfBaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[1] + BaseSize - 1, Piece::ChangeStorage});
    testData.append({byteArrayOffset, true, changeStarts[2], Piece::ChangeStorage});

    // 2: end
    byteArrayOffset += BaseSize;
    testData.append({byteArrayOffset - 1, true, changeStarts[2] + BaseSize - 1, Piece::ChangeStorage});

    testData.append({byteArrayOffset, false, 0, Piece::OriginalStorage});

    for (const auto& testDataEntry : testData) {
        Piece::StorageType storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset);

        QCOMPARE(result, testDataEntry.expectedResult);
        if (testDataEntry.expectedResult) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset);
            QCOMPARE(storageId, testDataEntry.expectedStorageId);
        }
    }
}

void PieceTableTest::testRemove_data()
{
    QTest::addColumn<int>("multiFillCount");
    QTest::addColumn<AddressRange>("removeRange");
    QTest::addColumn<Size>("expectedTableSize");
    QTest::addColumn<QVector<Piece>>("expectedPieces");
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
        << BaseSize - removeRange.width()
        << QVector<Piece> {
            {removeRange.nextBehindEnd(), BaseSize - removeRange.width(), Piece::OriginalStorage}}
        << QVector<StorageDataTestData> {
            {0, true, Start, Piece::OriginalStorage},
            {BaseSize - Start - 1, true, BaseSize - 1, Piece::OriginalStorage},
            {BaseSize - Start, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange(Start, End);
    QTest::newRow("removing-at-middle")
        << 0 << removeRange
        << BaseSize - removeRange.width()
        << QVector<Piece> {
            {0, removeRange.start(), Piece::OriginalStorage},
            {removeRange.nextBehindEnd(), BaseSize - removeRange.end() - 1, Piece::OriginalStorage}}
        << QVector<StorageDataTestData> {
            {Start - 1, true, Start - 1, Piece::OriginalStorage},
            {Start, true, End + 1, Piece::OriginalStorage},
            {BaseSize - Width - 1, true, BaseSize - 1, Piece::OriginalStorage},
            {BaseSize - Width, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange(End + 1, BaseSize - 1);
    QTest::newRow("removing-at-end")
        << 0 << removeRange
        << End + 1
        << QVector<Piece> {
            {0, BaseSize - removeRange.width(), Piece::OriginalStorage}}
        << QVector<StorageDataTestData> {
            {End, true, End, Piece::OriginalStorage},
            {End + 1, false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-all")
        << 0 << AddressRange::fromWidth(BaseSize)
        << 0
        << QVector<Piece>()
        << QVector<StorageDataTestData> {
            {0, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset + Start, Width);
    QTest::newRow("removing-inside-piece-in-middle")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, Start, Piece::ChangeStorage},
            {removeRange.nextBehindEnd(), 3 * BaseSize - removeRange.end() - 1, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {midPieceOffset + Start - 1, true, changeStarts[mid] + Start - 1, Piece::ChangeStorage},
            {midPieceOffset + Start, true, changeStarts[mid] + End + 1, Piece::ChangeStorage},
            {fullSize - Width - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - Width, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset, Start);
    QTest::newRow("removing-start-of-piece-in-middle")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize + Start, BaseSize - removeRange.width(), Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {midPieceOffset - 1, true, changeStarts[mid - 1] + BaseSize - 1, Piece::ChangeStorage},
            {midPieceOffset, true, changeStarts[mid] + Start, Piece::ChangeStorage},
            {fullSize - Start - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - Start, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset + End + 1, BaseSize - (End + 1));
    QTest::newRow("removing-end-of-piece-in-middle")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize - removeRange.width(), Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {midPieceOffset + End, true, changeStarts[mid] + End, Piece::ChangeStorage},
            {midPieceOffset + End + 1, true, changeStarts[mid + 1], Piece::ChangeStorage},
            {fullSize - (BaseSize - End - 1) - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - (BaseSize - End - 1), false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset, BaseSize);
    QTest::newRow("removing-whole-piece-in-middle")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {midPieceOffset - 1, true, changeStarts[mid - 1] + BaseSize - 1, Piece::ChangeStorage},
            {midPieceOffset, true, changeStarts[mid + 1], Piece::ChangeStorage},
            {fullSize - BaseSize - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset, BaseSize + Start);
    QTest::newRow("removing-whole-piece-and-start-of-next-in-middke")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize + Start, BaseSize - Start, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {midPieceOffset - 1, true, changeStarts[mid - 1] + BaseSize - 1, Piece::ChangeStorage},
            {midPieceOffset, true, changeStarts[mid + 1] + Start, Piece::ChangeStorage},
            {fullSize - BaseSize - Start - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize - Start, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset - (BaseSize - End - 1), BaseSize + BaseSize - (End + 1));
    QTest::newRow("removing-whole-piece-and-end-of-previous-in-middle")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, End + 1, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {midPieceOffset - (BaseSize - End - 1) - 1, true, changeStarts[mid - 1] + End, Piece::ChangeStorage},
            {midPieceOffset - (BaseSize - End - 1), true, changeStarts[mid + 1], Piece::ChangeStorage},
            {fullSize - BaseSize - (BaseSize - End - 1) - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize - (BaseSize - End - 1), false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset - (BaseSize - End - 1), Start + BaseSize + BaseSize - (End + 1));
    QTest::newRow("removing-end-of-previous-whole-and-start-of-next-in-middle")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, End + 1, Piece::ChangeStorage},
            {BaseSize + Start, BaseSize - Start, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {midPieceOffset - (BaseSize - End - 1) - 1, true, changeStarts[mid - 1] + BaseSize - (BaseSize - End - 1) - 1, Piece::ChangeStorage},
            {midPieceOffset - (BaseSize - End - 1), true, changeStarts[mid + 1] + Start, Piece::ChangeStorage},
            {fullSize - BaseSize - (BaseSize - End - 1) - Start - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize - (BaseSize - End - 1) - Start, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(Start);
    QTest::newRow("removing-start-of-piece-at-start")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize + Start, BaseSize - Start, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {0, true, changeStarts[1] + Start, Piece::ChangeStorage},
            {fullSize - Start - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - Start, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(BaseSize);
    QTest::newRow("removing-whole-piece-at-start")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {0, true, changeStarts[2], Piece::ChangeStorage},
            {fullSize - BaseSize - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(BaseSize + Start);
    QTest::newRow("removing-whole-piece-and-start-of-next-at-start")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {3 * BaseSize + Start, BaseSize - Start, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {0, true, changeStarts[2] + Start, Piece::ChangeStorage},
            {fullSize - BaseSize - Start - 1, true, changeStarts[pieceCount] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize - Start, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(fullSize - BaseSize + End + 1, BaseSize - (End + 1));
    QTest::newRow("removing-end-of-piece-at-end")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, End + 1, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {fullSize - BaseSize + End, true, changeStarts[pieceCount] + End, Piece::ChangeStorage},
            {fullSize - (BaseSize - End - 1), false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(fullSize - BaseSize, BaseSize);
    QTest::newRow("removing-whole-piece-at-end")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {fullSize - BaseSize - 1, true, changeStarts[pieceCount - 1] + BaseSize - 1, Piece::ChangeStorage},
            {fullSize - BaseSize, false, 0, Piece::OriginalStorage}};
    removeRange = AddressRange::fromWidth(fullSize - BaseSize - (BaseSize - End - 1), BaseSize + BaseSize - (End + 1));
    QTest::newRow("removing-whole-piece-and-end-of-previous-at-end")
        << pieceCount << removeRange
        << fullSize - removeRange.width()
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, End + 1, Piece::ChangeStorage}}
        << QVector<StorageDataTestData> {
            {fullSize - BaseSize - (BaseSize - End - 1) - 1, true, changeStarts[pieceCount - 1] + End, Piece::ChangeStorage},
            {fullSize - BaseSize - (BaseSize - End - 1), false, 0, Piece::OriginalStorage}};
    QTest::newRow("removing-all-pieces")
        << pieceCount << AddressRange::fromWidth(fullSize)
        << 0
        << QVector<Piece>()
        << QVector<StorageDataTestData> {
            {0, false, 0, Piece::OriginalStorage}};
}

void PieceTableTest::testRemove()
{
    QFETCH(const int, multiFillCount);
    QFETCH(const AddressRange, removeRange);
    QFETCH(const Size, expectedTableSize);
    QFETCH(const QVector<Piece>, expectedPieces);
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
        Piece::StorageType storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset);

        QCOMPARE(result, testDataEntry.expectedResult);
        if (testDataEntry.expectedResult) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset);
            QCOMPARE(storageId, testDataEntry.expectedStorageId);
        }
    }
    compare(pieceTable, expectedPieces);
}

void PieceTableTest::testSwap_data()
{
    QTest::addColumn<Address>("firstStart");
    QTest::addColumn<AddressRange>("secondRange");
    QTest::addColumn<QVector<Piece>>("expectedPieces");
    QTest::addColumn<QVector<StorageDataTestData>>("testData");

    AddressRange moveRange = AddressRange::fromWidth(End + 1, BaseSize - (End + 1));
    QTest::newRow("moving-end-at-begin")
        << 0 << moveRange
        << QVector<Piece> {
            {moveRange, Piece::OriginalStorage},
            {0, moveRange.start(), Piece::OriginalStorage}}
        << QVector<StorageDataTestData> {
            {0, true, End + 1, Piece::OriginalStorage},
            {BaseSize - End - 2, true, BaseSize - 1, Piece::OriginalStorage},
            {BaseSize - End - 1, true, 0, Piece::OriginalStorage},
            {BaseSize - 1, true, End, Piece::OriginalStorage},
            {BaseSize, false, -1, Piece::OriginalStorage}};
    moveRange = AddressRange::fromWidth(End + 1, BaseSize - (End + 1));
    QTest::newRow("moving-end-at-mid")
        << Start << moveRange
        << QVector<Piece> {
            {0, Start, Piece::OriginalStorage},
            {moveRange, Piece::OriginalStorage},
            {Start, Width, Piece::OriginalStorage}}
        << QVector<StorageDataTestData> {
            {Start - 1, true, Start - 1, Piece::OriginalStorage},
            {Start, true, End + 1, Piece::OriginalStorage},
            {Start + BaseSize - End - 2, true, BaseSize - 1, Piece::OriginalStorage},
            {Start + BaseSize - End - 1, true, Start, Piece::OriginalStorage},
            {BaseSize - 1, true, End, Piece::OriginalStorage},
            {BaseSize, false, -1, Piece::OriginalStorage}};
    moveRange = AddressRange::fromWidth(Start, Width);
    QTest::newRow("moving-mid-at-begin")
        << 0 << moveRange
        << QVector<Piece> {
            {moveRange, Piece::OriginalStorage},
            {0, moveRange.start(), Piece::OriginalStorage},
            {moveRange.nextBehindEnd(), BaseSize - moveRange.nextBehindEnd(), Piece::OriginalStorage}}
        << QVector<StorageDataTestData> {
            {0, true, Start, Piece::OriginalStorage},
            {Width - 1, true, End, Piece::OriginalStorage},
            {Width, true, 0, Piece::OriginalStorage},
            {End, true, Start - 1, Piece::OriginalStorage},
            {End + 1, true, End + 1, Piece::OriginalStorage}};
    const Address mid = (End + Start) / 2;
    moveRange = AddressRange::fromWidth(mid, End - mid + 1);
    QTest::newRow("moving-mid-at-mid")
        << Start << moveRange
        << QVector<Piece> {
            {0, Start, Piece::OriginalStorage},
            {moveRange, Piece::OriginalStorage},
            {Start,  Width - moveRange.width(), Piece::OriginalStorage},
            {moveRange.nextBehindEnd(), BaseSize - moveRange.nextBehindEnd(), Piece::OriginalStorage}}
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
    QFETCH(const QVector<Piece>, expectedPieces);
    QFETCH(const QVector<StorageDataTestData>, testData);

    PieceTable pieceTable;
    pieceTable.init(BaseSize);

    // tested action
    pieceTable.swap(firstStart, secondRange);

    // check result
    QCOMPARE(pieceTable.size(), BaseSize);

    for (const auto& testDataEntry : testData) {
        Piece::StorageType storageId;
        Address storageOffset;

        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset);

        QCOMPARE(result, testDataEntry.expectedResult);
        if (testDataEntry.expectedResult) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset);
            QCOMPARE(storageId, testDataEntry.expectedStorageId);
        }
    }
    compare(pieceTable, expectedPieces);
}

}

QTEST_GUILESS_MAIN(KPieceTable::PieceTableTest)

#include "moc_piecetabletest.cpp"
