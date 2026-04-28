/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "piecetest.hpp"

// test object
#include <piecetable/piece.hpp>
// Qt
#include <QTest>

namespace KPieceTable {
struct PieceDataTestData
{
    static PieceDataTestData invalid()
    { return {}; }
    static PieceDataTestData valid(Address start, Address end, Piece::StorageType storageId)
    { return {true, start, end, storageId}; }

    PieceDataTestData() = default;

    bool isValid() const { return m_isValid; }
    Address start() const { return m_start; }
    Address end() const { return m_end; }
    Piece::StorageType storageId() const { return m_storageId; }

private:
    PieceDataTestData(bool isValid, Address start, Address end, Piece::StorageType storageId)
        : m_isValid(isValid), m_start(start), m_end(end), m_storageId(storageId)
    {}

private:
    const bool m_isValid = false;
    const Address m_start = 0;
    const Address m_end = -1;
    const Piece::StorageType m_storageId = Piece::OriginalStorage;
};
}

Q_DECLARE_METATYPE(KPieceTable::AddressRange)
Q_DECLARE_METATYPE(KPieceTable::PieceDataTestData)
Q_DECLARE_METATYPE(KPieceTable::Piece::StorageType)

namespace KPieceTable {

// local variables
static constexpr Address Start = 15;
static constexpr Address End = 27;

static constexpr Address Width = End - Start + 1;

void PieceTest::testSimpleConstructor()
{
    Piece piece;
    QVERIFY(!piece.isValid());
}

void PieceTest::testFullConstructor()
{
    const AddressRange storageSection(Start, End);
    Piece piece(storageSection, Piece::ChangeStorage);

    QCOMPARE(piece.start(), Start);
    QCOMPARE(piece.end(), storageSection.end());
    QCOMPARE(piece.storageId(), Piece::ChangeStorage);
    QVERIFY(piece.isValid());
}

void PieceTest::testEqualOperator_data()
{
    QTest::addColumn<AddressRange>("section");
    QTest::addColumn<Piece::StorageType>("storageId");
    QTest::addColumn<bool>("expectedSameEqual");

    QTest::newRow("same")
        << AddressRange(Start, End) << Piece::OriginalStorage
        << true;

    QTest::newRow("other-start")
        << AddressRange(Start - 1, End) << Piece::OriginalStorage
        << false;

    QTest::newRow("other-end")
        << AddressRange(Start, End + 1) << Piece::OriginalStorage
        << false;

    QTest::newRow("other-storage")
        << AddressRange(Start, End) << Piece::ChangeStorage
        << false;

    QTest::newRow("all-other")
        << AddressRange(Start - 1, End + 1) << Piece::ChangeStorage
        << false;
}

void PieceTest::testEqualOperator()
{
    QFETCH(const AddressRange, section);
    QFETCH(const Piece::StorageType, storageId);
    QFETCH(const bool, expectedSameEqual);
    const bool expectedSameNotEqual = !expectedSameEqual;

    const Piece originalPiece(Start, Width, Piece::OriginalStorage);

    const Piece otherPiece(section, storageId);

    // tested action
    const bool isSameEqual = (originalPiece == otherPiece);
    const bool isSameNotEqual = (originalPiece != otherPiece);

    // check result
    QCOMPARE(isSameEqual, expectedSameEqual);
    QCOMPARE(isSameNotEqual, expectedSameNotEqual);
}

void PieceTest::testSetStorageId()
{
    Piece piece(Start, Width, Piece::OriginalStorage);

    // tested action
    piece.setStorageId(Piece::ChangeStorage);

    // check result
    QCOMPARE(piece.storageId(), Piece::ChangeStorage);

    // tested action
    piece.setStorageId(Piece::OriginalStorage);

    // check result
    QCOMPARE(piece.storageId(), Piece::OriginalStorage);
}

void PieceTest::testSplitAt_data()
{
    QTest::addColumn<Address>("splitAddress");
    QTest::addColumn<PieceDataTestData>("expectedPieceData");
    QTest::addColumn<PieceDataTestData>("expectedSplitPieceData");

    const Address Mid = (Start + End) / 2;

    QTest::newRow("at-start")
        << Start
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage);

