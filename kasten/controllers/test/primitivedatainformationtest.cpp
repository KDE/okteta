/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    Copyright 2012  Alex Richardson <alex.richardson@gmx.de>
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <QtTest>
#include <QtCore/QDate>
#include <qglobal.h>

#include <bytearraymodel.h>

#include "view/structures/datatypes/primitive/primitivetemplateinfo.h"
#include "view/structures/datatypes/primitive/primitivedatainformation.h"


class PrimitiveDataInformationTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testValueStringFloat();
    void testValueStringDouble();
    void testValueStringChar();
    void testValueStringInt();
    void testValueStringInt_data();
    void testValueStringUInt();
    void testValueStringBool();
private:
//     Okteta::Byte* data;
//     QScopedPointer<Okteta::ByteArrayModel> model;
};

Q_DECLARE_METATYPE(const char*);

void PrimitiveDataInformationTest::initTestCase()
{
    Kasten2::StructViewPreferences::setLocaleAwareDecimalFormatting(false); //this could mess with our expected results
    Kasten2::StructViewPreferences::setLocaleAwareFloatFormatting(false); //this could mess with our expected results
}

void PrimitiveDataInformationTest::testValueStringInt()
{
    QFETCH(qint64, value);
    QFETCH(int, minSize);
    QFETCH(QString, binExpected);
    QFETCH(QString, hexExpected);
    QFETCH(QString, decExpected);
    QFETCH(QString, octExpected);

    QVERIFY(minSize == 1 || minSize == 2 || minSize == 4 || minSize == 8);

    if (minSize <= 1)
    {
        QCOMPARE(PrimitiveInfo<Type_Int8>::Class::valueString(qint8(value), 2), binExpected);
        QCOMPARE(PrimitiveInfo<Type_Int8>::Class::valueString(qint8(value), 16), hexExpected);
        QCOMPARE(PrimitiveInfo<Type_Int8>::Class::valueString(qint8(value), 10), decExpected);
        QCOMPARE(PrimitiveInfo<Type_Int8>::Class::valueString(qint8(value), 8), octExpected);
    }
    if (minSize <= 2)
    {
        QCOMPARE(PrimitiveInfo<Type_Int16>::Class::valueString(qint16(value), 2), binExpected);
        QCOMPARE(PrimitiveInfo<Type_Int16>::Class::valueString(qint16(value), 16), hexExpected);
        QCOMPARE(PrimitiveInfo<Type_Int16>::Class::valueString(qint16(value), 10), decExpected);
        QCOMPARE(PrimitiveInfo<Type_Int16>::Class::valueString(qint16(value), 8), octExpected);
    }
    if (minSize <= 4)
    {
        QCOMPARE(PrimitiveInfo<Type_Int32>::Class::valueString(qint32(value), 2), binExpected);
        QCOMPARE(PrimitiveInfo<Type_Int32>::Class::valueString(qint32(value), 16), hexExpected);
        QCOMPARE(PrimitiveInfo<Type_Int32>::Class::valueString(qint32(value), 10), decExpected);
        QCOMPARE(PrimitiveInfo<Type_Int32>::Class::valueString(qint32(value), 8), octExpected);
    }
    if (minSize <= 8)
    {
        QCOMPARE(PrimitiveInfo<Type_Int64>::Class::valueString(qint64(value), 2), binExpected);
        QCOMPARE(PrimitiveInfo<Type_Int64>::Class::valueString(qint64(value), 16), hexExpected);
        QCOMPARE(PrimitiveInfo<Type_Int64>::Class::valueString(qint64(value), 10), decExpected);
        QCOMPARE(PrimitiveInfo<Type_Int64>::Class::valueString(qint64(value), 8), octExpected);
    }
}

