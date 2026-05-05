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
Q_DECLARE_METATYPE(KPieceTable::Piece)
Q_DECLARE_METATYPE(KPieceTable::Piece::StorageType)
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

void compare(const PieceList& pieceList, const QVector<Piece>& expectedPieces)
{
    const Size expectedListSize = std::accumulate(expectedPieces.begin(), expectedPieces.end(), 0, [](Size sum, const Piece& piece) {
        return sum + piece.width();
    });


    // TODO: find some useful output with QCOMPARE
    if ((pieceList.size() != expectedPieces.size()) ||
        (pieceList.totalLength() != expectedListSize)) {
        for (const auto& piece : expectedPieces) {
            qDebug() << "Expected:" << piece;
        }
        for (const auto& piece : pieceList) {
            qDebug() << "Actual:" << piece;
        }
    }

    QCOMPARE(pieceList.totalLength(), expectedListSize);
    QCOMPARE(pieceList.size(), expectedPieces.size());

    int i = 0;
    for (const auto& piece : pieceList) {
        const auto& expectedPiece = expectedPieces[i++];
        QCOMPARE(piece, expectedPiece);
    }
}

void compare(const PieceTable& pieceTable, const QVector<Piece>& expectedPieces)
{
    const Size expectedTableSize = std::accumulate(expectedPieces.begin(), expectedPieces.end(), 0, [](Size sum, const Piece& piece) {
        return sum + piece.width();
    });


    // TODO: find some useful output with QCOMPARE
    if ((pieceTable.piecesSize() != expectedPieces.size()) ||
        (pieceTable.size() != expectedTableSize)) {
        for (const auto& piece : expectedPieces) {
            qDebug() << "Expected:" << piece;
        }
        for (const auto& piece : pieceTable) {
            qDebug() << "Actual:" << piece;
        }
    }

    QCOMPARE(pieceTable.size(), expectedTableSize);
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

void PieceTableTest::testInsertPieceListToEmpty_data()
{
    QTest::addColumn<QVector<Piece>>("insertPieces");
    QTest::addColumn<QVector<Piece>>("expectedPieces");

    QTest::newRow("empty")
        << QVector<Piece> {}
        << QVector<Piece> {};

    QTest::newRow("one")
        << QVector<Piece> {
            {Start, Width, Piece::ChangeStorage}}
        << QVector<Piece> {
            {Start, Width, Piece::ChangeStorage}};

    QTest::newRow("two")
        << QVector<Piece> {
            {Start, Width, Piece::ChangeStorage},
            {0, Width, Piece::OriginalStorage}}
        << QVector<Piece> {
            {Start, Width, Piece::ChangeStorage},
            {0, Width, Piece::OriginalStorage}};
}

void PieceTableTest::testInsertPieceListToEmpty()
{
    QFETCH(const QVector<Piece>, insertPieces);
    QFETCH(const QVector<Piece>, expectedPieces);

    PieceList insertPieceList;
    for (const auto& piece : insertPieces) {
        insertPieceList.append(piece);
    }

    PieceTable pieceTable;

    // tested action
    pieceTable.insert(0, insertPieceList);

    // check result
    compare(pieceTable, expectedPieces);
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
    QTest::addColumn<QVector<Piece>>("initPieces");
    QTest::addColumn<AddressRange>("removeRange");
    QTest::addColumn<QVector<Piece>>("expectedRemovedPieces");
    QTest::addColumn<QVector<Piece>>("expectedPieces");

    const Piece originalPieceA {0, BaseSize, Piece::OriginalStorage};
    const Piece originalPieceA1 {0, 1, Piece::OriginalStorage};
    const Piece originalPieceA12 {0, 2, Piece::OriginalStorage};
    const Piece originalPiece__A {2, BaseSize - 2, Piece::OriginalStorage};
    const Piece originalPiece_A {1, BaseSize - 1, Piece::OriginalStorage};
    const Piece originalPieceA_ {0, BaseSize - 1, Piece::OriginalStorage};
    const Piece originalPieceA__ {0, BaseSize - 2, Piece::OriginalStorage};
    const Piece originalPieceAM {0, BaseSize / 2, Piece::OriginalStorage};
    const Piece originalPiece1MA {BaseSize / 2, 1, Piece::OriginalStorage};
    const Piece originalPiece12MA {BaseSize / 2, 2, Piece::OriginalStorage};
    const Piece originalPiece_MA {BaseSize / 2 + 1, BaseSize / 2 - 1, Piece::OriginalStorage};
    const Piece originalPiece__MA {BaseSize / 2 + 2, BaseSize / 2 - 2, Piece::OriginalStorage};
    const Piece originalPiece1A {BaseSize - 1, 1, Piece::OriginalStorage};
    const Piece originalPiece12A {BaseSize - 2, 2, Piece::OriginalStorage};

    const Piece changePieceA {0, BaseSize, Piece::ChangeStorage};
    const Piece changePieceA1 {0, 1, Piece::ChangeStorage};
    const Piece changePieceA12 {0, 2, Piece::ChangeStorage};
    const Piece changePieceA_ {0, BaseSize - 1, Piece::ChangeStorage};
    const Piece changePieceA__ {0, BaseSize - 2, Piece::ChangeStorage};
    const Piece changePiece_A {1, BaseSize - 1, Piece::ChangeStorage};
    const Piece changePiece__A {2, BaseSize - 2, Piece::ChangeStorage};
    const Piece changePiece1A {BaseSize - 1, 1, Piece::ChangeStorage};
    const Piece changePiece12A {BaseSize - 2, 2, Piece::ChangeStorage};
    const Piece changePieceB {BaseSize, BaseSize, Piece::ChangeStorage};
    const Piece changePieceBC {BaseSize, BaseSize * 2, Piece::ChangeStorage};
    const Piece changePieceC {BaseSize * 2, BaseSize, Piece::ChangeStorage};
    const Piece changePieceD {BaseSize * 3, BaseSize, Piece::ChangeStorage};
    const Piece changePieceE {BaseSize * 4, BaseSize, Piece::ChangeStorage};

    AddressRange removeRange = AddressRange(0, 0);
    QTest::newRow("one-at-begin-one-multi")
        << QVector<Piece> {originalPieceA}
        << removeRange
        << QVector<Piece> {originalPieceA1}
        << QVector<Piece> {originalPiece_A};
    removeRange = AddressRange(0, 1);
    QTest::newRow("multi-at-begin-one-multi")
        << QVector<Piece> {originalPieceA}
        << removeRange
        << QVector<Piece> {originalPieceA12}
        << QVector<Piece> {originalPiece__A};
    removeRange = AddressRange::fromWidth(BaseSize / 2, 1);
    QTest::newRow("one-at-middle-one-multi")
        << QVector<Piece> {originalPieceA}
        << removeRange
        << QVector<Piece> {originalPiece1MA}
        << QVector<Piece> {originalPieceAM, originalPiece_MA};
    removeRange = AddressRange::fromWidth(BaseSize / 2, 2);
    QTest::newRow("multi-at-middle-one-multi")
        << QVector<Piece> {originalPieceA}
        << removeRange
        << QVector<Piece> {originalPiece12MA}
        << QVector<Piece> {originalPieceAM, originalPiece__MA};
    removeRange = AddressRange(BaseSize - 1, BaseSize - 1);
    QTest::newRow("one-at-end-one-multi")
        << QVector<Piece> {originalPieceA}
        << removeRange
        << QVector<Piece> {originalPiece1A}
        << QVector<Piece> {originalPieceA_};
    removeRange = AddressRange(BaseSize - 2, BaseSize - 1);
    QTest::newRow("multi-at-end-one-multi")
        << QVector<Piece> {originalPieceA}
        << removeRange
        << QVector<Piece> {originalPiece12A}
        << QVector<Piece> {originalPieceA__};
    removeRange = AddressRange(0, 0);
    QTest::newRow("all-one-one")
        << QVector<Piece> {originalPieceA1}
        << removeRange
        << QVector<Piece> {originalPieceA1}
        << QVector<Piece> {};
    removeRange = AddressRange::fromWidth(BaseSize);
    QTest::newRow("all-one-multi")
        << QVector<Piece> {originalPieceA}
        << removeRange
        << QVector<Piece> {originalPieceA}
        << QVector<Piece>();

    removeRange = AddressRange(0, 0);
    QTest::newRow("one-at-begin-first-two-multi")
        << QVector<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA1}
        << QVector<Piece> {originalPiece_A, changePieceA};
    removeRange = AddressRange(0, 1);
    QTest::newRow("multi-at-begin-first-two-multi")
        << QVector<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA12}
        << QVector<Piece> {originalPiece__A, changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize / 2, 1);
    QTest::newRow("one-at-middle-first-two-multi")
        << QVector<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QVector<Piece> {originalPiece1MA}
        << QVector<Piece> {originalPieceAM, originalPiece_MA, changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize / 2, 2);
    QTest::newRow("multi-at-middle-first-two-multi")
        << QVector<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QVector<Piece> {originalPiece12MA}
        << QVector<Piece> {originalPieceAM, originalPiece__MA, changePieceA};
    removeRange = AddressRange(BaseSize - 1, BaseSize - 1);
    QTest::newRow("one-at-end-first-two-multi-noncontinuous")
        << QVector<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QVector<Piece> {originalPiece1A}
        << QVector<Piece> {originalPieceA_, changePieceA};
    removeRange = AddressRange(BaseSize - 2, BaseSize - 1);
    QTest::newRow("multi-at-end-first-two-multi-noncontinuous")
        << QVector<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QVector<Piece> {originalPiece12A}
        << QVector<Piece> {originalPieceA__, changePieceA};
    removeRange = AddressRange(0, 0);
    QTest::newRow("all-first-two-one")
        << QVector<Piece> {originalPieceA1, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA1}
        << QVector<Piece> {changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize);
    QTest::newRow("all-first-two-multi")
        << QVector<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA}
        << QVector<Piece> {changePieceA};
    removeRange = AddressRange::fromWidth(0, BaseSize + 1);
    QTest::newRow("all-first-one-at-begin-second-two-multi")
        << QVector<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA, changePieceA1}
        << QVector<Piece> {changePiece_A};
    removeRange = AddressRange::fromWidth(0, BaseSize + 2);
    QTest::newRow("all-first-multi-at-begin-second-two-multi")
        << QVector<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA, changePieceA12}
        << QVector<Piece> {changePiece__A};

    removeRange = AddressRange::fromWidth(BaseSize, 1);
    QTest::newRow("one-at-begin-second-two-multi-noncontinuous")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {originalPieceA1}
        << QVector<Piece> {changePieceA, originalPiece_A};
    removeRange = AddressRange::fromWidth(BaseSize, 2);
    QTest::newRow("multi-at-begin-second-two-multi-noncontinuous")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {originalPieceA12}
        << QVector<Piece> {changePieceA, originalPiece__A};
    removeRange = AddressRange::fromWidth(BaseSize + BaseSize / 2, 1);
    QTest::newRow("one-at-middle-second-two-multi")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {originalPiece1MA}
        << QVector<Piece> {changePieceA, originalPieceAM, originalPiece_MA};
    removeRange = AddressRange::fromWidth(BaseSize + BaseSize / 2, 2);
    QTest::newRow("multi-at-middle-second-two-multi")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {originalPiece12MA}
        << QVector<Piece> {changePieceA, originalPieceAM, originalPiece__MA};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 1, 1);
    QTest::newRow("one-at-end-second-two-multi")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {originalPiece1A}
        << QVector<Piece> {changePieceA, originalPieceA_};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 2, 2);
    QTest::newRow("multi-at-end-second-two-multi")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {originalPiece12A}
        << QVector<Piece> {changePieceA, originalPieceA__};
    removeRange = AddressRange::fromWidth(BaseSize, 1);
    QTest::newRow("all-second-two-one")
        << QVector<Piece> {changePieceA, originalPieceA1}
        << removeRange
        << QVector<Piece> {originalPieceA1}
        << QVector<Piece> {changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize, BaseSize);
    QTest::newRow("all-second-two-multi")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {originalPieceA}
        << QVector<Piece> {changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize - 1, BaseSize + 1);
    QTest::newRow("all-second-one-at-end-first-two-multi")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {changePiece1A, originalPieceA}
        << QVector<Piece> {changePieceA_};
    removeRange = AddressRange::fromWidth(BaseSize - 2, BaseSize + 2);
    QTest::newRow("all-second-multi-at-end-first-two-multi")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {changePiece12A, originalPieceA}
        << QVector<Piece> {changePieceA__};

    removeRange = AddressRange::fromWidth(BaseSize - 1, 2);
    QTest::newRow("one-at-end-first-one-at-begin-second-two-multi")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {changePiece1A, originalPieceA1}
        << QVector<Piece> {changePieceA_, originalPiece_A};
    removeRange = AddressRange::fromWidth(BaseSize - 2, 4);
    QTest::newRow("multi-at-end-first-multi-at-begin-second-two-multi")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {changePiece12A, originalPieceA12}
        << QVector<Piece> {changePieceA__, originalPiece__A};
    removeRange = AddressRange::fromWidth(0, 2);
    QTest::newRow("all-two-one")
        << QVector<Piece> {changePieceA1, originalPieceA1}
        << removeRange
        << QVector<Piece> {changePieceA1, originalPieceA1}
        << QVector<Piece> {};
    removeRange = AddressRange::fromWidth(0, 2 * BaseSize);
    QTest::newRow("all-two-multi")
        << QVector<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QVector<Piece> {changePieceA, originalPieceA}
        << QVector<Piece> {};

    removeRange = AddressRange::fromWidth(2 * BaseSize, 1);
    QTest::newRow("one-at-begin-third-five-multi-noncontinuous")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA1}
        << QVector<Piece> {changePieceE, changePieceD, originalPiece_A, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize, 2);
    QTest::newRow("multi-at-begin-third-five-multi-noncontinuous")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA12}
        << QVector<Piece> {changePieceE, changePieceD, originalPiece__A, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize + BaseSize / 2, 1);
    QTest::newRow("one-at-middle-third-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {originalPiece1MA}
        << QVector<Piece> {changePieceE, changePieceD, originalPieceAM, originalPiece_MA, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize + BaseSize / 2, 2);
    QTest::newRow("multi-at-middle-third-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {originalPiece12MA}
        << QVector<Piece> {changePieceE, changePieceD, originalPieceAM, originalPiece__MA, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(3 * BaseSize - 1, 1);
    QTest::newRow("one-at-end-third-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {originalPiece1A}
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA_, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(3 * BaseSize - 2, 2);
    QTest::newRow("multi-at-end-third-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {originalPiece12A}
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA__, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize, 1);
    QTest::newRow("all-third-five-one-noncontinuous")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA1, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA1}
        << QVector<Piece> {changePieceE, changePieceD, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize, BaseSize);
    QTest::newRow("all-third-five-multi-noncontinuous")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA}
        << QVector<Piece> {changePieceE, changePieceD, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize, 1);
    QTest::newRow("all-third-five-one-continuous")
        << QVector<Piece> {changePieceE, changePieceB, originalPieceA1, changePieceC, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA1}
        << QVector<Piece> {changePieceE, changePieceBC, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize, BaseSize);
    QTest::newRow("all-third-five-multi-noncontinuous")
        << QVector<Piece> {changePieceE, changePieceB, originalPieceA, changePieceC, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA}
        << QVector<Piece> {changePieceE, changePieceBC, changePieceA};

    removeRange = AddressRange::fromWidth(2 * BaseSize, BaseSize + 1);
    QTest::newRow("all-third-one-at-begin-fourth-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceA, changePieceC}
        << removeRange
        << QVector<Piece> {originalPieceA, changePieceA1}
        << QVector<Piece> {changePieceE, changePieceD, changePiece_A, changePieceC};
    removeRange = AddressRange::fromWidth(2 * BaseSize, BaseSize + 2);
    QTest::newRow("all-third-multi-at-begin-fourth-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceA, changePieceC}
        << removeRange
        << QVector<Piece> {originalPieceA, changePieceA12}
        << QVector<Piece> {changePieceE, changePieceD, changePiece__A, changePieceC};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 1, BaseSize + 1);
    QTest::newRow("all-third-one-at-end-second-five-multi")
        << QVector<Piece> {changePieceE, changePieceA, originalPieceA, changePieceD, changePieceC}
        << removeRange
        << QVector<Piece> {changePiece1A, originalPieceA}
        << QVector<Piece> {changePieceE, changePieceA_, changePieceD, changePieceC};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 2, BaseSize + 2);
    QTest::newRow("all-third-multi-at-end-second-five-multi")
        << QVector<Piece> {changePieceE, changePieceA, originalPieceA, changePieceD, changePieceC}
        << removeRange
        << QVector<Piece> {changePiece12A, originalPieceA}
        << QVector<Piece> {changePieceE, changePieceA__, changePieceD, changePieceC};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 1, BaseSize + 2);
    QTest::newRow("all-third-one-at-end-second-one-at-begin-fourth-five-multi")
        << QVector<Piece> {changePieceE, changePieceA, changePieceD, originalPieceA, changePieceC}
        << removeRange
        << QVector<Piece> {changePiece1A, changePieceD, originalPieceA1}
        << QVector<Piece> {changePieceE, changePieceA_, originalPiece_A, changePieceC};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 2, BaseSize + 4);
    QTest::newRow("all-third-multi-at-end-second-multi-at-begin-fourth-five-multi")
        << QVector<Piece> {changePieceE, changePieceA, changePieceD, originalPieceA, changePieceC}
        << removeRange
        << QVector<Piece> {changePiece12A, changePieceD, originalPieceA12}
        << QVector<Piece> {changePieceE, changePieceA__, originalPiece__A, changePieceC};
    removeRange = AddressRange::fromWidth(BaseSize, 3 * BaseSize);
    QTest::newRow("all-second-third-fourth-five-multi-noncontinuous")
        << QVector<Piece> {changePieceE, changePieceA, changePieceD, originalPieceA, changePieceC}
        << removeRange
        << QVector<Piece> {changePieceA, changePieceD, originalPieceA}
        << QVector<Piece> {changePieceE, changePieceC};
    removeRange = AddressRange::fromWidth(BaseSize, 3 * BaseSize);
    QTest::newRow("all-second-third-fourth-five-multi-continuous")
        << QVector<Piece> {changePieceB, changePieceA, changePieceD, originalPieceA, changePieceC}
        << removeRange
        << QVector<Piece> {changePieceA, changePieceD, originalPieceA}
        << QVector<Piece> {changePieceBC};

    removeRange = AddressRange::fromWidth(0, 2);
    QTest::newRow("multi-at-begin-first-five-multi")
        << QVector<Piece> {originalPieceA, changePieceD, changePieceC, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {originalPieceA12}
        << QVector<Piece> {originalPiece__A, changePieceD, changePieceC, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize);
    QTest::newRow("all-first-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, changePieceC, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {changePieceE}
        << QVector<Piece> {changePieceD, changePieceC, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(0, BaseSize + 2);
    QTest::newRow("all-first-multi-at-begin-second-five-multi")
        << QVector<Piece> {changePieceD, originalPieceA, changePieceC, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {changePieceD, originalPieceA12}
        << QVector<Piece> {originalPiece__A, changePieceC, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(5 * BaseSize - 2, 2);
    QTest::newRow("multi-at-end-last-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, changePieceC, changePieceB, originalPieceA}
        << removeRange
        << QVector<Piece> {originalPiece12A}
        << QVector<Piece> {changePieceE, changePieceD, changePieceC, changePieceB, originalPieceA__};
    removeRange = AddressRange::fromWidth(4 * BaseSize, BaseSize);
    QTest::newRow("all-last-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, changePieceC, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {changePieceA}
        << QVector<Piece> {changePieceE, changePieceD, changePieceC, changePieceB};
    removeRange = AddressRange::fromWidth(4 * BaseSize - 2, BaseSize + 2);
    QTest::newRow("all-last-multi-at-end-fourth-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, changePieceC, originalPieceA, changePieceA}
        << removeRange
        << QVector<Piece> {originalPiece12A, changePieceA}
        << QVector<Piece> {changePieceE, changePieceD, changePieceC, originalPieceA__};

    removeRange = AddressRange::fromWidth(0, 5 * BaseSize);
    QTest::newRow("all-five-multi")
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QVector<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << QVector<Piece> {};
}

void PieceTableTest::testRemove()
{
    QFETCH(const QVector<Piece>, initPieces);
    QFETCH(const AddressRange, removeRange);
    QFETCH(const QVector<Piece>, expectedRemovedPieces);
    QFETCH(const QVector<Piece>, expectedPieces);

    PieceList initPieceList;
    for (const auto& piece : initPieces) {
        initPieceList.append(piece);
    }

    PieceTable pieceTable;
    pieceTable.insert(0, initPieceList);

    // tested action
    const PieceList removedPieces = pieceTable.remove(removeRange);

    // check result
    compare(removedPieces, expectedRemovedPieces);
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

void PieceTableTest::testReplaceOne_data()
{
    QTest::addColumn<QVector<Piece>>("initPieces");
    QTest::addColumn<Address>("replaceDataOffset");
    QTest::addColumn<Address>("replaceStorageOffset");
    QTest::addColumn<Piece::StorageType>("replaceStorageId");
    QTest::addColumn<Piece>("expectedReplacedPiece");
    QTest::addColumn<QVector<Piece>>("expectedPieces");

    const Piece changePieceA1 {0, 1, Piece::ChangeStorage};
    const Piece changePieceA2 {1, 1, Piece::ChangeStorage};
    const Piece changePieceA3 {2, 1, Piece::ChangeStorage};
    const Piece changePieceA12 {0, 2, Piece::ChangeStorage};
    const Piece changePieceA123 {0, 3, Piece::ChangeStorage};
    const Piece changePieceB {Start, Width, Piece::ChangeStorage};
    const Piece changePieceB1 {Start, 1, Piece::ChangeStorage};
    const Piece changePieceB2 {Start + 1, 1, Piece::ChangeStorage};
    const Piece changePiece_B {Start + 1, Width - 1, Piece::ChangeStorage};
    const Piece changePiece__B {Start + 2, Width - 2, Piece::ChangeStorage};

    const Piece originalPieceB {Start, Width, Piece::OriginalStorage};
    const Piece originalPieceB_ {Start, Width - 1, Piece::OriginalStorage};
    const Piece originalPieceB__ {Start, Width - 2, Piece::OriginalStorage};
    const Piece originalPieceB1 {Start, 1, Piece::OriginalStorage};
    const Piece originalPiece_B {Start + 1, Width - 1, Piece::OriginalStorage};
    const Piece originalPiece1B {End, 1, Piece::OriginalStorage};
    const Piece originalPieceBM {Start, Width / 2, Piece::OriginalStorage};
    const Piece originalPiece_MB {Start + Width / 2 + 1, Width / 2, Piece::OriginalStorage};

    QTest::newRow("one-with-one-only")
        << QVector<Piece> {originalPieceB1}
        << 0 << Start + 1 << Piece::ChangeStorage
        << originalPieceB1
        << QVector<Piece> {changePieceB2};

    QTest::newRow("one-with-one-first-continuous")
        << QVector<Piece> {originalPieceB1, changePieceA2}
        << 0 << 0 << Piece::ChangeStorage
        << originalPieceB1
        << QVector<Piece> {changePieceA12};

    QTest::newRow("one-with-one-first-noncontinuous")
        << QVector<Piece> {originalPieceB1, changePieceA1}
        << 0 << Start + 1 << Piece::ChangeStorage
        << originalPieceB1
        << QVector<Piece> {changePieceB2, changePieceA1};

    QTest::newRow("one-with-one-second-continuous")
        << QVector<Piece> {changePieceA1, originalPieceB1}
        << 1 << 1 << Piece::ChangeStorage
        << originalPieceB1
        << QVector<Piece> {changePieceA12};

    QTest::newRow("one-with-one-second-noncontinuous")
        << QVector<Piece> {changePieceA1, originalPieceB1}
        << 1 << Start + 1 << Piece::ChangeStorage
        << originalPieceB1
        << QVector<Piece> {changePieceA1, changePieceB2};

    QTest::newRow("one-with-one-three-continuous")
        << QVector<Piece> {changePieceA1, originalPieceB1, changePieceA3}
        << 1 << 1 << Piece::ChangeStorage
        << originalPieceB1
        << QVector<Piece> {changePieceA123};

    QTest::newRow("one-with-begin-multi")
        << QVector<Piece> {originalPieceB}
        << 0 << Start + 1 << Piece::ChangeStorage
        << originalPieceB1
        << QVector<Piece> {changePieceB2, originalPiece_B};

    QTest::newRow("one-with-middle-multi")
        << QVector<Piece> {originalPieceB}
        << Width / 2 << Start + 1 << Piece::ChangeStorage
        << Piece {Start + Width / 2, 1, Piece::OriginalStorage}
        << QVector<Piece> {originalPieceBM, changePieceB2, originalPiece_MB};

    QTest::newRow("one-with-end-multi")
        << QVector<Piece> {originalPieceB}
        << Width - 1 << Start + 1 << Piece::ChangeStorage
        << originalPiece1B
        << QVector<Piece> {originalPieceB_, changePieceB2};

    QTest::newRow("one-with-end-multi-next-continuous")
        << QVector<Piece> {originalPieceB, changePiece_B}
        << Width - 1 << Start << Piece::ChangeStorage
        << originalPiece1B
        << QVector<Piece> {originalPieceB_, changePieceB};

    QTest::newRow("one-with-end-multi-next-noncontinuous")
        << QVector<Piece> {originalPieceB, changePiece__B}
        << Width - 1 << Start << Piece::ChangeStorage
        << originalPiece1B
        << QVector<Piece> {originalPieceB_, changePieceB1, changePiece__B};

    QTest::newRow("one-with-begin-multi-previous-continuous")
        << QVector<Piece> {originalPieceB_, changePieceB}
        << Width - 1 << End << Piece::OriginalStorage
        << changePieceB1
        << QVector<Piece> {originalPieceB, changePiece_B};

    QTest::newRow("one-with-begin-multi-previous-noncontinuous")
        << QVector<Piece> {originalPieceB__, changePieceB}
        << Width - 2 << End << Piece::OriginalStorage
        << changePieceB1
        << QVector<Piece> {originalPieceB__, originalPiece1B, changePiece_B};
}

void PieceTableTest::testReplaceOne()
{
    QFETCH(const QVector<Piece>, initPieces);
    QFETCH(const Address, replaceDataOffset);
    QFETCH(const Address, replaceStorageOffset);
    QFETCH(const Piece::StorageType, replaceStorageId);
    QFETCH(const Piece, expectedReplacedPiece);
    QFETCH(const QVector<Piece>, expectedPieces);

    PieceList initPieceList;
    for (const auto& piece : initPieces) {
        initPieceList.append(piece);
    }

    PieceTable pieceTable;
    pieceTable.insert(0, initPieceList);

    // tested action
    const Piece replacedPiece = pieceTable.replaceOne(replaceDataOffset, replaceStorageOffset, replaceStorageId);

    // check result
    QCOMPARE(replacedPiece, expectedReplacedPiece);

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
