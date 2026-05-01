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
#include <numeric>

namespace KPieceTable {
struct StorageDataTestData
{
    static StorageDataTestData invalid(Address dataOffset)
    { return {dataOffset, false, 0, Piece::OriginalStorage}; }
    static StorageDataTestData valid(Address dataOffset, Address storageOffset, Piece::StorageType storageId)
    { return {dataOffset, true, storageOffset, storageId}; }

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
    const Size expectedTableSize = std::accumulate(expectedPieces.begin(), expectedPieces.end(), 0, [](Size sum, const Piece& piece) {
        return sum + piece.width();
    });

    QCOMPARE(pieceTable.size(), expectedTableSize);

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
    QTest::addColumn<QVector<Piece>>("expectedPieces");

    QTest::newRow("inserting-to-empty")
        << 0
        << 0 << Width << ChangeStart
        << QVector<Piece> {
            {ChangeStart, Width, Piece::ChangeStorage}};
    QTest::newRow("inserting-one-at-begin")
        << BaseSize
        << 0 << Width << ChangeStart
        << QVector<Piece> {
            {ChangeStart, Width, Piece::ChangeStorage},
            {0, BaseSize, Piece::OriginalStorage}};
    QTest::newRow("inserting-one-in-middle")
        << BaseSize
        << Start << Width << ChangeStart
        << QVector<Piece> {
            {0, Start, Piece::OriginalStorage},
            {ChangeStart, Width, Piece::ChangeStorage},
            {Start, BaseSize - Start, Piece::OriginalStorage}};
    QTest::newRow("inserting-one-at-end")
        << BaseSize
        << BaseSize << Width << ChangeStart
        << QVector<Piece> {
            {0, BaseSize, Piece::OriginalStorage},
            {ChangeStart, Width, Piece::ChangeStorage}};
}

void PieceTableTest::testInsert()
{
    QFETCH(const Size, initTableSize);
    QFETCH(const Address, insertDataOffset);
    QFETCH(const Size, insertLength);
    QFETCH(const Address, insertStorageOffset);
    QFETCH(const QVector<Piece>, expectedPieces);

    PieceTable pieceTable;
    pieceTable.init(initTableSize);

    // tested action
    pieceTable.insert(insertDataOffset, insertLength, insertStorageOffset);

    // check result
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

    const QVector<Piece> expectedPieces {
        {changeStarts[12],      BaseSize,       Piece::ChangeStorage},
        {changeStarts[9],       Mid,            Piece::ChangeStorage}, // part ninth
        {changeStarts[11],      BaseSize,       Piece::ChangeStorage},
        {changeStarts[9] + Mid, Mid + BaseSize, Piece::ChangeStorage}, // split ninth + merged tenth
        {  0,  Mid, Piece::OriginalStorage},  // part original
        {changeStarts[8],       BaseSize,       Piece::ChangeStorage},
        {changeStarts[5],       Mid,            Piece::ChangeStorage}, // part fifth
        {changeStarts[7],       BaseSize,       Piece::ChangeStorage},
        {changeStarts[5] + Mid, Mid + BaseSize, Piece::ChangeStorage}, // split fifth + merged sixth
        { Mid,  Mid, Piece::OriginalStorage}, // split original
        {changeStarts[4],       BaseSize,       Piece::ChangeStorage},
        {changeStarts[1],       Mid,            Piece::ChangeStorage}, // part first
        {changeStarts[3],       BaseSize,       Piece::ChangeStorage},
        {changeStarts[1] + Mid, Mid + BaseSize, Piece::ChangeStorage}, // split first + merged second
    };

    compare(pieceTable, expectedPieces);
}

void PieceTableTest::testRemove_data()
{
    QTest::addColumn<int>("multiFillCount");
    QTest::addColumn<AddressRange>("removeRange");
    QTest::addColumn<QVector<Piece>>("expectedPieces");

    // removing a lot:
    const int pieceCount = 5;
    const int mid = (pieceCount + 1) / 2;
    const Address midPieceOffset = BaseSize * (mid - 1);
    const Size fullSize = pieceCount * BaseSize;

    AddressRange removeRange = AddressRange(0, Start - 1);
    QTest::newRow("removing-at-begin")
        << 0 << removeRange
        << QVector<Piece> {
            {removeRange.nextBehindEnd(), BaseSize - removeRange.width(), Piece::OriginalStorage}};
    removeRange = AddressRange(Start, End);
    QTest::newRow("removing-at-middle")
        << 0 << removeRange
        << QVector<Piece> {
            {0, removeRange.start(), Piece::OriginalStorage},
            {removeRange.nextBehindEnd(), BaseSize - removeRange.end() - 1, Piece::OriginalStorage}};
    removeRange = AddressRange(End + 1, BaseSize - 1);
    QTest::newRow("removing-at-end")
        << 0 << removeRange
        << QVector<Piece> {
            {0, BaseSize - removeRange.width(), Piece::OriginalStorage}};
    QTest::newRow("removing-all")
        << 0 << AddressRange::fromWidth(BaseSize)
        << QVector<Piece>();
    removeRange = AddressRange::fromWidth(midPieceOffset + Start, Width);
    QTest::newRow("removing-inside-piece-in-middle")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, Start, Piece::ChangeStorage},
            {removeRange.nextBehindEnd(), 3 * BaseSize - removeRange.end() - 1, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset, Start);
    QTest::newRow("removing-start-of-piece-in-middle")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize + Start, BaseSize - removeRange.width(), Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset + End + 1, BaseSize - (End + 1));
    QTest::newRow("removing-end-of-piece-in-middle")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize - removeRange.width(), Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset, BaseSize);
    QTest::newRow("removing-whole-piece-in-middle")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset, BaseSize + Start);
    QTest::newRow("removing-whole-piece-and-start-of-next-in-middle")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize + Start, BaseSize - Start, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset - (BaseSize - End - 1), BaseSize + BaseSize - (End + 1));
    QTest::newRow("removing-whole-piece-and-end-of-previous-in-middle")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, End + 1, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(midPieceOffset - (BaseSize - End - 1), Start + BaseSize + BaseSize - (End + 1));
    QTest::newRow("removing-end-of-previous-whole-and-start-of-next-in-middle")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, End + 1, Piece::ChangeStorage},
            {BaseSize + Start, BaseSize - Start, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(Start);
    QTest::newRow("removing-start-of-piece-at-start")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize + Start, BaseSize - Start, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(BaseSize);
    QTest::newRow("removing-whole-piece-at-start")
        << pieceCount << removeRange
        << QVector<Piece> {
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(BaseSize + Start);
    QTest::newRow("removing-whole-piece-and-start-of-next-at-start")
        << pieceCount << removeRange
        << QVector<Piece> {
            {3 * BaseSize + Start, BaseSize - Start, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(fullSize - BaseSize + End + 1, BaseSize - (End + 1));
    QTest::newRow("removing-end-of-piece-at-end")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage},
            {0, End + 1, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(fullSize - BaseSize, BaseSize);
    QTest::newRow("removing-whole-piece-at-end")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, BaseSize, Piece::ChangeStorage}};
    removeRange = AddressRange::fromWidth(fullSize - BaseSize - (BaseSize - End - 1), BaseSize + BaseSize - (End + 1));
    QTest::newRow("removing-whole-piece-and-end-of-previous-at-end")
        << pieceCount << removeRange
        << QVector<Piece> {
            {4 * BaseSize, BaseSize, Piece::ChangeStorage},
            {3 * BaseSize, BaseSize, Piece::ChangeStorage},
            {2 * BaseSize, BaseSize, Piece::ChangeStorage},
            {BaseSize, End + 1, Piece::ChangeStorage}};
    QTest::newRow("removing-all-pieces")
        << pieceCount << AddressRange::fromWidth(fullSize)
        << QVector<Piece>();
}

void PieceTableTest::testRemove()
{
    QFETCH(const int, multiFillCount);
    QFETCH(const AddressRange, removeRange);
    QFETCH(const QVector<Piece>, expectedPieces);

    PieceTable pieceTable;
    if (multiFillCount > 0) {
        fillWithSize(&pieceTable, multiFillCount);
    } else {
        pieceTable.init(BaseSize);
    }

    // tested action
    pieceTable.remove(removeRange);

    // check result
    compare(pieceTable, expectedPieces);
}

void PieceTableTest::testSwap_data()
{
    QTest::addColumn<Address>("firstStart");
    QTest::addColumn<AddressRange>("secondRange");
    QTest::addColumn<QVector<Piece>>("expectedPieces");

    AddressRange moveRange = AddressRange::fromWidth(End + 1, BaseSize - (End + 1));
    QTest::newRow("moving-end-at-begin")
        << 0 << moveRange
        << QVector<Piece> {
            {moveRange, Piece::OriginalStorage},
            {0, moveRange.start(), Piece::OriginalStorage}};
    moveRange = AddressRange::fromWidth(End + 1, BaseSize - (End + 1));
    QTest::newRow("moving-end-at-mid")
        << Start << moveRange
        << QVector<Piece> {
            {0, Start, Piece::OriginalStorage},
            {moveRange, Piece::OriginalStorage},
            {Start, Width, Piece::OriginalStorage}};
    moveRange = AddressRange::fromWidth(Start, Width);
    QTest::newRow("moving-mid-at-begin")
        << 0 << moveRange
        << QVector<Piece> {
            {moveRange, Piece::OriginalStorage},
            {0, moveRange.start(), Piece::OriginalStorage},
            {moveRange.nextBehindEnd(), BaseSize - moveRange.nextBehindEnd(), Piece::OriginalStorage}};
    const Address mid = (End + Start) / 2;
    moveRange = AddressRange::fromWidth(mid, End - mid + 1);
    QTest::newRow("moving-mid-at-mid")
        << Start << moveRange
        << QVector<Piece> {
            {0, Start, Piece::OriginalStorage},
            {moveRange, Piece::OriginalStorage},
            {Start,  Width - moveRange.width(), Piece::OriginalStorage},
            {moveRange.nextBehindEnd(), BaseSize - moveRange.nextBehindEnd(), Piece::OriginalStorage}};
}

void PieceTableTest::testSwap()
{
    QFETCH(const Address, firstStart);
    QFETCH(const AddressRange, secondRange);
    QFETCH(const QVector<Piece>, expectedPieces);

    PieceTable pieceTable;
    pieceTable.init(BaseSize);

    // tested action
    pieceTable.swap(firstStart, secondRange);

    // check result
    QCOMPARE(pieceTable.size(), BaseSize);

    compare(pieceTable, expectedPieces);
}

void PieceTableTest::testGetStorageData_data()
{
    static constexpr Address Start2 = End + 5;
    static constexpr Size Width2 = Width;
    static constexpr Address End2 = Start2 + Width2 - 1;

    QTest::addColumn<QVector<Piece>>("pieces");
    QTest::addColumn<QVector<StorageDataTestData>>("testData");

    QTest::newRow("void")
        << QVector<Piece> {
        }
        << QVector<StorageDataTestData> {
            StorageDataTestData::invalid(0),
            StorageDataTestData::invalid(1),
        };

    QTest::newRow("one-original")
        << QVector<Piece> {
            {Start, Width, Piece::OriginalStorage},
        }
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(1, Start + 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width - 2, End - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width - 1, End, Piece::OriginalStorage),
            StorageDataTestData::invalid(Width),
        };

    QTest::newRow("one-original-short")
        << QVector<Piece> {
            {Start, 1, Piece::OriginalStorage},
        }
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, Start, Piece::OriginalStorage),
            StorageDataTestData::invalid(1),
        };

    QTest::newRow("one-change")
        << QVector<Piece> {
            {Start2, Width2, Piece::ChangeStorage},
        }
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, Start2, Piece::ChangeStorage),
            StorageDataTestData::valid(1, Start2 + 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 - 2, End2 - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 - 1, End2, Piece::ChangeStorage),
            StorageDataTestData::invalid(Width2),
        };

    QTest::newRow("two-original")
        << QVector<Piece> {
            {Start, Width, Piece::OriginalStorage},
            {Start2, Width2, Piece::OriginalStorage},
        }
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(1, Start + 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width - 2, End - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width - 1, End, Piece::OriginalStorage),
            StorageDataTestData::valid(Width, Start2, Piece::OriginalStorage),
            StorageDataTestData::valid(Width + 1, Start2 + 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width + Width2 - 2, End2 - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width + Width2 - 1, End2, Piece::OriginalStorage),
            StorageDataTestData::invalid(Width + Width2),
        };

    QTest::newRow("two-change")
        << QVector<Piece> {
            {Start2, Width2, Piece::ChangeStorage},
            {Start, Width, Piece::ChangeStorage},
        }
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, Start2, Piece::ChangeStorage),
            StorageDataTestData::valid(1, Start2 + 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 - 2, End2 - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 - 1, End2, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2, Start, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 + 1, Start + 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 + Width - 2, End - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 + Width - 1, End, Piece::ChangeStorage),
            StorageDataTestData::invalid(Width2 + Width),
        };

    QTest::newRow("two-original-change")
        << QVector<Piece> {
            {Start, Width, Piece::OriginalStorage},
            {Start2, Width2, Piece::ChangeStorage},
        }
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(1, Start + 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width - 2, End - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width - 1, End, Piece::OriginalStorage),
            StorageDataTestData::valid(Width, Start2, Piece::ChangeStorage),
            StorageDataTestData::valid(Width + 1, Start2 + 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width + Width2 - 2, End2 - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width + Width2 - 1, End2, Piece::ChangeStorage),
            StorageDataTestData::invalid(Width + Width2),
        };

    QTest::newRow("two-original-change-short")
        << QVector<Piece> {
            {Start, 1, Piece::OriginalStorage},
            {Start2, 1, Piece::ChangeStorage},
        }
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(1, Start2, Piece::ChangeStorage),
            StorageDataTestData::invalid(2),
        };

    QTest::newRow("two-change-original")
        << QVector<Piece> {
            {Start2, Width2, Piece::ChangeStorage},
            {Start, Width, Piece::OriginalStorage},
        }
        << QVector<StorageDataTestData> {
            StorageDataTestData::valid(0, Start2, Piece::ChangeStorage),
            StorageDataTestData::valid(1, Start2 + 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 - 2, End2 - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 - 1, End2, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(Width2 + 1, Start + 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width2 + Width - 2, End - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width2 + Width - 1, End, Piece::OriginalStorage),
            StorageDataTestData::invalid(Width2 + Width),
        };
}

void PieceTableTest::testGetStorageData()
{
    QFETCH(const QVector<Piece>, pieces);
    QFETCH(const QVector<StorageDataTestData>, testData);

    // setup piece-table
    PieceList pieceList;
    for (const auto& piece : pieces) {
        pieceList.append(piece);
    }

    PieceTable pieceTable;
    pieceTable.insert(0, pieceList);

    for (const auto& testDataEntry : testData) {
        Piece::StorageType storageId;
        Address storageOffset;

        // tested action
        const bool result = pieceTable.getStorageData(&storageId, &storageOffset, testDataEntry.dataOffset());

        // check result
        QCOMPARE(result, testDataEntry.expectedResult());
        if (testDataEntry.expectedResult()) {
            QCOMPARE(storageOffset, testDataEntry.expectedStorageOffset());
            QCOMPARE(storageId, testDataEntry.expectedStorageId());
        }
    }
}

}

QTEST_GUILESS_MAIN(KPieceTable::PieceTableTest)

#include "moc_piecetabletest.cpp"