    QTest::newRow("at-one-after-start")
        << Start + 1
        << PieceDataTestData::valid(Start, Start, Piece::ChangeStorage)
        << PieceDataTestData::valid(Start + 1, End, Piece::ChangeStorage);

    QTest::newRow("at-mid")
        << Mid
        << PieceDataTestData::valid(Start, Mid - 1, Piece::ChangeStorage)
        << PieceDataTestData::valid(Mid, End, Piece::ChangeStorage);

    QTest::newRow("at-one-before-width")
        << End
        << PieceDataTestData::valid(Start, End - 1, Piece::ChangeStorage)
        << PieceDataTestData::valid(End, End, Piece::ChangeStorage);

    QTest::newRow("at-start")
        << End + 1
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage)
        << PieceDataTestData::invalid();
}

void PieceTest::testSplitAt()
{
    QFETCH(const Address, splitAddress);
    QFETCH(const PieceDataTestData, expectedPieceData);
    QFETCH(const PieceDataTestData, expectedSplitPieceData);

    const AddressRange storageSection(Start, End);
    Piece piece(storageSection, Piece::ChangeStorage);

    // tested action
    const Piece splitPiece = piece.splitAt(splitAddress);

    // check result
    QCOMPARE(piece.isValid(), expectedPieceData.isValid());
    if (expectedPieceData.isValid()) {
        QCOMPARE(piece.start(), expectedPieceData.start());
        QCOMPARE(piece.end(),   expectedPieceData.end());
        QCOMPARE(piece.storageId(), expectedPieceData.storageId());
    }
    QCOMPARE(splitPiece.isValid(), expectedSplitPieceData.isValid());
    if (expectedSplitPieceData.isValid()) {
        QCOMPARE(splitPiece.start(), expectedSplitPieceData.start());
        QCOMPARE(splitPiece.end(),   expectedSplitPieceData.end());
        QCOMPARE(splitPiece.storageId(), expectedSplitPieceData.storageId());
    }
}

void PieceTest::testSplitAtLocal_data()
{
    QTest::addColumn<Address>("splitAddress");
    QTest::addColumn<PieceDataTestData>("expectedPieceData");
    QTest::addColumn<PieceDataTestData>("expectedSplitPieceData");

    const Address Mid = Width / 2;

    QTest::newRow("at-start")
        << 0
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage);

    QTest::newRow("at-one-after-start")
        << 1
        << PieceDataTestData::valid(Start, Start, Piece::ChangeStorage)
        << PieceDataTestData::valid(Start + 1, End, Piece::ChangeStorage);

    QTest::newRow("at-mid")
        << Mid
        << PieceDataTestData::valid(Start, Start + Mid - 1, Piece::ChangeStorage)
        << PieceDataTestData::valid(Start + Mid, End, Piece::ChangeStorage);

    QTest::newRow("at-one-before-width")
        << Width - 1
        << PieceDataTestData::valid(Start, End - 1, Piece::ChangeStorage)
        << PieceDataTestData::valid(End, End, Piece::ChangeStorage);

    QTest::newRow("at-start")
        << Width
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage)
        << PieceDataTestData::invalid();
}

