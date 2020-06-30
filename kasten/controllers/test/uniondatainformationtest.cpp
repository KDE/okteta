/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *    SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QTest>

#include <Okteta/ByteArrayModel>
#include <Okteta/Byte>

#include "testutils.hpp"

#include "view/structures/datatypes/uniondatainformation.hpp"
#include "view/structures/datatypes/topleveldatainformation.hpp"
#include "view/structures/datatypes/primitivefactory.hpp"
#include "view/structures/datatypes/primitive/bitfield/unsignedbitfielddatainformation.hpp"

class UnionDataInformationTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void testSize();
    void testReadData1();
    void testReadData1_data();

private:
    QScopedPointer<Okteta::ByteArrayModel> model;
};

/** In binary:
 *  00010010 00110100 01010110 01111000
 *  10011010 10111100 11011110 11110000
 */
static constexpr Okteta::Byte testData[8] = {
    0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0
};

void UnionDataInformationTest::initTestCase()
{
    model.reset(new Okteta::ByteArrayModel(testData, sizeof(testData)));
    model->setAutoDelete(false);
    QCOMPARE(model->size(), Okteta::Size(8));
}

void UnionDataInformationTest::testSize()
{
    LoggerWithContext lwc(nullptr, QString());

    UnionDataInformation empty(QStringLiteral("empty"));
    QCOMPARE(empty.size(), BitCount32(0));
    QVector<DataInformation*> size54;
    size54 << PrimitiveFactory::newInstance(QStringLiteral("8"), PrimitiveDataType::Bool8, lwc);
    size54 << PrimitiveFactory::newInstance(QStringLiteral("16"), PrimitiveDataType::Int16, lwc);
    size54 << PrimitiveFactory::newInstance(QStringLiteral("32"), PrimitiveDataType::Float, lwc);
    size54 << new UnsignedBitfieldDataInformation(QStringLiteral("54"), 54);
    UnionDataInformation fourChildren(QStringLiteral("four"), size54);
    QCOMPARE(fourChildren.size(), BitCount32(54));
}

void UnionDataInformationTest::testReadData1()
{
    LoggerWithContext lwc(nullptr, QString());

    PrimitiveDataInformation* b8 = PrimitiveFactory::newInstance(QStringLiteral("8"), PrimitiveDataType::Bool8, lwc);
    PrimitiveDataInformation* u32 = PrimitiveFactory::newInstance(QStringLiteral("32"), PrimitiveDataType::UInt32, lwc);
    PrimitiveDataInformation* i16 = PrimitiveFactory::newInstance(QStringLiteral("16"), PrimitiveDataType::Int16, lwc);
    UnsignedBitfieldDataInformation* u54 = new UnsignedBitfieldDataInformation(QStringLiteral("54"),
                                                                               54);
    QVector<DataInformation*> children;
    children << b8 << u32 << i16 << u54;
    UnionDataInformation* un = new UnionDataInformation(QStringLiteral("un"), children);
    un->setByteOrder(DataInformation::DataInformationEndianess::EndianessLittle);
    TopLevelDataInformation top(un);
    // read from bit 0
    QFETCH(uint, address);
    QFETCH(quint8, offset);
    QFETCH(quint64, bitsRemaining);
    // update needed before read
    top.scriptHandler()->updateDataInformation(un);
    qint64 read = un->readData(model.data(), address, bitsRemaining, &offset);
    QFETCH(qint64, readBitsTotal);
    QCOMPARE(read, readBitsTotal);
    QFETCH(quint8, offsetAfterRead);
    QCOMPARE(offset, offsetAfterRead);
    QFETCH(bool, unWasAble);
    QCOMPARE(un->wasAbleToRead(), unWasAble);

    QFETCH(quint8, b8Expected);
    QFETCH(bool, b8WasAble);
    QFETCH(qint16, i16Expected);
    QFETCH(bool, i16WasAble);
    QFETCH(quint32, u32Expected);
    QFETCH(bool, u32WasAble);
    QFETCH(quint64, u54Expected);
    QFETCH(bool, u54WasAble);

    QCOMPARE(b8->value().value<quint8>(), b8Expected);
    QCOMPARE(b8->wasAbleToRead(), b8WasAble);
    QCOMPARE(i16->value().value<qint16>(), i16Expected);
    QCOMPARE(i16->wasAbleToRead(), i16WasAble);
    QCOMPARE(u32->value().value<quint32>(), u32Expected);
    QCOMPARE(u32->wasAbleToRead(), u32WasAble);
    QCOMPARE(u54->value().value<quint64>(), u54Expected);
    QCOMPARE(u54->wasAbleToRead(), u54WasAble);
}

