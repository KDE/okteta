/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecelisttest.hpp"

// test object
#include <piecetable/piecelist.hpp>
// Qt
#include <QTest>

Q_DECLARE_METATYPE(KPieceTable::Piece)

namespace KPieceTable {

// TODO: these helper methods duplicated from piecetabletest, see to deduplicate
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
    // TODO: find some useful output with QCOMPARE
    if (pieceList.size() != expectedPieces.size()) {
        for (const auto& piece : expectedPieces) {
            qDebug() << "Expected:" << piece;
        }
        for (const auto& piece : pieceList) {
            qDebug() << "Actual:" << piece;
        }
    }

    QCOMPARE(pieceList.size(), expectedPieces.size());
    QCOMPARE(pieceList.isEmpty(), expectedPieces.isEmpty());

    int i = 0;
    for (const auto& piece : pieceList) {
        const auto& expectedPiece = expectedPieces[i++];
        QCOMPARE(piece, expectedPiece);
    }
}

void PieceListTest::testSimpleConstructor()
{
    // tested action
    PieceList pieceList;

    // check result
    QCOMPARE(pieceList.isEmpty(), true);
    QCOMPARE(pieceList.size(), 0);
    QCOMPARE(pieceList.totalLength(), 0);
}

void PieceListTest::testPieceConstructor_data()
{
    QTest::addColumn<Piece>("initPiece");
    QTest::addColumn<bool>("expectedEmpty");
    QTest::addColumn<int>("expectedSize");
    QTest::addColumn<Size>("expectedTotalLength");

    QTest::newRow("0-10-Original")
        << Piece{0, 10, Piece::OriginalStorage}
        << false << 1 << 10;
    QTest::newRow("10-10-Change")
        << Piece{10, 5, Piece::ChangeStorage}
        << false << 1 << 5;
    QTest::newRow("invalid")
        << Piece{}
        << true << 0 << 0;
}

void PieceListTest::testPieceConstructor()
{
    QFETCH(const Piece, initPiece);
    QFETCH(const bool, expectedEmpty);
    QFETCH(const int, expectedSize);
    QFETCH(const Size, expectedTotalLength);

    // tested action
    PieceList pieceList(initPiece);

    // check result
    QCOMPARE(pieceList.isEmpty(), expectedEmpty);
    QCOMPARE(pieceList.size(), expectedSize);
    QCOMPARE(pieceList.totalLength(), expectedTotalLength);
    if (!expectedEmpty) {
        QCOMPARE(pieceList.at(0), initPiece);
    }
}

void PieceListTest::testAppendPiece_data()
{
    QTest::addColumn<Piece>("initPiece");
    QTest::addColumn<Piece>("appendedPiece");
    QTest::addColumn<Size>("expectedTotalLength");
    QTest::addColumn<QVector<Piece>>("expectedPieces");

    QTest::newRow("emptylist")
        << Piece()
        << Piece{20, 5, Piece::OriginalStorage}
        << 5
        << QVector<Piece> {
            {20, 5, Piece::OriginalStorage}};

    QTest::newRow("invalid")
        << Piece{0, 10, Piece::OriginalStorage}
        << Piece()
        << 10
        << QVector<Piece> {
            {0, 10, Piece::OriginalStorage}};

    QTest::newRow("noncontinous-samestorage")
        << Piece{0, 10, Piece::OriginalStorage}
        << Piece{20, 5, Piece::OriginalStorage}
        << 15
        << QVector<Piece> {
            {0, 10, Piece::OriginalStorage},
            {20, 5, Piece::OriginalStorage}};

    QTest::newRow("noncontinous-differentstorage")
        << Piece{0, 10, Piece::OriginalStorage}
        << Piece{20, 5, Piece::ChangeStorage}
        << 15
        << QVector<Piece> {
            {0, 10, Piece::OriginalStorage},
            {20, 5, Piece::ChangeStorage}};

    QTest::newRow("continous-samestorage")
        << Piece{0, 10, Piece::OriginalStorage}
        << Piece{10, 5, Piece::OriginalStorage}
        << 15
        << QVector<Piece> {
            {0, 15, Piece::OriginalStorage}};

    QTest::newRow("continous-differentstorage")
        << Piece{0, 10, Piece::OriginalStorage}
        << Piece{10, 5, Piece::ChangeStorage}
        << 15
        << QVector<Piece> {
            {0, 10, Piece::OriginalStorage},
            {10, 5, Piece::ChangeStorage}};
}

void PieceListTest::testAppendPiece()
{
    QFETCH(const Piece, initPiece);
    QFETCH(const Piece, appendedPiece);
    QFETCH(const Size, expectedTotalLength);
    QFETCH(const QVector<Piece>, expectedPieces);

    PieceList pieceList(initPiece);

    // tested action
    pieceList.append(appendedPiece);

    // check result
    QCOMPARE(pieceList.totalLength(), expectedTotalLength);
    compare(pieceList, expectedPieces);
}