void PieceTest::testSplitAtLocal()
{
    QFETCH(const Address, splitAddress);
    QFETCH(const PieceDataTestData, expectedPieceData);
    QFETCH(const PieceDataTestData, expectedSplitPieceData);

    const AddressRange storageSection(Start, End);
    Piece piece(storageSection, Piece::ChangeStorage);

    // tested action
    const Piece splitPiece = piece.splitAtLocal(splitAddress);

    // check result
    QCOMPARE(piece.isValid(), expectedPieceData.isValid());
    if (expectedPieceData.isValid()) {
        QCOMPARE(piece.start(), expectedPieceData.start());
        QCOMPARE(piece.end(),   expectedPieceData.end());
        QCOMPARE(piece.storageId(), expectedPieceData.storageId());
    }
    QCOMPARE(splitPiece.isValid(), expectedSplitPieceData.isValid());
    if (expectedSplitPieceData.isValid()) {
        QCOMPARE(splitPiece.start(), expectedSplitPieceData.start());
        QCOMPARE(splitPiece.end(),   expectedSplitPieceData.end());
        QCOMPARE(splitPiece.storageId(), expectedSplitPieceData.storageId());
    }
}

void PieceTest::testRemove_data()
{
    QTest::addColumn<AddressRange>("removeSection");
    QTest::addColumn<PieceDataTestData>("expectedPieceData");
    QTest::addColumn<PieceDataTestData>("expectedSplitPieceData");

    const Address Mid = (Start + End) / 2;

    QTest::newRow("none-at-start")
        << AddressRange(Start, Start - 1)
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage);

    QTest::newRow("one-at-start")
        << AddressRange(Start, Start)
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(Start + 1, End, Piece::ChangeStorage);

    QTest::newRow("many-at-start")
        << AddressRange(Start, Mid)
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(Mid + 1, End, Piece::ChangeStorage);

    QTest::newRow("all-except-last")
        << AddressRange(Start, End - 1)
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(End, End, Piece::ChangeStorage);

    QTest::newRow("at-mid")
        << AddressRange(Mid - 1, Mid + 1)
        << PieceDataTestData::valid(Start, Mid - 2, Piece::ChangeStorage)
        << PieceDataTestData::valid(Mid + 2, End, Piece::ChangeStorage);

    QTest::newRow("none-at-width")
        << AddressRange(End + 1, End)
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage)
        << PieceDataTestData::invalid();

    QTest::newRow("one-at-width")
        << AddressRange(End, End)
        << PieceDataTestData::valid(Start, End - 1, Piece::ChangeStorage)
        << PieceDataTestData::invalid();

    QTest::newRow("many-at-width")
        << AddressRange(Mid, End)
        << PieceDataTestData::valid(Start, Mid - 1, Piece::ChangeStorage)
        << PieceDataTestData::invalid();

    QTest::newRow("all-except-first")
        << AddressRange(Start + 1, End)
        << PieceDataTestData::valid(Start, Start, Piece::ChangeStorage)
        << PieceDataTestData::invalid();

    QTest::newRow("all")
        << AddressRange(Start, End)
        << PieceDataTestData::invalid()
        << PieceDataTestData::invalid();
}

void PieceTest::testRemove()
{
    QFETCH(const AddressRange, removeSection);
    QFETCH(const PieceDataTestData, expectedPieceData);
    QFETCH(const PieceDataTestData, expectedSplitPieceData);

    const AddressRange storageSection(Start, End);
    Piece piece(storageSection, Piece::ChangeStorage);

    // tested action
    const Piece removedPiece = piece.remove(removeSection);

    // check result
    QCOMPARE(piece.isValid(), expectedPieceData.isValid());
    if (expectedPieceData.isValid()) {
        QCOMPARE(piece.start(), expectedPieceData.start());
        QCOMPARE(piece.end(),   expectedPieceData.end());
        QCOMPARE(piece.storageId(), expectedPieceData.storageId());
    }
    QCOMPARE(removedPiece.isValid(), expectedSplitPieceData.isValid());
    if (expectedSplitPieceData.isValid()) {
        QCOMPARE(removedPiece.start(), expectedSplitPieceData.start());
        QCOMPARE(removedPiece.end(),   expectedSplitPieceData.end());
        QCOMPARE(removedPiece.storageId(), expectedSplitPieceData.storageId());
    }
}

