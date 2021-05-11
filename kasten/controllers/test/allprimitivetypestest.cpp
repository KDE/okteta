/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "view/structures/allprimitivetypes.hpp"
// Okteta
#include <Okteta/ByteArrayModel>
// Qt
#include <QTest>
// Std
#include <memory>

class AllPrimitiveTypesTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void readLittleEndian();
    void readBigEndian();
    void testValuesMatch();

private:
    template <typename T> inline T readLittle(quint8 bitOffset)
    {
        return AllPrimitiveTypes::readValue<T>(model.get(), 0, QSysInfo::LittleEndian, bitOffset);
    }
    template <typename T> inline T readBig(quint8 bitOffset)
    {
        return AllPrimitiveTypes::readValue<T>(model.get(), 0, QSysInfo::BigEndian, bitOffset);
    }

private:
    std::unique_ptr<Okteta::ByteArrayModel> model;
};

static constexpr Okteta::Byte data[9] =
{ 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x00 };

inline void AllPrimitiveTypesTest::initTestCase()
{
    auto* copy = new Okteta::Byte[sizeof(data)];
    memcpy(copy, data, sizeof(data));
    model.reset(new Okteta::ByteArrayModel(copy, sizeof(data)));
    model->setAutoDelete(true);
    QCOMPARE(model->size(), Okteta::Size(sizeof(data)));
}

void AllPrimitiveTypesTest::testValuesMatch()
{
    QCOMPARE((int)sizeof(AllPrimitiveTypes), 8);
    AllPrimitiveTypes test = Q_UINT64_C(0x1234567890abcdef);
    QCOMPARE(test.value<qint8>(), qint8(0xef));
    QCOMPARE(test.value<quint8>(), quint8(0xef));
    QCOMPARE(test.value<qint16>(), qint16(0xcdef));
    QCOMPARE(test.value<quint16>(), quint16(0xcdef));
    QCOMPARE(test.value<qint32>(), qint32(0x90abcdef));
    QCOMPARE(test.value<quint32>(), quint32(0x90abcdef));
    QCOMPARE(test.value<qint64>(), Q_INT64_C(0x1234567890abcdef));
    QCOMPARE(test.value<quint64>(), Q_UINT64_C(0x1234567890abcdef));
    AllPrimitiveTypes test2 = Q_INT64_C(-0x1234567890abcdef);
    QVERIFY(test2 == Q_UINT64_C(0xedcba9876f543211));
}

inline void AllPrimitiveTypesTest::readLittleEndian()
{
    QCOMPARE(readLittle<quint8>(0), quint8(0x12U));
    QCOMPARE(readLittle<quint16>(0), quint16(0x3412U));
    QCOMPARE(readLittle<quint32>(0), quint32(0x78563412U));
    QCOMPARE(readLittle<quint64>(0), Q_UINT64_C(0xf0debc9a78563412));
    // shifted by 4
    // this is the way gcc handles it on little endian systems
    QCOMPARE(readLittle<quint8>(4), quint8(0x41U));
    QCOMPARE(readLittle<quint16>(4), quint16(0x6341U));
    QCOMPARE(readLittle<quint32>(4), quint32(0xa7856341U));
    QCOMPARE(readLittle<quint64>(4), Q_UINT64_C(0x0f0debc9a7856341));

    // now read a quint16 with bitOffset 1 to 7
    QCOMPARE(readLittle<quint16>(1), quint16(0x1a09U));
    QCOMPARE(readLittle<quint16>(2), quint16(0x8d04U));
    QCOMPARE(readLittle<quint16>(3), quint16(0xc682U));
    QCOMPARE(readLittle<quint16>(4), quint16(0x6341U));
    QCOMPARE(readLittle<quint16>(5), quint16(0xb1a0U));
    QCOMPARE(readLittle<quint16>(6), quint16(0x58d0U));
    QCOMPARE(readLittle<quint16>(7), quint16(0xac68U));

}

/**
 * 7 6 5 4 | 3 2 1 0 | 7 6 5 4 | 3 2 1 0 | 7 6 5 4 | 3 2 1 0 |
 * ------------------|-------------------|-------------------|
 * 0 0 0 1 | 0 0 1 0 | 0 0 1 1 | 0 1 0 0 | 0 1 0 1 | 0 1 1 0 |
 */

inline void AllPrimitiveTypesTest::readBigEndian()
{
    QCOMPARE(readBig<quint8>(0), quint8(0x12U));
    QCOMPARE(readBig<quint16>(0), quint16(0x1234U));
    QCOMPARE(readBig<quint32>(0), quint32(0x12345678U));
    QCOMPARE(readBig<quint64>(0), Q_UINT64_C(0x123456789abcdef0));

    QCOMPARE(readBig<quint8>(4), quint8(0x23U));
    QCOMPARE(readBig<quint16>(4), quint16(0x2345U));

    QCOMPARE(readBig<quint16>(1), quint16(0x2468U));
    QCOMPARE(readBig<quint16>(2), quint16(0x48d1U));
    QCOMPARE(readBig<quint16>(3), quint16(0x91a2U));
    QCOMPARE(readBig<quint16>(4), quint16(0x2345U));
    QCOMPARE(readBig<quint16>(5), quint16(0x468aU));
    QCOMPARE(readBig<quint16>(6), quint16(0x8d15U));
    QCOMPARE(readBig<quint16>(7), quint16(0x1a2bU));

}

QTEST_GUILESS_MAIN(AllPrimitiveTypesTest)

#include "allprimitivetypestest.moc"