void PieceListTest::testAppendPieceList_data()
{
    QTest::addColumn<QVector<Piece>>("initPieces");
    QTest::addColumn<QVector<Piece>>("appendedPieces");
    QTest::addColumn<Size>("expectedTotalLength");
    QTest::addColumn<QVector<Piece>>("expectedPieces");

    QTest::newRow("to-emptylist")
        << QVector<Piece> ()
        << QVector<Piece> {
            {20, 5, Piece::OriginalStorage}}
        << 5
        << QVector<Piece> {
            {20, 5, Piece::OriginalStorage}};

    QTest::newRow("by-emptylist")
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}}
        << QVector<Piece> ()
        << 15
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}};

    QTest::newRow("noncontinous-samestorage")
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}}
        << QVector<Piece> {
            {20, 5, Piece::OriginalStorage}}
        << 20
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage},
            {20, 5, Piece::OriginalStorage}};

    QTest::newRow("noncontinous-differentstorage")
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}}
        << QVector<Piece> {
            {20, 5, Piece::ChangeStorage},
            {20, 10, Piece::OriginalStorage}}
        << 30
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage},
            {20, 5, Piece::ChangeStorage},
            {20, 10, Piece::OriginalStorage}};

    QTest::newRow("continous-samestorage")
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}}
        << QVector<Piece> {
            {10, 5, Piece::OriginalStorage},
            {20, 5, Piece::OriginalStorage}}
        << 25
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 15, Piece::OriginalStorage},
            {20, 5, Piece::OriginalStorage}};

    QTest::newRow("continous-differentstorage")
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}}
        << QVector<Piece> {
            {10, 5, Piece::ChangeStorage},
            {20, 5, Piece::OriginalStorage}}
        << 25
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage},
            {10, 5, Piece::ChangeStorage},
            {20, 5, Piece::OriginalStorage}};
}

void PieceListTest::testAppendPieceList()
{
    QFETCH(const QVector<Piece>, initPieces);
    QFETCH(const QVector<Piece>, appendedPieces);
    QFETCH(const Size, expectedTotalLength);
    QFETCH(const QVector<Piece>, expectedPieces);

    PieceList pieceList;
    for (const auto& piece : initPieces) {
        pieceList.append(piece);
    }

    PieceList appendedPieceList;
    for (const auto& piece : appendedPieces) {
        appendedPieceList.append(piece);
    }

    // tested action
    pieceList.append(appendedPieceList);

    // check result
    QCOMPARE(pieceList.totalLength(), expectedTotalLength);
    compare(pieceList, expectedPieces);
}

void PieceListTest::testPrependPieceList_data()
{
    QTest::addColumn<QVector<Piece>>("initPieces");
    QTest::addColumn<QVector<Piece>>("prependedPieces");
    QTest::addColumn<Size>("expectedTotalLength");
    QTest::addColumn<QVector<Piece>>("expectedPieces");

    QTest::newRow("to-emptylist")
        << QVector<Piece> ()
        << QVector<Piece> {
            {20, 5, Piece::OriginalStorage}}
        << 5
        << QVector<Piece> {
            {20, 5, Piece::OriginalStorage}};

    QTest::newRow("by-emptylist")
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}}
        << QVector<Piece> ()
        << 15
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}};

    QTest::newRow("noncontinous-samestorage")
        << QVector<Piece> {
            {20, 5, Piece::OriginalStorage}}
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}}
        << 20
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage},
            {20, 5, Piece::OriginalStorage}};

    QTest::newRow("noncontinous-differentstorage")
        << QVector<Piece> {
            {20, 5, Piece::ChangeStorage},
            {20, 10, Piece::OriginalStorage}}
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}}
        << 30
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage},
            {20, 5, Piece::ChangeStorage},
            {20, 10, Piece::OriginalStorage}};

    QTest::newRow("continous-samestorage")
        << QVector<Piece> {
            {10, 5, Piece::OriginalStorage},
            {20, 5, Piece::OriginalStorage}}
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}}
        << 25
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 15, Piece::OriginalStorage},
            {20, 5, Piece::OriginalStorage}};

    QTest::newRow("continous-differentstorage")
        << QVector<Piece> {
            {10, 5, Piece::ChangeStorage},
            {20, 5, Piece::OriginalStorage}}
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage}}
        << 25
        << QVector<Piece> {
            {0, 5, Piece::ChangeStorage},
            {0, 10, Piece::OriginalStorage},
            {10, 5, Piece::ChangeStorage},
            {20, 5, Piece::OriginalStorage}};
}

void PieceListTest::testPrependPieceList()
{
    QFETCH(const QVector<Piece>, initPieces);
    QFETCH(const QVector<Piece>, prependedPieces);
    QFETCH(const Size, expectedTotalLength);
    QFETCH(const QVector<Piece>, expectedPieces);

    PieceList pieceList;
    for (const auto& piece : initPieces) {
        pieceList.append(piece);
    }

    PieceList prependedPieceList;
    for (const auto& piece : prependedPieces) {
        prependedPieceList.append(piece);
    }

    // tested action
    pieceList.prepend(prependedPieceList);

    // check result
    QCOMPARE(pieceList.totalLength(), expectedTotalLength);
    compare(pieceList, expectedPieces);
}

}


QTEST_GUILESS_MAIN(KPieceTable::PieceListTest)

#include "moc_piecelisttest.cpp"