void PieceTest::testRemoveLocal_data()
{
    QTest::addColumn<AddressRange>("removeSection");
    QTest::addColumn<PieceDataTestData>("expectedPieceData");
    QTest::addColumn<PieceDataTestData>("expectedSplitPieceData");

    const Address Mid = Width / 2;

    QTest::newRow("none-at-start")
        << AddressRange(0, 0 - 1)
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage);

    QTest::newRow("one-at-start")
        << AddressRange(0, 0)
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(Start + 1, End, Piece::ChangeStorage);

    QTest::newRow("many-at-start")
        << AddressRange(0, Mid)
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(Start + Mid + 1, End, Piece::ChangeStorage);

    QTest::newRow("all-except-last")
        << AddressRange(0, Width - 2)
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(End, End, Piece::ChangeStorage);

    QTest::newRow("at-mid")
        << AddressRange(Mid - 1, Mid + 1)
        << PieceDataTestData::valid(Start, Start + Mid - 2, Piece::ChangeStorage)
        << PieceDataTestData::valid(Start + Mid + 2, End, Piece::ChangeStorage);

    QTest::newRow("none-at-width")
        << AddressRange(Width, Width - 1)
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage)
        << PieceDataTestData::invalid();

    QTest::newRow("one-at-width")
        << AddressRange(Width - 1, Width - 1)
        << PieceDataTestData::valid(Start, End - 1, Piece::ChangeStorage)
        << PieceDataTestData::invalid();

    QTest::newRow("many-at-width")
        << AddressRange(Mid, Width - 1)
        << PieceDataTestData::valid(Start, Start + Mid - 1, Piece::ChangeStorage)
        << PieceDataTestData::invalid();

    QTest::newRow("all-except-first")
        << AddressRange(1, Width - 1)
        << PieceDataTestData::valid(Start, Start, Piece::ChangeStorage)
        << PieceDataTestData::invalid();

    QTest::newRow("all")
        << AddressRange(0, Width - 1)
        << PieceDataTestData::invalid()
        << PieceDataTestData::invalid();
}

void PieceTest::testRemoveLocal()
{
    QFETCH(const AddressRange, removeSection);
    QFETCH(const PieceDataTestData, expectedPieceData);
    QFETCH(const PieceDataTestData, expectedSplitPieceData);

    const AddressRange storageSection(Start, End);
    Piece piece(storageSection, Piece::ChangeStorage);

    // tested action
    const Piece removedPiece = piece.removeLocal(removeSection);

    // check result
    QCOMPARE(piece.isValid(), expectedPieceData.isValid());
    if (expectedPieceData.isValid()) {
        QCOMPARE(piece.start(), expectedPieceData.start());
        QCOMPARE(piece.end(),   expectedPieceData.end());
        QCOMPARE(piece.storageId(), expectedPieceData.storageId());
    }
    QCOMPARE(removedPiece.isValid(), expectedSplitPieceData.isValid());
    if (expectedSplitPieceData.isValid()) {
        QCOMPARE(removedPiece.start(), expectedSplitPieceData.start());
        QCOMPARE(removedPiece.end(),   expectedSplitPieceData.end());
        QCOMPARE(removedPiece.storageId(), expectedSplitPieceData.storageId());
    }
}

void PieceTest::testRemoveStartBeforeLocal_data()
{
    QTest::addColumn<Address>("localRemoveAddress");
    QTest::addColumn<PieceDataTestData>("expectedPieceData");
    QTest::addColumn<PieceDataTestData>("expectedRemovedPieceData");

    const Address Mid = Width / 2;

    QTest::newRow("none")
        << 0
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage)
        << PieceDataTestData::invalid();

    QTest::newRow("one")
        << 1
        << PieceDataTestData::valid(Start + 1, End, Piece::ChangeStorage)
        << PieceDataTestData::valid(Start, Start, Piece::ChangeStorage);

    QTest::newRow("many")
        << Mid
        << PieceDataTestData::valid(Start + Mid, End, Piece::ChangeStorage)
        << PieceDataTestData::valid(Start, Start + Mid - 1, Piece::ChangeStorage);

    QTest::newRow("all-except-last")
        << Width - 1
        << PieceDataTestData::valid(End, End, Piece::ChangeStorage)
        << PieceDataTestData::valid(Start, End - 1, Piece::ChangeStorage);

    QTest::newRow("all")
        << Width
        << PieceDataTestData::invalid()
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage);
}