void PrimitiveDataInformationTest::testValueStringInt_data() {
    QTest::addColumn<int>("minSize");
    QTest::addColumn<qint64>("value");
    QTest::addColumn<QString>("binExpected");
    QTest::addColumn<QString>("hexExpected");
    QTest::addColumn<QString>("decExpected");
    QTest::addColumn<QString>("octExpected");

    QTest::newRow("0") << 1 << qint64(0) << "0b0" << "0x0" << "0" << "0o0";
    QTest::newRow("32") << 1 << qint64(32) << "0b100000" << "0x20" << "32" << "0o40";
    QTest::newRow("79") << 1 << qint64(79) << "0b1001111" << "0x4f" << "79" << "0o117";
    QTest::newRow("172") << 2 << qint64(172) << "0b10101100" //8 chars -> no space
        << "0xac" << "172" << "0o254";
    QTest::newRow("259") << 2 << qint64(259) << "0b1 00000011" //9 chars -> space
        << "0x103" << "259" << "0o403";
    QTest::newRow("50448") << 4 << qint64(50448) << "0b11000101 00010000" //16 chars -> no space
        << "0xc510" << "50448" << "0o142420";
    QTest::newRow("126832") << 4 << qint64(126832) << "0b1 11101111 01110000" //17 chars -> space
        << "0x1ef70" << "126832" << "0o367560";
    //maximum
    QTest::newRow("qint8::max()") << 1 << qint64(std::numeric_limits<qint8>::max())
        << "0b1111111" << "0x7f" << "127" << "0o177";
    QTest::newRow("qint16::max()") << 2 << qint64(std::numeric_limits<qint16>::max())
        << "0b1111111 11111111" << "0x7fff" << "32767" << "0o77777";
    QTest::newRow("qint32::max()") << 4 << qint64(std::numeric_limits<qint32>::max())
        << "0b1111111 11111111 11111111 11111111" << "0x7fffffff" << "2147483647" << "0o177 77777777";
    QTest::newRow("qint64::max()") << 8 << qint64(std::numeric_limits<qint64>::max())
        << "0b1111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111"
        << "0x7fffffff ffffffff" << "9223372036854775807" << "0o77777 77777777 77777777";
    //negative
    QTest::newRow("-32") << 1 << qint64(-32) << "-0b100000" << "-0x20" << "-32" << "-0o40";
    QTest::newRow("-79") << 1 << qint64(-79) << "-0b1001111" << "-0x4f" << "-79" << "-0o117";
    QTest::newRow("-172") << 2 << qint64(-172) << "-0b10101100" << "-0xac" << "-172" << "-0o254";
    QTest::newRow("-259") << 2 << qint64(-259) << "-0b1 00000011" << "-0x103" << "-259" << "-0o403";;
    QTest::newRow("-50448") << 4 << qint64(-50448) << "-0b11000101 00010000"
        << "-0xc510" << "-50448" << "-0o142420";
    QTest::newRow("-126832") << 4 << qint64(-126832) << "-0b1 11101111 01110000"
        << "-0x1ef70" << "-126832" << "-0o367560";
    // -1 * postive maximum
    QTest::newRow("- qint8::max()") << 1 << qint64(-std::numeric_limits<qint8>::max())
        << "-0b1111111" << "-0x7f" << "-127" << "-0o177";
    QTest::newRow("- qint16::max()") << 2 << qint64(-std::numeric_limits<qint16>::max())
        << "-0b1111111 11111111" << "-0x7fff" << "-32767" << "-0o77777";;
    QTest::newRow("- qint32::max()") << 4 << qint64(-std::numeric_limits<qint32>::max())
        << "-0b1111111 11111111 11111111 11111111" << "-0x7fffffff" << "-2147483647" << "-0o177 77777777";
    QTest::newRow("- qint64::max()") << 8 << qint64(-std::numeric_limits<qint64>::max())
        << "-0b1111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111"
        << "-0x7fffffff ffffffff" << "-9223372036854775807" << "-0o77777 77777777 77777777";

    //negative minimum
    QTest::newRow("qint8::min()") << 1 << qint64(std::numeric_limits<qint8>::min())
        << "-0b10000000" << "-0x80" << "-128" << "-0o200";
    QTest::newRow("qint16::min()") << 2 << qint64(std::numeric_limits<qint16>::min())
        << "-0b10000000 00000000"  << "-0x8000" << "-32768" << "-0o100000";
    QTest::newRow("qint32::min()") << 4 << qint64(std::numeric_limits<qint32>::min())
        << "-0b10000000 00000000 00000000 00000000"  << "-0x80000000" << "-2147483648" << "-0o200 00000000";
    QTest::newRow("qint64::min()") << 8 << qint64(std::numeric_limits<qint64>::min())
        << "-0b10000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000"
        << "-0x80000000 00000000" << "-9223372036854775808" << "-0o100000 00000000 00000000";
}


void PrimitiveDataInformationTest::testValueStringUInt()
{

}

void PrimitiveDataInformationTest::testValueStringBool()
{

}

void PrimitiveDataInformationTest::testValueStringChar()
{

}

void PrimitiveDataInformationTest::testValueStringDouble()
{

}

void PrimitiveDataInformationTest::testValueStringFloat()
{

}




QTEST_MAIN(PrimitiveDataInformationTest)


#include "primitivedatainformationtest.moc"