void UnionDataInformationTest::testReadData1_data()
{
    QTest::addColumn<uint>("address");
    QTest::addColumn<quint8>("offset");
    QTest::addColumn<quint64>("bitsRemaining"); // XXX get rid of this argument?

    QTest::addColumn<qint64>("readBitsTotal");
    QTest::addColumn<quint8>("offsetAfterRead");
    QTest::addColumn<bool>("unWasAble");

    QTest::addColumn<quint8>("b8Expected");
    QTest::addColumn<bool>("b8WasAble");
    QTest::addColumn<qint16>("i16Expected");
    QTest::addColumn<bool>("i16WasAble");
    QTest::addColumn<quint32>("u32Expected");
    QTest::addColumn<bool>("u32WasAble");
    QTest::addColumn<quint64>("u54Expected");
    QTest::addColumn<bool>("u54WasAble");

    /* In little endian:
     *  11110000 11011110 10111100 10011010
     *  01111000 01010110 00110100 00010010
     */
    QTest::newRow("0 bytes 0 bits") << 0u << quint8(0) << quint64(64) << qint64(54) << quint8(6) << true
        /* now b8 and i16 */ << quint8(0x12u) << true << qint16(0x3412) << true
        /* u32 and u54 */ << quint32(0x78563412u) << true << Q_UINT64_C(0x1ebc9a78563412) << true;
    // shifted by two bytes in little endian:
    // 11110000 11011110 10111100 10011010 01111000 01010110 00110100 000100xx
    QTest::newRow("0 bytes 2 bits") << 0u << quint8(2) << quint64(62) << qint64(54) << quint8(0) << true
        /* b8 */ << Utils::binary<quint8>("00 000100") << true
        /* i16 */ << Utils::binary<qint16>("10 00110100 000100") << true
        /* u32 */ << Utils::binary<quint32>("10 01111000 01010110 00110100 000100") << true
        /* u54 */ << Utils::binary<quint64>("11011110 10111100 10011010 01111000 01010110 00110100 000100") << true;

    // now so that the 54bit value fits in exactly, i.e. 10 bits shifted
    // shifted by 10 bytes in little endian:
    // 11110000 11011110 10111100 10011010 01111000 01010110 001101xx xxxxxxxx
    QTest::newRow("1 bytes 2 bits") << 1u << quint8(2) << quint64(54) << qint64(54) << quint8(0) << true
        /* b8 */ << Utils::binary<quint8>("10 001101") << true
        /* i16 */ << Utils::binary<qint16>("00 01010110 001101") << true
        /* u32 */ << Utils::binary<quint32>("00 10011010 01111000 01010110 001101") << true
        /* u54 */ << Utils::binary<quint64>("11110000 11011110 10111100 10011010 01111000 01010110 001101") << true;

    // now make the 54 bit value go past eof (11 bits)
    // shifted by 10 bytes in little endian:
    // 11110000 11011110 10111100 10011010 01111000 01010110 00110xxx xxxxxxxx
    QTest::newRow("1 bytes 3 bits") << 1u << quint8(3) << quint64(53) << qint64(-1) << quint8(3) << false
        /* b8 */ << Utils::binary<quint8>("110 00110") << true
        /* i16 */ << Utils::binary<qint16>("000 01010110 00110") << true
        /* u32 */ << Utils::binary<quint32>("100 10011010 01111000 01010110 00110") << true
        /* u54 */ << Utils::binary<quint64>("0") << false;
}

QTEST_GUILESS_MAIN(UnionDataInformationTest)

#include "uniondatainformationtest.moc"