void PieceTest::testRemoveStartBeforeLocal()
{
    QFETCH(const Address, localRemoveAddress);
    QFETCH(const PieceDataTestData, expectedPieceData);
    QFETCH(const PieceDataTestData, expectedRemovedPieceData);

    const AddressRange storageSection(Start, End);
    Piece piece(storageSection, Piece::ChangeStorage);

    // tested action
    const Piece removedPiece = piece.removeStartBeforeLocal(localRemoveAddress);

    // check result
    QCOMPARE(piece.isValid(), expectedPieceData.isValid());
    if (expectedPieceData.isValid()) {
        QCOMPARE(piece.start(), expectedPieceData.start());
        QCOMPARE(piece.end(),   expectedPieceData.end());
        QCOMPARE(piece.storageId(), expectedPieceData.storageId());
    }
    QCOMPARE(removedPiece.isValid(), expectedRemovedPieceData.isValid());
    if (expectedRemovedPieceData.isValid()) {
        QCOMPARE(removedPiece.start(), expectedRemovedPieceData.start());
        QCOMPARE(removedPiece.end(),   expectedRemovedPieceData.end());
        QCOMPARE(removedPiece.storageId(), expectedRemovedPieceData.storageId());
    }
}

void PieceTest::testRemoveEndBehindLocal_data()
{
    QTest::addColumn<Address>("localRemoveAddress");
    QTest::addColumn<PieceDataTestData>("expectedPieceData");
    QTest::addColumn<PieceDataTestData>("expectedRemovedPieceData");

    const Address Mid = Width / 2;

    QTest::newRow("none")
        << Width - 1
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage)
        << PieceDataTestData::invalid();
    QTest::newRow("one")
        << Width - 2
        << PieceDataTestData::valid(Start, End - 1, Piece::ChangeStorage)
        << PieceDataTestData::valid(End, End, Piece::ChangeStorage);

    QTest::newRow("many")
        << Mid
        << PieceDataTestData::valid(Start, Start + Mid, Piece::ChangeStorage)
        << PieceDataTestData::valid(Start + Mid + 1, End, Piece::ChangeStorage);

    QTest::newRow("all-except-first")
        << 0
        << PieceDataTestData::valid(Start, Start, Piece::ChangeStorage)
        << PieceDataTestData::valid(Start + 1, End, Piece::ChangeStorage);
}

void PieceTest::testRemoveEndBehindLocal()
{
    QFETCH(const Address, localRemoveAddress);
    QFETCH(const PieceDataTestData, expectedPieceData);
    QFETCH(const PieceDataTestData, expectedRemovedPieceData);

    const AddressRange storageSection(Start, End);
    Piece piece(storageSection, Piece::ChangeStorage);

    // tested action
    const Piece removedPiece = piece.removeEndBehindLocal(localRemoveAddress);

    // check result
    QCOMPARE(piece.isValid(), expectedPieceData.isValid());
    if (expectedPieceData.isValid()) {
        QCOMPARE(piece.start(), expectedPieceData.start());
        QCOMPARE(piece.end(),   expectedPieceData.end());
        QCOMPARE(piece.storageId(), expectedPieceData.storageId());
    }
    QCOMPARE(removedPiece.isValid(), expectedRemovedPieceData.isValid());
    if (expectedRemovedPieceData.isValid()) {
        QCOMPARE(removedPiece.start(), expectedRemovedPieceData.start());
        QCOMPARE(removedPiece.end(),   expectedRemovedPieceData.end());
        QCOMPARE(removedPiece.storageId(), expectedRemovedPieceData.storageId());
    }
}

