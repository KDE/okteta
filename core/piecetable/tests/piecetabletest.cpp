/*
    SPDX-FileCopyrightText: 2008, 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetabletest.hpp"

// test object
#include <piecetable/piecetable.hpp>
// Qt
#include <QList>
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

Q_DECLARE_METATYPE(QList<KPieceTable::StorageDataTestData>)
Q_DECLARE_METATYPE(QList<KPieceTable::Piece>)
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

void compare(const PieceList& pieceList, const QList<Piece>& expectedPieces)
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

void compare(const PieceTable& pieceTable, const QList<Piece>& expectedPieces)
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
    const QList<Piece> expectedPieces {
            {0, BaseSize, Piece::OriginalStorage}
    };
    compare(pieceTable, expectedPieces);
}

void PieceTableTest::testInsertPieceListToEmpty_data()
{
    QTest::addColumn<QList<Piece>>("insertPieces");
    QTest::addColumn<QList<Piece>>("expectedPieces");

    QTest::newRow("empty")
        << QList<Piece> {}
        << QList<Piece> {};

    QTest::newRow("one")
        << QList<Piece> {
            {Start, Width, Piece::ChangeStorage}}
        << QList<Piece> {
            {Start, Width, Piece::ChangeStorage}};

    QTest::newRow("two")
        << QList<Piece> {
            {Start, Width, Piece::ChangeStorage},
            {0, Width, Piece::OriginalStorage}}
        << QList<Piece> {
            {Start, Width, Piece::ChangeStorage},
            {0, Width, Piece::OriginalStorage}};
}

void PieceTableTest::testInsertPieceListToEmpty()
{
    QFETCH(const QList<Piece>, insertPieces);
    QFETCH(const QList<Piece>, expectedPieces);

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
    QTest::addColumn<QList<Piece>>("initPieces");
    QTest::addColumn<Address>("insertDataOffset");
    QTest::addColumn<Size>("insertLength");
    QTest::addColumn<Address>("insertStorageOffset");
    QTest::addColumn<QList<Piece>>("expectedPieces");

    const Piece originalPieceA {0, BaseSize, Piece::OriginalStorage};
    const Piece originalPieceA1 {0, 1, Piece::OriginalStorage};
    const Piece originalPieceAM {0, HalfBaseSize, Piece::OriginalStorage};
    const Piece originalPieceA_ {0, BaseSize - 1, Piece::OriginalStorage};
    const Piece originalPieceMA {HalfBaseSize, HalfBaseSize, Piece::OriginalStorage};
    const Piece originalPiece1A {BaseSize - 1, 1, Piece::OriginalStorage};
    const Piece originalPiece_A {1, BaseSize - 1, Piece::OriginalStorage};
    const Piece originalPieceB {BaseSize, BaseSize, Piece::OriginalStorage};
    const Piece originalPieceB1 {BaseSize, 1, Piece::OriginalStorage};
    const Piece originalPiece_B {BaseSize + 1, BaseSize - 1, Piece::OriginalStorage};

    const Piece changePieceA {0, BaseSize, Piece::ChangeStorage};
    const Piece changePieceA1 {0, 1, Piece::ChangeStorage};
    const Piece changePieceA2 {1, 1, Piece::ChangeStorage};
    const Piece changePieceA12 {0, 2, Piece::ChangeStorage};
    const Piece changePieceA_ {0, BaseSize - 1, Piece::ChangeStorage};
    const Piece changePiece1A {BaseSize - 1, 1, Piece::ChangeStorage};
    const Piece changePiece_A {1, BaseSize - 1, Piece::ChangeStorage};
    const Piece changePiece__A {2, BaseSize - 2, Piece::ChangeStorage};
    const Piece changePieceB {BaseSize, BaseSize, Piece::ChangeStorage};
    const Piece changePieceB1 {BaseSize, 1, Piece::ChangeStorage};
    const Piece changePieceB2 {BaseSize + 1, 1, Piece::ChangeStorage};
    const Piece changePiece_B {BaseSize + 1, BaseSize - 1, Piece::ChangeStorage};
    const Piece changePieceAB {0, 2 * BaseSize, Piece::ChangeStorage};
    const Piece changePieceAB1 {0, BaseSize + 1, Piece::ChangeStorage};

    QTest::newRow("empty-to-empty")
        << QList<Piece> {}
        << 0 << 0 << 0
        << QList<Piece> {};
    QTest::newRow("one-to-empty")
        << QList<Piece> {}
        << 0 << changePieceB1.width() << changePieceB1.start()
        << QList<Piece> {changePieceB1};
    QTest::newRow("multi-to-empty")
        << QList<Piece> {}
        << 0 << changePieceB.width() << changePieceB.start()
        << QList<Piece> {changePieceB};

    QTest::newRow("empty-to-begin-one")
        << QList<Piece> {originalPieceA1}
        << 0 << 0 << 0
        << QList<Piece> {originalPieceA1};
    QTest::newRow("one-to-begin-one-noncontinuous-samestorage")
        << QList<Piece> {changePieceB2}
        << 0 << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {changePiece1A, changePieceB2};
    QTest::newRow("one-to-begin-one-noncontinuous-differentstorage")
        << QList<Piece> {originalPieceB1}
        << 0 << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {changePiece1A, originalPieceB1};
    QTest::newRow("one-to-begin-one-continuous")
        << QList<Piece> {changePieceA2}
        << 0 << changePieceA1.width() << changePieceA1.start()
        << QList<Piece> {changePieceA12};
    QTest::newRow("multi-to-begin-one-noncontinuous-samestorage")
        << QList<Piece> {changePieceB2}
        << 0 << changePieceA.width() << changePieceA.start()
        << QList<Piece> {changePieceA, changePieceB2};
    QTest::newRow("multi-to-begin-one-noncontinuous-differentstorage")
        << QList<Piece> {originalPieceB1}
        << 0 << changePieceA.width() << changePieceA.start()
        << QList<Piece> {changePieceA, originalPieceB1};
    QTest::newRow("multi-to-begin-one-continuous")
        << QList<Piece> {changePieceB1}
        << 0 << changePieceA.width() << changePieceA.start()
        << QList<Piece> {changePieceAB1};

    QTest::newRow("empty-to-end-one")
        << QList<Piece> {originalPieceA1}
        << originalPieceA1.width() << 0 << 0
        << QList<Piece> {originalPieceA1};
    QTest::newRow("one-to-end-one-noncontinuous-samestorage")
        << QList<Piece> {changePiece1A}
        << changePiece1A.width() << changePieceB2.width() << changePieceB2.start()
        << QList<Piece> {changePiece1A, changePieceB2};
    QTest::newRow("one-to-end-one-noncontinuous-differentstorage")
        << QList<Piece> {originalPiece1A}
        << originalPiece1A.width() << changePieceB1.width() << changePieceB1.start()
        << QList<Piece> {originalPiece1A, changePieceB1};
    QTest::newRow("one-to-end-one-continuous")
        << QList<Piece> {changePieceA1}
        << changePieceA1.width() << changePieceA2.width() << changePieceA2.start()
        << QList<Piece> {changePieceA12};
    QTest::newRow("multi-to-end-one-noncontinuous-samestorage")
        << QList<Piece> {changePiece1A}
        << changePiece1A.width() << changePiece_B.width() << changePiece_B.start()
        << QList<Piece> {changePiece1A, changePiece_B};
    QTest::newRow("multi-to-end-one-noncontinuous-differentstorage")
        << QList<Piece> {originalPiece1A}
        << originalPiece1A.width() << changePieceB.width() << changePieceB.start()
        << QList<Piece> {originalPiece1A, changePieceB};
    QTest::newRow("multi-to-end-one-continuous")
        << QList<Piece> {changePieceA1}
        << changePieceA1.width() << changePiece_A.width() << changePiece_A.start()
        << QList<Piece> {changePieceA};

    QTest::newRow("empty-to-begin-multi")
        << QList<Piece> {originalPieceA}
        << 0 << 0 << 0
        << QList<Piece> {originalPieceA};
    QTest::newRow("one-to-begin-multi-noncontinuous-samestorage")
        << QList<Piece> {changePiece_B}
        << 0 << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {changePiece1A, changePiece_B};
    QTest::newRow("one-to-begin-multi-noncontinuous-differentstorage")
        << QList<Piece> {originalPieceB}
        << 0 << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {changePiece1A, originalPieceB};
    QTest::newRow("one-to-begin-multi-continuous")
        << QList<Piece> {changePiece_A}
        << 0 << changePieceA1.width() << changePieceA1.start()
        << QList<Piece> {changePieceA};
    QTest::newRow("multi-to-begin-multi-noncontinuous-samestorage")
        << QList<Piece> {changePiece_B}
        << 0 << changePieceA.width() << changePieceA.start()
        << QList<Piece> {changePieceA, changePiece_B};
    QTest::newRow("multi-to-begin-multi-noncontinuous-differentstorage")
        << QList<Piece> {originalPieceB}
        << 0 << changePieceA.width() << changePieceA.start()
        << QList<Piece> {changePieceA, originalPieceB};
    QTest::newRow("multi-to-begin-multi-continuous")
        << QList<Piece> {changePieceB}
        << 0 << changePieceA.width() << changePieceA.start()
        << QList<Piece> {changePieceAB};

    QTest::newRow("empty-to-after-begin-multi")
        << QList<Piece> {originalPieceA}
        << 1 << 0 << 0
        << QList<Piece> {originalPieceA};
    QTest::newRow("one-to-after-begin-multi")
        << QList<Piece> {originalPieceA}
        << 1 << changePieceB1.width() << changePieceB1.start()
        << QList<Piece> {originalPieceA1, changePieceB1, originalPiece_A};
    QTest::newRow("multi-to-after-begin-multi")
        << QList<Piece> {originalPieceA}
        << 1 << changePieceB.width() << changePieceB.start()
        << QList<Piece> {originalPieceA1, changePieceB, originalPiece_A};

    QTest::newRow("emoty-to-middle-multi")
        << QList<Piece> {originalPieceA}
        << originalPieceAM.width() << 0 << 0
        << QList<Piece> {originalPieceA};
    QTest::newRow("one-to-middle-multi")
        << QList<Piece> {originalPieceA}
        << originalPieceAM.width() << changePieceB1.width() << changePieceB1.start()
        << QList<Piece> {originalPieceAM, changePieceB1, originalPieceMA};
    QTest::newRow("multi-to-middle-multi")
        << QList<Piece> {originalPieceA}
        << originalPieceAM.width() << changePieceB.width() << changePieceB.start()
        << QList<Piece> {originalPieceAM, changePieceB, originalPieceMA};

    QTest::newRow("empty-to-before-end-multi")
        << QList<Piece> {originalPieceA}
        << originalPieceA.width() - 1 << 0 << 0
        << QList<Piece> {originalPieceA};
    QTest::newRow("one-to-before-end-multi")
        << QList<Piece> {originalPieceA}
        << originalPieceA.width() - 1 << changePieceB1.width() << changePieceB1.start()
        << QList<Piece> {originalPieceA_, changePieceB1, originalPiece1A};
    QTest::newRow("multi-to-before-end-multi")
        << QList<Piece> {originalPieceA}
        << originalPieceA.width() - 1 << changePieceB.width() << changePieceB.start()
        << QList<Piece> {originalPieceA_, changePieceB, originalPiece1A};

    QTest::newRow("empty-to-end-multi")
        << QList<Piece> {originalPieceA}
        << originalPieceA.width() << 0 << 0
        << QList<Piece> {originalPieceA};
    QTest::newRow("one-to-end-multi-noncontinuous-samestorage")
        << QList<Piece> {changePieceA}
        << changePieceA.width() << changePieceB2.width() << changePieceB2.start()
        << QList<Piece> {changePieceA, changePieceB2};
    QTest::newRow("one-to-end-multi-noncontinuous-differentstorage")
        << QList<Piece> {originalPieceA}
        << originalPieceA.width() << changePieceB1.width() << changePieceB1.start()
        << QList<Piece> {originalPieceA, changePieceB1};
    QTest::newRow("one-to-end-multi-continuous")
        << QList<Piece> {changePieceA_}
        << changePieceA_.width() << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {changePieceA};
    QTest::newRow("multi-to-end-multi-noncontinuous-samestorage")
        << QList<Piece> {changePieceA}
        << changePieceA.width() << changePiece_B.width() << changePiece_B.start()
        << QList<Piece> {changePieceA, changePiece_B};
    QTest::newRow("multi-to-end-multi-noncontinuous-differentstorage")
        << QList<Piece> {originalPieceA}
        << originalPieceA.width() << changePieceB.width() << changePieceB.start()
        << QList<Piece> {originalPieceA, changePieceB};
    QTest::newRow("multi-to-end-multi-continuous")
        << QList<Piece> {changePieceA}
        << changePieceA.width() << changePieceB.width() << changePieceB.start()
        << QList<Piece> {changePieceAB};

    QTest::newRow("empty-to-begin-first-two-multi")
        << QList<Piece> {originalPieceB, originalPieceA}
        << 0 << 0 << 0
        << QList<Piece> {originalPieceB, originalPieceA};
    QTest::newRow("one-to-begin-first-two-multi-noncontinuous-samestorage")
        << QList<Piece> {changePiece_B, originalPieceB}
        << 0 << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {changePiece1A, changePiece_B, originalPieceB};
    QTest::newRow("one-to-begin-first-two-multi-noncontinuous-differentstorage")
        << QList<Piece> {originalPieceB, changePieceB}
        << 0 << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {changePiece1A, originalPieceB, changePieceB};
    QTest::newRow("one-to-begin-first-two-multi-continuous")
        << QList<Piece> {changePiece_A, originalPieceB}
        << 0 << changePieceA1.width() << changePieceA1.start()
        << QList<Piece> {changePieceA, originalPieceB};
    QTest::newRow("multi-to-begin-first-two-multi-noncontinuous-samestorage")
        << QList<Piece> {changePiece_B, originalPieceB}
        << 0 << changePieceA.width() << changePieceA.start()
        << QList<Piece> {changePieceA, changePiece_B, originalPieceB};
    QTest::newRow("multi-to-begin-first-two-multi-noncontinuous-differentstorage")
        << QList<Piece> {originalPieceB, changePieceB}
        << 0 << changePieceA.width() << changePieceA.start()
        << QList<Piece> {changePieceA, originalPieceB, changePieceB};
    QTest::newRow("multi-to-begin-first-two-multi-continuous")
        << QList<Piece> {changePieceB,originalPieceB}
        << 0 << changePieceA.width() << changePieceA.start()
        << QList<Piece> {changePieceAB, originalPieceB};

    QTest::newRow("empty-to-begin-second-two-multi")
        << QList<Piece> {originalPieceB, originalPieceA}
        << originalPieceB.width() << 0 << 0
        << QList<Piece> {originalPieceB, originalPieceA};
    QTest::newRow("one-to-begin-second-two-multi-noncontinuous-samestorage")
        << QList<Piece> {originalPieceB, changePiece_B}
        << originalPieceB.width() << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {originalPieceB, changePiece1A, changePiece_B};
    QTest::newRow("one-to-begin-second-two-multi-noncontinuous-differentstorage")
        << QList<Piece> {changePieceB, originalPieceB}
        << changePieceB.width() << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {changePieceB, changePiece1A, originalPieceB};
    QTest::newRow("one-to-begin-second-two-multi-continuous-first")
        << QList<Piece> {changePieceA_, originalPieceB}
        << changePieceA_.width() << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {changePieceA, originalPieceB};
    QTest::newRow("one-to-begin-second-two-multi-continuous-second")
        << QList<Piece> {originalPieceB, changePiece_A}
        << originalPieceB.width() << changePieceA1.width() << changePieceA1.start()
        << QList<Piece> {originalPieceB, changePieceA};
    QTest::newRow("one-to-begin-second-two-multi-continuous-all")
        << QList<Piece> {changePieceA_, changePieceB}
        << changePieceA_.width() << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {changePieceAB};
    QTest::newRow("multi-to-begin-second-two-multi-noncontinuous-samestorage")
        << QList<Piece> {originalPieceB, changePiece_B}
        << originalPieceB.width() << changePieceA.width() << changePieceA.start()
        << QList<Piece> {originalPieceB, changePieceA, changePiece_B};
    QTest::newRow("multi-to-begin-second-two-multi-noncontinuous-differentstorage")
        << QList<Piece> {changePieceB, originalPieceB}
        << changePieceB.width() << changePieceA.width() << changePieceA.start()
        << QList<Piece> {changePieceB, changePieceA, originalPieceB};
    QTest::newRow("multi-to-begin-second-two-multi-continuous-first")
        << QList<Piece> {changePieceA, originalPieceB}
        << changePieceA.width() << changePieceB.width() << changePieceB.start()
        << QList<Piece> {changePieceAB, originalPieceB};
    QTest::newRow("multi-to-begin-second-two-multi-continuous-second")
        << QList<Piece> {originalPieceB, changePieceB}
        << originalPieceB.width() << changePieceA.width() << changePieceA.start()
        << QList<Piece> {originalPieceB, changePieceAB};
    QTest::newRow("multi-to-begin-second-two-multi-continuous-all")
        << QList<Piece> {changePieceA12, changePieceB}
        << changePieceA12.width() << changePiece__A.width() << changePiece__A.start()
        << QList<Piece> {changePieceAB};

    QTest::newRow("empty-to-end-second-two-multi")
        << QList<Piece> {originalPieceB, originalPieceA}
        << originalPieceB.width() + originalPieceA.width() << 0 << 0
        << QList<Piece> {originalPieceB, originalPieceA};
    QTest::newRow("one-to-end-second-two-multi-noncontinuous-samestorage")
        << QList<Piece> {originalPieceB, changePieceA}
        << originalPieceB.width() + changePieceA.width() << changePieceB2.width() << changePieceB2.start()
        << QList<Piece> {originalPieceB, changePieceA, changePieceB2};
    QTest::newRow("one-to-end-second-two-multi-noncontinuous-differentstorage")
        << QList<Piece> {originalPieceB, originalPieceA}
        << originalPieceB.width() + originalPieceA.width() << changePieceB1.width() << changePieceB1.start()
        << QList<Piece> {originalPieceB, originalPieceA, changePieceB1};
    QTest::newRow("one-to-end-second-two-multi-continuous")
        << QList<Piece> {originalPieceB, changePieceA_}
        << originalPieceB.width() + changePieceA_.width() << changePiece1A.width() << changePiece1A.start()
        << QList<Piece> {originalPieceB, changePieceA};
    QTest::newRow("multi-to-end-second-two-multi-noncontinuous-samestorage")
        << QList<Piece> {originalPieceB, changePieceA}
        << originalPieceB.width() + changePieceA.width() << changePiece_B.width() << changePiece_B.start()
        << QList<Piece> {originalPieceB, changePieceA, changePiece_B};
    QTest::newRow("multi-to-end-second-two-multi-noncontinuous-differentstorage")
        << QList<Piece> {originalPieceB, originalPieceA}
        << originalPieceB.width() + originalPieceA.width() << changePieceB.width() << changePieceB.start()
        << QList<Piece> {originalPieceB, originalPieceA, changePieceB};
    QTest::newRow("multi-to-end-second-two-multi-continuous")
        << QList<Piece> {originalPieceB, changePieceA}
        << originalPieceB.width() + changePieceA.width() << changePieceB.width() << changePieceB.start()
        << QList<Piece> {originalPieceB, changePieceAB};
}

void PieceTableTest::testInsert()
{
    QFETCH(const QList<Piece>, initPieces);
    QFETCH(const Address, insertDataOffset);
    QFETCH(const Size, insertLength);
    QFETCH(const Address, insertStorageOffset);
    QFETCH(const QList<Piece>, expectedPieces);

    PieceList initPieceList;
    for (const auto& piece : initPieces) {
        initPieceList.append(piece);
    }

    PieceTable pieceTable;
    pieceTable.insert(0, initPieceList);

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

    const QList<Piece> expectedPieces {
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

void PieceTableTest::testInsertPieceList_data()
{
    QTest::addColumn<QList<Piece>>("initPieces");
    QTest::addColumn<Address>("insertDataOffset");
    QTest::addColumn<QList<Piece>>("insertPieces");
    QTest::addColumn<QList<Piece>>("expectedPieces");

    const QList<Piece> noPieces;

    const Piece changePieceA {0, Start, Piece::ChangeStorage};
    const Piece changePieceA_ {0, Start - 1, Piece::ChangeStorage};
    const Piece changePieceB {Start, Width, Piece::ChangeStorage};
    const Piece changePieceB1 {Start, 1, Piece::ChangeStorage};
    const Piece changePieceB_ {Start, Width - 1, Piece::ChangeStorage};
    const Piece changePieceBM {Start, Width / 2, Piece::ChangeStorage};
    const Piece changePieceMB {Start + Width / 2, Width / 2 + 1, Piece::ChangeStorage};
    const Piece changePiece_B {Start + 1, Width - 1, Piece::ChangeStorage};
    const Piece changePiece1B {End, 1, Piece::ChangeStorage};
    const Piece changePieceC {End + 1, Width, Piece::ChangeStorage};
    const Piece changePiece_C {End + 2, Width - 1, Piece::ChangeStorage};
    const Piece changePieceAB {0, Width + Start, Piece::ChangeStorage};
    const Piece changePieceBC {Start, Width + Width, Piece::ChangeStorage};
    const Piece changePieceABC {0, Start + Width + Width, Piece::ChangeStorage};

    const Piece originalPieceA {0, Start, Piece::OriginalStorage};
    const Piece originalPieceA_ {0, Start - 1, Piece::OriginalStorage};
    const Piece originalPieceB {Start, Width, Piece::OriginalStorage};
    const Piece originalPieceC {End + 1, Width, Piece::OriginalStorage};
    const Piece originalPiece_C {End + 2, Width - 1, Piece::OriginalStorage};
    const Piece originalPieceBC {Start, Width + Width, Piece::OriginalStorage};

    QTest::newRow("empty-to-empty")
        << noPieces
        << 0
        << noPieces
        << noPieces;

    QTest::newRow("one-to-empty")
        << noPieces
        << 0
        << QList<Piece> {changePieceB}
        << QList<Piece> {changePieceB};

    QTest::newRow("three-to-empty")
        << noPieces
        << 0
        << QList<Piece> {changePieceB, originalPieceB, originalPieceA}
        << QList<Piece> {changePieceB, originalPieceB, originalPieceA};

    QTest::newRow("empty-to-one-begin")
        << QList<Piece> {changePieceB}
        << 0
        << noPieces
        << QList<Piece> {changePieceB};

    QTest::newRow("one-to-one-begin-noncontinuous-samestorage")
        << QList<Piece> {changePieceB}
        << 0
        << QList<Piece> {changePieceA_}
        << QList<Piece> {changePieceA_, changePieceB};

    QTest::newRow("one-to-one-begin-noncontinuous-differentstorage")
        << QList<Piece> {changePieceB}
        << 0
        << QList<Piece> {originalPieceA_}
        << QList<Piece> {originalPieceA_, changePieceB};

    QTest::newRow("one-to-one-begin-continuous-samestorage")
        << QList<Piece> {changePieceB}
        << 0
        << QList<Piece> {changePieceA}
        << QList<Piece> {changePieceAB};

    QTest::newRow("one-to-one-begin-continuous-differentstorage")
        << QList<Piece> {changePieceB}
        << 0
        << QList<Piece> {originalPieceA}
        << QList<Piece> {originalPieceA, changePieceB};

    QTest::newRow("two-to-one-begin-noncontinuous-samestorage")
        << QList<Piece> {changePieceB}
        << 0
        << QList<Piece> {originalPieceB, changePieceA_}
        << QList<Piece> {originalPieceB, changePieceA_, changePieceB};

    QTest::newRow("two-to-one-begin-continuous-samestorage")
        << QList<Piece> {changePieceB}
        << 0
        << QList<Piece> {originalPieceB, changePieceA}
        << QList<Piece> {originalPieceB, changePieceAB};

    QTest::newRow("empty-to-one-after-begin")
        << QList<Piece> {changePieceB}
        << 1
        << noPieces
        << QList<Piece> {changePieceB};

    QTest::newRow("one-to-one-after-begin-noncontinuous-samestorage")
        << QList<Piece> {changePieceB}
        << 1
        << QList<Piece> {changePieceA_}
        << QList<Piece> {changePieceB1, changePieceA_, changePiece_B};

    QTest::newRow("two-to-one-after-begin")
        << QList<Piece> {changePieceB}
        << 1
        << QList<Piece> {originalPieceB, changePieceA}
        << QList<Piece> {changePieceB1, originalPieceB, changePieceA, changePiece_B};

    QTest::newRow("empty-to-one-middle")
        << QList<Piece> {changePieceB}
        << changePieceB.width() / 2
        << noPieces
        << QList<Piece> {changePieceB};

    QTest::newRow("one-to-one-middle-noncontinuous-samestorage")
        << QList<Piece> {changePieceB}
        << changePieceB.width() / 2
        << QList<Piece> {changePieceA_}
        << QList<Piece> {changePieceBM, changePieceA_, changePieceMB};

    QTest::newRow("one-to-one-middle-noncontinuous-differentstorage")
        << QList<Piece> {changePieceB}
        << changePieceB.width() / 2
        << QList<Piece> {originalPieceA_}
        << QList<Piece> {changePieceBM, originalPieceA_, changePieceMB};

    QTest::newRow("two-to-one-middle")
        << QList<Piece> {changePieceB}
        << changePieceB.width() / 2
        << QList<Piece> {changePieceA_, originalPieceB}
        << QList<Piece> {changePieceBM, changePieceA_, originalPieceB, changePieceMB};

    QTest::newRow("three-to-one-middle")
        << QList<Piece> {changePieceB}
        << changePieceB.width() / 2
        << QList<Piece> {changePieceA_, originalPieceB, originalPieceA_}
        << QList<Piece> {changePieceBM, changePieceA_, originalPieceB, originalPieceA_, changePieceMB};

    QTest::newRow("empty-to-one-before-end")
        << QList<Piece> {changePieceB}
        << changePieceB.width() - 1
        << noPieces
        << QList<Piece> {changePieceB};

    QTest::newRow("one-to-one-before-end-noncontinuous-samestorage")
        << QList<Piece> {changePieceB}
        << changePieceB.width() - 1
        << QList<Piece> {changePiece_C}
        << QList<Piece> {changePieceB_, changePiece_C, changePiece1B};

    QTest::newRow("one-to-one-before-end-continuous-samestorage")
        << QList<Piece> {changePieceB}
        << changePieceB.width() - 1
        << QList<Piece> {changePieceC}
        << QList<Piece> {changePieceB_, changePieceC, changePiece1B};

    QTest::newRow("two-to-one-before-end")
        << QList<Piece> {changePieceB}
        << changePieceB.width() - 1
        << QList<Piece> {originalPieceB, changePieceC}
        << QList<Piece> {changePieceB_, originalPieceB, changePieceC, changePiece1B};

    QTest::newRow("empty-to-one-end")
        << QList<Piece> {changePieceB}
        << changePieceB.width() - 1
        << noPieces
        << QList<Piece> {changePieceB};

    QTest::newRow("one-to-one-end-noncontinuous-samestorage")
        << QList<Piece> {changePieceB}
        << changePieceB.width()
        << QList<Piece> {changePiece_C}
        << QList<Piece> {changePieceB, changePiece_C};

    QTest::newRow("one-to-one-end-noncontinuous-differentstorage")
        << QList<Piece> {changePieceB}
        << changePieceB.width()
        << QList<Piece> {originalPiece_C}
        << QList<Piece> {changePieceB, originalPiece_C};

    QTest::newRow("one-to-one-end-continuous-samestorage")
        << QList<Piece> {changePieceB}
        << changePieceB.width()
        << QList<Piece> {changePieceC}
        << QList<Piece> {changePieceBC};

    QTest::newRow("one-to-one-end-continuous-differentstorage")
        << QList<Piece> {changePieceB}
        << changePieceB.width()
        << QList<Piece> {originalPieceC}
        << QList<Piece> {changePieceB, originalPieceC};

    QTest::newRow("two-to-one-end-continuous-samestorage")
        << QList<Piece> {changePieceB}
        << changePieceB.width()
        << QList<Piece> {changePieceC, originalPieceB}
        << QList<Piece> {changePieceBC, originalPieceB};

    QTest::newRow("empty-to-two-begin")
        << QList<Piece> {changePieceA, originalPieceB}
        << 0
        << noPieces
        << QList<Piece> {changePieceA, originalPieceB};

    QTest::newRow("one-to-two-begin-noncontinuous-samestorage")
        << QList<Piece> {changePieceB, originalPieceB}
        << 0
        << QList<Piece> {changePieceA_}
        << QList<Piece> {changePieceA_, changePieceB, originalPieceB};

    QTest::newRow("one-to-two-begin-noncontinuous-differentstorage")
        << QList<Piece> {changePieceB, originalPieceB}
        << 0
        << QList<Piece> {originalPieceA_}
        << QList<Piece> {originalPieceA_, changePieceB, originalPieceB};

    QTest::newRow("one-to-two-begin-continuous-samestorage")
        << QList<Piece> {changePieceB, originalPieceB}
        << 0
        << QList<Piece> {changePieceA}
        << QList<Piece> {changePieceAB, originalPieceB};

    QTest::newRow("one-to-two-begin-continuous-differentstorage")
        << QList<Piece> {changePieceB, originalPieceB}
        << 0
        << QList<Piece> {originalPieceA}
        << QList<Piece> {originalPieceA, changePieceB, originalPieceB};

    QTest::newRow("two-to-two-begin-noncontinuous-samestorage")
        << QList<Piece> {changePieceB, originalPieceC}
        << 0
        << QList<Piece> {originalPieceB, changePieceA_}
        << QList<Piece> {originalPieceB, changePieceA_, changePieceB, originalPieceC};

    QTest::newRow("two-to-two-begin-continuous-samestorage")
        << QList<Piece> {changePieceB, originalPieceC}
        << 0
        << QList<Piece> {originalPieceB, changePieceA}
        << QList<Piece> {originalPieceB, changePieceAB, originalPieceC};

    QTest::newRow("empty-to-two-after-begin")
        << QList<Piece> {changePieceB, originalPieceC}
        << 1
        << noPieces
        << QList<Piece> {changePieceB, originalPieceC};

    QTest::newRow("one-to-two-after-begin-noncontinuous-samestorage")
        << QList<Piece> {changePieceB, originalPieceC}
        << 1
        << QList<Piece> {changePieceA_}
        << QList<Piece> {changePieceB1, changePieceA_, changePiece_B, originalPieceC};

    QTest::newRow("two-to-two-after-begin")
        << QList<Piece> {changePieceB, originalPieceC}
        << 1
        << QList<Piece> {originalPieceB, changePieceA}
        << QList<Piece> {changePieceB1, originalPieceB, changePieceA, changePiece_B, originalPieceC};

    QTest::newRow("one-to-two-middle-continuous-samestorage")
        << QList<Piece> {changePieceA, changePieceC}
        << Start
        << QList<Piece> {changePieceB}
        << QList<Piece> {changePieceABC};

    QTest::newRow("one-to-two-middle-continuous-differentstorage")
        << QList<Piece> {changePieceA, changePieceC}
        << changePieceA.width()
        << QList<Piece> {originalPieceB}
        << QList<Piece> {changePieceA, originalPieceB, changePieceC};

    QTest::newRow("one-to-two-middle-first-continuous-samestorage")
        << QList<Piece> {changePieceA, changePiece_C}
        << changePieceA.width()
        << QList<Piece> {changePieceB}
        << QList<Piece> {changePieceAB, changePiece_C};

    QTest::newRow("one-to-two-middle-second-continuous-samestorage")
        << QList<Piece> {changePieceA_, changePieceC}
        << changePieceA_.width()
        << QList<Piece> {changePieceB}
        << QList<Piece> {changePieceA_, changePieceBC};

    QTest::newRow("two-to-two-middle-continuous-samestorage")
        << QList<Piece> {changePieceA, originalPieceC}
        << changePieceA.width()
        << QList<Piece> {changePieceB, originalPieceB}
        << QList<Piece> {changePieceAB, originalPieceBC};

    QTest::newRow("two-to-two-middle-continuous-differentstorage")
        << QList<Piece> {changePieceA, originalPieceC}
        << changePieceA.width()
        << QList<Piece> {originalPieceB, changePieceB}
        << QList<Piece> {changePieceA, originalPieceB, changePieceB, originalPieceC};

    QTest::newRow("three-to-two-middle-continuous-samestorage")
        << QList<Piece> {changePieceA, originalPieceC}
        << changePieceA.width()
        << QList<Piece> {changePieceB, originalPieceA_, originalPieceB}
        << QList<Piece> {changePieceAB, originalPieceA_, originalPieceBC};
}

void PieceTableTest::testInsertPieceList()
{
    QFETCH(const QList<Piece>, initPieces);
    QFETCH(const Address, insertDataOffset);
    QFETCH(const QList<Piece>, insertPieces);
    QFETCH(const QList<Piece>, expectedPieces);

    PieceList initPieceList;
    for (const auto& piece : initPieces) {
        initPieceList.append(piece);
    }

    PieceList insertPieceList;
    for (const auto& piece : insertPieces) {
        insertPieceList.append(piece);
    }

    PieceTable pieceTable;
    pieceTable.insert(0, initPieceList);

    // tested action
    pieceTable.insert(insertDataOffset, insertPieceList);

    // check result
    compare(pieceTable, expectedPieces);
}

void PieceTableTest::testRemove_data()
{
    QTest::addColumn<QList<Piece>>("initPieces");
    QTest::addColumn<AddressRange>("removeRange");
    QTest::addColumn<QList<Piece>>("expectedRemovedPieces");
    QTest::addColumn<QList<Piece>>("expectedPieces");

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
        << QList<Piece> {originalPieceA}
        << removeRange
        << QList<Piece> {originalPieceA1}
        << QList<Piece> {originalPiece_A};
    removeRange = AddressRange(0, 1);
    QTest::newRow("multi-at-begin-one-multi")
        << QList<Piece> {originalPieceA}
        << removeRange
        << QList<Piece> {originalPieceA12}
        << QList<Piece> {originalPiece__A};
    removeRange = AddressRange::fromWidth(BaseSize / 2, 1);
    QTest::newRow("one-at-middle-one-multi")
        << QList<Piece> {originalPieceA}
        << removeRange
        << QList<Piece> {originalPiece1MA}
        << QList<Piece> {originalPieceAM, originalPiece_MA};
    removeRange = AddressRange::fromWidth(BaseSize / 2, 2);
    QTest::newRow("multi-at-middle-one-multi")
        << QList<Piece> {originalPieceA}
        << removeRange
        << QList<Piece> {originalPiece12MA}
        << QList<Piece> {originalPieceAM, originalPiece__MA};
    removeRange = AddressRange(BaseSize - 1, BaseSize - 1);
    QTest::newRow("one-at-end-one-multi")
        << QList<Piece> {originalPieceA}
        << removeRange
        << QList<Piece> {originalPiece1A}
        << QList<Piece> {originalPieceA_};
    removeRange = AddressRange(BaseSize - 2, BaseSize - 1);
    QTest::newRow("multi-at-end-one-multi")
        << QList<Piece> {originalPieceA}
        << removeRange
        << QList<Piece> {originalPiece12A}
        << QList<Piece> {originalPieceA__};
    removeRange = AddressRange(0, 0);
    QTest::newRow("all-one-one")
        << QList<Piece> {originalPieceA1}
        << removeRange
        << QList<Piece> {originalPieceA1}
        << QList<Piece> {};
    removeRange = AddressRange::fromWidth(BaseSize);
    QTest::newRow("all-one-multi")
        << QList<Piece> {originalPieceA}
        << removeRange
        << QList<Piece> {originalPieceA}
        << QList<Piece>();

    removeRange = AddressRange(0, 0);
    QTest::newRow("one-at-begin-first-two-multi")
        << QList<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA1}
        << QList<Piece> {originalPiece_A, changePieceA};
    removeRange = AddressRange(0, 1);
    QTest::newRow("multi-at-begin-first-two-multi")
        << QList<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA12}
        << QList<Piece> {originalPiece__A, changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize / 2, 1);
    QTest::newRow("one-at-middle-first-two-multi")
        << QList<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QList<Piece> {originalPiece1MA}
        << QList<Piece> {originalPieceAM, originalPiece_MA, changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize / 2, 2);
    QTest::newRow("multi-at-middle-first-two-multi")
        << QList<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QList<Piece> {originalPiece12MA}
        << QList<Piece> {originalPieceAM, originalPiece__MA, changePieceA};
    removeRange = AddressRange(BaseSize - 1, BaseSize - 1);
    QTest::newRow("one-at-end-first-two-multi-noncontinuous")
        << QList<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QList<Piece> {originalPiece1A}
        << QList<Piece> {originalPieceA_, changePieceA};
    removeRange = AddressRange(BaseSize - 2, BaseSize - 1);
    QTest::newRow("multi-at-end-first-two-multi-noncontinuous")
        << QList<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QList<Piece> {originalPiece12A}
        << QList<Piece> {originalPieceA__, changePieceA};
    removeRange = AddressRange(0, 0);
    QTest::newRow("all-first-two-one")
        << QList<Piece> {originalPieceA1, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA1}
        << QList<Piece> {changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize);
    QTest::newRow("all-first-two-multi")
        << QList<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA}
        << QList<Piece> {changePieceA};
    removeRange = AddressRange::fromWidth(0, BaseSize + 1);
    QTest::newRow("all-first-one-at-begin-second-two-multi")
        << QList<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA, changePieceA1}
        << QList<Piece> {changePiece_A};
    removeRange = AddressRange::fromWidth(0, BaseSize + 2);
    QTest::newRow("all-first-multi-at-begin-second-two-multi")
        << QList<Piece> {originalPieceA, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA, changePieceA12}
        << QList<Piece> {changePiece__A};

    removeRange = AddressRange::fromWidth(BaseSize, 1);
    QTest::newRow("one-at-begin-second-two-multi-noncontinuous")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {originalPieceA1}
        << QList<Piece> {changePieceA, originalPiece_A};
    removeRange = AddressRange::fromWidth(BaseSize, 2);
    QTest::newRow("multi-at-begin-second-two-multi-noncontinuous")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {originalPieceA12}
        << QList<Piece> {changePieceA, originalPiece__A};
    removeRange = AddressRange::fromWidth(BaseSize + BaseSize / 2, 1);
    QTest::newRow("one-at-middle-second-two-multi")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {originalPiece1MA}
        << QList<Piece> {changePieceA, originalPieceAM, originalPiece_MA};
    removeRange = AddressRange::fromWidth(BaseSize + BaseSize / 2, 2);
    QTest::newRow("multi-at-middle-second-two-multi")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {originalPiece12MA}
        << QList<Piece> {changePieceA, originalPieceAM, originalPiece__MA};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 1, 1);
    QTest::newRow("one-at-end-second-two-multi")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {originalPiece1A}
        << QList<Piece> {changePieceA, originalPieceA_};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 2, 2);
    QTest::newRow("multi-at-end-second-two-multi")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {originalPiece12A}
        << QList<Piece> {changePieceA, originalPieceA__};
    removeRange = AddressRange::fromWidth(BaseSize, 1);
    QTest::newRow("all-second-two-one")
        << QList<Piece> {changePieceA, originalPieceA1}
        << removeRange
        << QList<Piece> {originalPieceA1}
        << QList<Piece> {changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize, BaseSize);
    QTest::newRow("all-second-two-multi")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {originalPieceA}
        << QList<Piece> {changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize - 1, BaseSize + 1);
    QTest::newRow("all-second-one-at-end-first-two-multi")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {changePiece1A, originalPieceA}
        << QList<Piece> {changePieceA_};
    removeRange = AddressRange::fromWidth(BaseSize - 2, BaseSize + 2);
    QTest::newRow("all-second-multi-at-end-first-two-multi")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {changePiece12A, originalPieceA}
        << QList<Piece> {changePieceA__};

    removeRange = AddressRange::fromWidth(BaseSize - 1, 2);
    QTest::newRow("one-at-end-first-one-at-begin-second-two-multi")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {changePiece1A, originalPieceA1}
        << QList<Piece> {changePieceA_, originalPiece_A};
    removeRange = AddressRange::fromWidth(BaseSize - 2, 4);
    QTest::newRow("multi-at-end-first-multi-at-begin-second-two-multi")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {changePiece12A, originalPieceA12}
        << QList<Piece> {changePieceA__, originalPiece__A};
    removeRange = AddressRange::fromWidth(0, 2);
    QTest::newRow("all-two-one")
        << QList<Piece> {changePieceA1, originalPieceA1}
        << removeRange
        << QList<Piece> {changePieceA1, originalPieceA1}
        << QList<Piece> {};
    removeRange = AddressRange::fromWidth(0, 2 * BaseSize);
    QTest::newRow("all-two-multi")
        << QList<Piece> {changePieceA, originalPieceA}
        << removeRange
        << QList<Piece> {changePieceA, originalPieceA}
        << QList<Piece> {};

    removeRange = AddressRange::fromWidth(2 * BaseSize, 1);
    QTest::newRow("one-at-begin-third-five-multi-noncontinuous")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA1}
        << QList<Piece> {changePieceE, changePieceD, originalPiece_A, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize, 2);
    QTest::newRow("multi-at-begin-third-five-multi-noncontinuous")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA12}
        << QList<Piece> {changePieceE, changePieceD, originalPiece__A, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize + BaseSize / 2, 1);
    QTest::newRow("one-at-middle-third-five-multi")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {originalPiece1MA}
        << QList<Piece> {changePieceE, changePieceD, originalPieceAM, originalPiece_MA, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize + BaseSize / 2, 2);
    QTest::newRow("multi-at-middle-third-five-multi")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {originalPiece12MA}
        << QList<Piece> {changePieceE, changePieceD, originalPieceAM, originalPiece__MA, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(3 * BaseSize - 1, 1);
    QTest::newRow("one-at-end-third-five-multi")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {originalPiece1A}
        << QList<Piece> {changePieceE, changePieceD, originalPieceA_, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(3 * BaseSize - 2, 2);
    QTest::newRow("multi-at-end-third-five-multi")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {originalPiece12A}
        << QList<Piece> {changePieceE, changePieceD, originalPieceA__, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize, 1);
    QTest::newRow("all-third-five-one-noncontinuous")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA1, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA1}
        << QList<Piece> {changePieceE, changePieceD, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize, BaseSize);
    QTest::newRow("all-third-five-multi-noncontinuous")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA}
        << QList<Piece> {changePieceE, changePieceD, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize, 1);
    QTest::newRow("all-third-five-one-continuous")
        << QList<Piece> {changePieceE, changePieceB, originalPieceA1, changePieceC, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA1}
        << QList<Piece> {changePieceE, changePieceBC, changePieceA};
    removeRange = AddressRange::fromWidth(2 * BaseSize, BaseSize);
    QTest::newRow("all-third-five-multi-noncontinuous")
        << QList<Piece> {changePieceE, changePieceB, originalPieceA, changePieceC, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA}
        << QList<Piece> {changePieceE, changePieceBC, changePieceA};

    removeRange = AddressRange::fromWidth(2 * BaseSize, BaseSize + 1);
    QTest::newRow("all-third-one-at-begin-fourth-five-multi")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceA, changePieceC}
        << removeRange
        << QList<Piece> {originalPieceA, changePieceA1}
        << QList<Piece> {changePieceE, changePieceD, changePiece_A, changePieceC};
    removeRange = AddressRange::fromWidth(2 * BaseSize, BaseSize + 2);
    QTest::newRow("all-third-multi-at-begin-fourth-five-multi")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceA, changePieceC}
        << removeRange
        << QList<Piece> {originalPieceA, changePieceA12}
        << QList<Piece> {changePieceE, changePieceD, changePiece__A, changePieceC};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 1, BaseSize + 1);
    QTest::newRow("all-third-one-at-end-second-five-multi")
        << QList<Piece> {changePieceE, changePieceA, originalPieceA, changePieceD, changePieceC}
        << removeRange
        << QList<Piece> {changePiece1A, originalPieceA}
        << QList<Piece> {changePieceE, changePieceA_, changePieceD, changePieceC};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 2, BaseSize + 2);
    QTest::newRow("all-third-multi-at-end-second-five-multi")
        << QList<Piece> {changePieceE, changePieceA, originalPieceA, changePieceD, changePieceC}
        << removeRange
        << QList<Piece> {changePiece12A, originalPieceA}
        << QList<Piece> {changePieceE, changePieceA__, changePieceD, changePieceC};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 1, BaseSize + 2);
    QTest::newRow("all-third-one-at-end-second-one-at-begin-fourth-five-multi")
        << QList<Piece> {changePieceE, changePieceA, changePieceD, originalPieceA, changePieceC}
        << removeRange
        << QList<Piece> {changePiece1A, changePieceD, originalPieceA1}
        << QList<Piece> {changePieceE, changePieceA_, originalPiece_A, changePieceC};
    removeRange = AddressRange::fromWidth(2 * BaseSize - 2, BaseSize + 4);
    QTest::newRow("all-third-multi-at-end-second-multi-at-begin-fourth-five-multi")
        << QList<Piece> {changePieceE, changePieceA, changePieceD, originalPieceA, changePieceC}
        << removeRange
        << QList<Piece> {changePiece12A, changePieceD, originalPieceA12}
        << QList<Piece> {changePieceE, changePieceA__, originalPiece__A, changePieceC};
    removeRange = AddressRange::fromWidth(BaseSize, 3 * BaseSize);
    QTest::newRow("all-second-third-fourth-five-multi-noncontinuous")
        << QList<Piece> {changePieceE, changePieceA, changePieceD, originalPieceA, changePieceC}
        << removeRange
        << QList<Piece> {changePieceA, changePieceD, originalPieceA}
        << QList<Piece> {changePieceE, changePieceC};
    removeRange = AddressRange::fromWidth(BaseSize, 3 * BaseSize);
    QTest::newRow("all-second-third-fourth-five-multi-continuous")
        << QList<Piece> {changePieceB, changePieceA, changePieceD, originalPieceA, changePieceC}
        << removeRange
        << QList<Piece> {changePieceA, changePieceD, originalPieceA}
        << QList<Piece> {changePieceBC};

    removeRange = AddressRange::fromWidth(0, 2);
    QTest::newRow("multi-at-begin-first-five-multi")
        << QList<Piece> {originalPieceA, changePieceD, changePieceC, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {originalPieceA12}
        << QList<Piece> {originalPiece__A, changePieceD, changePieceC, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(BaseSize);
    QTest::newRow("all-first-five-multi")
        << QList<Piece> {changePieceE, changePieceD, changePieceC, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {changePieceE}
        << QList<Piece> {changePieceD, changePieceC, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(0, BaseSize + 2);
    QTest::newRow("all-first-multi-at-begin-second-five-multi")
        << QList<Piece> {changePieceD, originalPieceA, changePieceC, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {changePieceD, originalPieceA12}
        << QList<Piece> {originalPiece__A, changePieceC, changePieceB, changePieceA};
    removeRange = AddressRange::fromWidth(5 * BaseSize - 2, 2);
    QTest::newRow("multi-at-end-last-five-multi")
        << QList<Piece> {changePieceE, changePieceD, changePieceC, changePieceB, originalPieceA}
        << removeRange
        << QList<Piece> {originalPiece12A}
        << QList<Piece> {changePieceE, changePieceD, changePieceC, changePieceB, originalPieceA__};
    removeRange = AddressRange::fromWidth(4 * BaseSize, BaseSize);
    QTest::newRow("all-last-five-multi")
        << QList<Piece> {changePieceE, changePieceD, changePieceC, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {changePieceA}
        << QList<Piece> {changePieceE, changePieceD, changePieceC, changePieceB};
    removeRange = AddressRange::fromWidth(4 * BaseSize - 2, BaseSize + 2);
    QTest::newRow("all-last-multi-at-end-fourth-five-multi")
        << QList<Piece> {changePieceE, changePieceD, changePieceC, originalPieceA, changePieceA}
        << removeRange
        << QList<Piece> {originalPiece12A, changePieceA}
        << QList<Piece> {changePieceE, changePieceD, changePieceC, originalPieceA__};

    removeRange = AddressRange::fromWidth(0, 5 * BaseSize);
    QTest::newRow("all-five-multi")
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << removeRange
        << QList<Piece> {changePieceE, changePieceD, originalPieceA, changePieceB, changePieceA}
        << QList<Piece> {};
}

void PieceTableTest::testRemove()
{
    QFETCH(const QList<Piece>, initPieces);
    QFETCH(const AddressRange, removeRange);
    QFETCH(const QList<Piece>, expectedRemovedPieces);
    QFETCH(const QList<Piece>, expectedPieces);

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
    QTest::addColumn<QList<Piece>>("expectedPieces");

    AddressRange moveRange = AddressRange::fromWidth(End + 1, BaseSize - (End + 1));
    QTest::newRow("moving-end-at-begin")
        << 0 << moveRange
        << QList<Piece> {
            {moveRange, Piece::OriginalStorage},
            {0, moveRange.start(), Piece::OriginalStorage}};
    moveRange = AddressRange::fromWidth(End + 1, BaseSize - (End + 1));
    QTest::newRow("moving-end-at-mid")
        << Start << moveRange
        << QList<Piece> {
            {0, Start, Piece::OriginalStorage},
            {moveRange, Piece::OriginalStorage},
            {Start, Width, Piece::OriginalStorage}};
    moveRange = AddressRange::fromWidth(Start, Width);
    QTest::newRow("moving-mid-at-begin")
        << 0 << moveRange
        << QList<Piece> {
            {moveRange, Piece::OriginalStorage},
            {0, moveRange.start(), Piece::OriginalStorage},
            {moveRange.nextBehindEnd(), BaseSize - moveRange.nextBehindEnd(), Piece::OriginalStorage}};
    const Address mid = (End + Start) / 2;
    moveRange = AddressRange::fromWidth(mid, End - mid + 1);
    QTest::newRow("moving-mid-at-mid")
        << Start << moveRange
        << QList<Piece> {
            {0, Start, Piece::OriginalStorage},
            {moveRange, Piece::OriginalStorage},
            {Start,  Width - moveRange.width(), Piece::OriginalStorage},
            {moveRange.nextBehindEnd(), BaseSize - moveRange.nextBehindEnd(), Piece::OriginalStorage}};
}

void PieceTableTest::testSwap()
{
    QFETCH(const Address, firstStart);
    QFETCH(const AddressRange, secondRange);
    QFETCH(const QList<Piece>, expectedPieces);

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
    QTest::addColumn<QList<Piece>>("initPieces");
    QTest::addColumn<Address>("replaceDataOffset");
    QTest::addColumn<Address>("replaceStorageOffset");
    QTest::addColumn<Piece::StorageType>("replaceStorageId");
    QTest::addColumn<Piece>("expectedReplacedPiece");
    QTest::addColumn<QList<Piece>>("expectedPieces");

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
        << QList<Piece> {originalPieceB1}
        << 0 << Start + 1 << Piece::ChangeStorage
        << originalPieceB1
        << QList<Piece> {changePieceB2};

    QTest::newRow("one-with-one-first-continuous")
        << QList<Piece> {originalPieceB1, changePieceA2}
        << 0 << 0 << Piece::ChangeStorage
        << originalPieceB1
        << QList<Piece> {changePieceA12};

    QTest::newRow("one-with-one-first-noncontinuous")
        << QList<Piece> {originalPieceB1, changePieceA1}
        << 0 << Start + 1 << Piece::ChangeStorage
        << originalPieceB1
        << QList<Piece> {changePieceB2, changePieceA1};

    QTest::newRow("one-with-one-second-continuous")
        << QList<Piece> {changePieceA1, originalPieceB1}
        << 1 << 1 << Piece::ChangeStorage
        << originalPieceB1
        << QList<Piece> {changePieceA12};

    QTest::newRow("one-with-one-second-noncontinuous")
        << QList<Piece> {changePieceA1, originalPieceB1}
        << 1 << Start + 1 << Piece::ChangeStorage
        << originalPieceB1
        << QList<Piece> {changePieceA1, changePieceB2};

    QTest::newRow("one-with-one-three-continuous")
        << QList<Piece> {changePieceA1, originalPieceB1, changePieceA3}
        << 1 << 1 << Piece::ChangeStorage
        << originalPieceB1
        << QList<Piece> {changePieceA123};

    QTest::newRow("one-with-begin-multi")
        << QList<Piece> {originalPieceB}
        << 0 << Start + 1 << Piece::ChangeStorage
        << originalPieceB1
        << QList<Piece> {changePieceB2, originalPiece_B};

    QTest::newRow("one-with-middle-multi")
        << QList<Piece> {originalPieceB}
        << Width / 2 << Start + 1 << Piece::ChangeStorage
        << Piece {Start + Width / 2, 1, Piece::OriginalStorage}
        << QList<Piece> {originalPieceBM, changePieceB2, originalPiece_MB};

    QTest::newRow("one-with-end-multi")
        << QList<Piece> {originalPieceB}
        << Width - 1 << Start + 1 << Piece::ChangeStorage
        << originalPiece1B
        << QList<Piece> {originalPieceB_, changePieceB2};

    QTest::newRow("one-with-end-multi-next-continuous")
        << QList<Piece> {originalPieceB, changePiece_B}
        << Width - 1 << Start << Piece::ChangeStorage
        << originalPiece1B
        << QList<Piece> {originalPieceB_, changePieceB};

    QTest::newRow("one-with-end-multi-next-noncontinuous")
        << QList<Piece> {originalPieceB, changePiece__B}
        << Width - 1 << Start << Piece::ChangeStorage
        << originalPiece1B
        << QList<Piece> {originalPieceB_, changePieceB1, changePiece__B};

    QTest::newRow("one-with-begin-multi-previous-continuous")
        << QList<Piece> {originalPieceB_, changePieceB}
        << Width - 1 << End << Piece::OriginalStorage
        << changePieceB1
        << QList<Piece> {originalPieceB, changePiece_B};

    QTest::newRow("one-with-begin-multi-previous-noncontinuous")
        << QList<Piece> {originalPieceB__, changePieceB}
        << Width - 2 << End << Piece::OriginalStorage
        << changePieceB1
        << QList<Piece> {originalPieceB__, originalPiece1B, changePiece_B};
}

void PieceTableTest::testReplaceOne()
{
    QFETCH(const QList<Piece>, initPieces);
    QFETCH(const Address, replaceDataOffset);
    QFETCH(const Address, replaceStorageOffset);
    QFETCH(const Piece::StorageType, replaceStorageId);
    QFETCH(const Piece, expectedReplacedPiece);
    QFETCH(const QList<Piece>, expectedPieces);

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

    QTest::addColumn<QList<Piece>>("pieces");
    QTest::addColumn<QList<StorageDataTestData>>("testData");

    QTest::newRow("void")
        << QList<Piece> {
        }
        << QList<StorageDataTestData> {
            StorageDataTestData::invalid(0),
            StorageDataTestData::invalid(1),
        };

    QTest::newRow("one-original")
        << QList<Piece> {
            {Start, Width, Piece::OriginalStorage},
        }
        << QList<StorageDataTestData> {
            StorageDataTestData::valid(0, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(1, Start + 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width - 2, End - 1, Piece::OriginalStorage),
            StorageDataTestData::valid(Width - 1, End, Piece::OriginalStorage),
            StorageDataTestData::invalid(Width),
        };

    QTest::newRow("one-original-short")
        << QList<Piece> {
            {Start, 1, Piece::OriginalStorage},
        }
        << QList<StorageDataTestData> {
            StorageDataTestData::valid(0, Start, Piece::OriginalStorage),
            StorageDataTestData::invalid(1),
        };

    QTest::newRow("one-change")
        << QList<Piece> {
            {Start2, Width2, Piece::ChangeStorage},
        }
        << QList<StorageDataTestData> {
            StorageDataTestData::valid(0, Start2, Piece::ChangeStorage),
            StorageDataTestData::valid(1, Start2 + 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 - 2, End2 - 1, Piece::ChangeStorage),
            StorageDataTestData::valid(Width2 - 1, End2, Piece::ChangeStorage),
            StorageDataTestData::invalid(Width2),
        };

    QTest::newRow("two-original")
        << QList<Piece> {
            {Start, Width, Piece::OriginalStorage},
            {Start2, Width2, Piece::OriginalStorage},
        }
        << QList<StorageDataTestData> {
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
        << QList<Piece> {
            {Start2, Width2, Piece::ChangeStorage},
            {Start, Width, Piece::ChangeStorage},
        }
        << QList<StorageDataTestData> {
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
        << QList<Piece> {
            {Start, Width, Piece::OriginalStorage},
            {Start2, Width2, Piece::ChangeStorage},
        }
        << QList<StorageDataTestData> {
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
        << QList<Piece> {
            {Start, 1, Piece::OriginalStorage},
            {Start2, 1, Piece::ChangeStorage},
        }
        << QList<StorageDataTestData> {
            StorageDataTestData::valid(0, Start, Piece::OriginalStorage),
            StorageDataTestData::valid(1, Start2, Piece::ChangeStorage),
            StorageDataTestData::invalid(2),
        };

    QTest::newRow("two-change-original")
        << QList<Piece> {
            {Start2, Width2, Piece::ChangeStorage},
            {Start, Width, Piece::OriginalStorage},
        }
        << QList<StorageDataTestData> {
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
    QFETCH(const QList<Piece>, pieces);
    QFETCH(const QList<StorageDataTestData>, testData);

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