void PieceTest::testPrepend_data()
{
    QTest::addColumn<AddressRange>("initSection");
    QTest::addColumn<Piece::StorageType>("initStorageId");
    QTest::addColumn<AddressRange>("prependSection");
    QTest::addColumn<Piece::StorageType>("prependStorageId");
    QTest::addColumn<bool>("expectedSuccess");
    QTest::addColumn<PieceDataTestData>("expectedPieceData");

    QTest::newRow("noncontinous-samestorage")
        << AddressRange(Start, End) << Piece::OriginalStorage
        << AddressRange(0, Start - 2) << Piece::OriginalStorage
        << false << PieceDataTestData::valid(Start, End, Piece::OriginalStorage);

    QTest::newRow("noncontinous-differentstorage")
        << AddressRange(Start, End) << Piece::OriginalStorage
        << AddressRange(0, Start - 2) << Piece::ChangeStorage
        << false << PieceDataTestData::valid(Start, End, Piece::OriginalStorage);

    QTest::newRow("continous-samestorage")
        << AddressRange(Start, End) << Piece::OriginalStorage
        << AddressRange(0, Start - 1) << Piece::OriginalStorage
        << true << PieceDataTestData::valid(0, End, Piece::OriginalStorage);

    QTest::newRow("continous-differentstorage")
        << AddressRange(Start, End) << Piece::OriginalStorage
        << AddressRange(0, Start - 1) << Piece::ChangeStorage
        << false << PieceDataTestData::valid(Start, End, Piece::OriginalStorage);
}

void PieceTest::testPrepend()
{
    QFETCH(const AddressRange, initSection);
    QFETCH(const Piece::StorageType, initStorageId);
    QFETCH(const AddressRange, prependSection);
    QFETCH(const Piece::StorageType, prependStorageId);
    QFETCH(const bool, expectedSuccess);
    QFETCH(const PieceDataTestData, expectedPieceData);

    Piece piece(initSection, initStorageId);

    const Piece otherPiece(prependSection, prependStorageId);

    // tested action
    const bool success = piece.prepend(otherPiece);

    // check result
    QCOMPARE(success, expectedSuccess);
    QCOMPARE(piece.isValid(), expectedPieceData.isValid());
    if (expectedPieceData.isValid()) {
        QCOMPARE(piece.start(), expectedPieceData.start());
        QCOMPARE(piece.end(),   expectedPieceData.end());
        QCOMPARE(piece.storageId(), expectedPieceData.storageId());
    }
}

void PieceTest::testAppend_data()
{
    QTest::addColumn<AddressRange>("initSection");
    QTest::addColumn<Piece::StorageType>("initStorageId");
    QTest::addColumn<AddressRange>("prependSection");
    QTest::addColumn<Piece::StorageType>("prependStorageId");
    QTest::addColumn<bool>("expectedSuccess");
    QTest::addColumn<PieceDataTestData>("expectedPieceData");

    QTest::newRow("noncontinous-samestorage")
        << AddressRange(Start, End) << Piece::OriginalStorage
        << AddressRange(End + 2, End + 10) << Piece::OriginalStorage
        << false << PieceDataTestData::valid(Start, End, Piece::OriginalStorage);

    QTest::newRow("noncontinous-differentstorage")
        << AddressRange(Start, End) << Piece::OriginalStorage
        << AddressRange(End + 2, End + 10) << Piece::ChangeStorage
        << false << PieceDataTestData::valid(Start, End, Piece::OriginalStorage);

    QTest::newRow("continous-samestorage")
        << AddressRange(Start, End) << Piece::OriginalStorage
        << AddressRange(End + 1, End + 10) << Piece::OriginalStorage
        << true << PieceDataTestData::valid(Start, End + 10, Piece::OriginalStorage);

    QTest::newRow("continous-differentstorage")
        << AddressRange(Start, End) << Piece::OriginalStorage
        << AddressRange(End + 1, End + 10) << Piece::ChangeStorage
        << false << PieceDataTestData::valid(Start, End, Piece::OriginalStorage);
}

void PieceTest::testAppend()
{
    QFETCH(const AddressRange, initSection);
    QFETCH(const Piece::StorageType, initStorageId);
    QFETCH(const AddressRange, prependSection);
    QFETCH(const Piece::StorageType, prependStorageId);
    QFETCH(const bool, expectedSuccess);
    QFETCH(const PieceDataTestData, expectedPieceData);

    Piece piece(initSection, initStorageId);

    const Piece otherPiece(prependSection, prependStorageId);

    // tested action
    const bool success = piece.append(otherPiece);

    // check result
    QCOMPARE(success, expectedSuccess);
    QCOMPARE(piece.isValid(), expectedPieceData.isValid());
    if (expectedPieceData.isValid()) {
        QCOMPARE(piece.start(), expectedPieceData.start());
        QCOMPARE(piece.end(),   expectedPieceData.end());
        QCOMPARE(piece.storageId(), expectedPieceData.storageId());
    }
}

void PieceTest::testSubPiece_data()
{
    QTest::addColumn<AddressRange>("subRange");
    QTest::addColumn<PieceDataTestData>("expectedSubPieceData");

    const Address Mid = Width / 2;

    QTest::newRow("none-at-start")
        << AddressRange(0, 0 - 1)
        << PieceDataTestData::invalid();

    QTest::newRow("one-at-start")
        << AddressRange(0, 0)
        << PieceDataTestData::valid(Start, Start, Piece::ChangeStorage);

    QTest::newRow("many-at-start")
        << AddressRange(0, Mid)
        << PieceDataTestData::valid(Start, Start + Mid, Piece::ChangeStorage);

    QTest::newRow("all-except-last")
        << AddressRange(0, Width - 2)
        << PieceDataTestData::valid(Start, End - 1, Piece::ChangeStorage);

    QTest::newRow("at-mid")
        << AddressRange(Mid - 1, Mid + 1)
        << PieceDataTestData::valid(Start + Mid - 1, Start + Mid + 1, Piece::ChangeStorage);

    QTest::newRow("none-at-width")
        << AddressRange(Width, Width - 1)
        << PieceDataTestData::invalid();

    QTest::newRow("one-at-width")
        << AddressRange(Width - 1, Width - 1)
        << PieceDataTestData::valid(End, End, Piece::ChangeStorage);

    QTest::newRow("many-at-width")
        << AddressRange(Mid, Width - 1)
        << PieceDataTestData::valid(Start + Mid, End, Piece::ChangeStorage);

    QTest::newRow("all-except-first")
        << AddressRange(1, Width - 1)
        << PieceDataTestData::valid(Start + 1, End, Piece::ChangeStorage);

    QTest::newRow("all")
        << AddressRange(0, Width - 1)
        << PieceDataTestData::valid(Start, End, Piece::ChangeStorage);
}

void PieceTest::testSubPiece()
{
    QFETCH(const AddressRange, subRange);
    QFETCH(const PieceDataTestData, expectedSubPieceData);

    const AddressRange storageSection(Start, End);
    Piece piece(storageSection, Piece::ChangeStorage);

    // tested action
    const Piece subPiece = piece.subPiece(subRange);

    // check result
    QCOMPARE(subPiece.isValid(), expectedSubPieceData.isValid());
    if (expectedSubPieceData.isValid()) {
        QCOMPARE(subPiece.start(), expectedSubPieceData.start());
        QCOMPARE(subPiece.end(),   expectedSubPieceData.end());
        QCOMPARE(subPiece.storageId(), expectedSubPieceData.storageId());
    }
}

}

QTEST_GUILESS_MAIN(KPieceTable::PieceTest)

#include "moc_piecetest.cpp"
