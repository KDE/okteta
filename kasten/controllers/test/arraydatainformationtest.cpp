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

#include "view/structures/datatypes/array/arraydatainformation.h"
#include "view/structures/datatypes/array/primitivearraydata.h"
#include "view/structures/datatypes/topleveldatainformation.h"
#include "view/structures/datatypes/primitive/primitivetemplateinfo.h"
#include "view/structures/datatypes/primitivefactory.h"
#include "view/structures/datatypes/structuredatainformation.h"

class ArrayDataInformationTest : public QObject
{
    Q_OBJECT
private:
    void testResizeHelper(ArrayDataInformation* array, TopLevelDataInformation* top);
private Q_SLOTS:
    void initTestCase();
    void testResize();
    void testResize_data();
    void cleanupTestCase();
private:
    ArrayDataInformation* primitive;
    ArrayDataInformation* complex;
    TopLevelDataInformation* primitiveTop;
    TopLevelDataInformation* complexTop;
    uint primitiveSize;
    uint complexSize;
};

void ArrayDataInformationTest::initTestCase()
{
    qRegisterMetaType<const DataInformation*>();
    qRegisterMetaType<DataInformation*>();
    LoggerWithContext lwc(0, QString());

    primitive = new ArrayDataInformation(QLatin1String("primitives"), 0,
            PrimitiveFactory::newInstance(QLatin1String("child"), Type_UInt32, lwc));
    primitiveSize = 32;
    primitiveTop = new TopLevelDataInformation(primitive);

    QCOMPARE(primitive->isArray(), true);
    QCOMPARE(primitive->isBitfield(), false);
    QCOMPARE(primitive->isDummy(), false);
    QCOMPARE(primitive->isEnum(), false);
    QCOMPARE(primitive->isEnum(), false);
    QCOMPARE(primitive->isString(), false);
    QCOMPARE(primitive->isUnion(), false);
    QCOMPARE(primitive->mainStructure(), primitive);
    QCOMPARE(primitive->canHaveChildren(), true);
    QCOMPARE(primitive->childCount(), 0u);
    QCOMPARE(primitive->length(), 0u);
    QCOMPARE(primitive->size(), 0u);
    QCOMPARE(primitive->isTopLevel(), false);
    QCOMPARE(primitiveTop->isTopLevel(), true);
    QCOMPARE(primitive->positionInFile(3), BitCount64(24));
    QCOMPARE(primitive->topLevelDataInformation(), primitiveTop);

    QVector<DataInformation*> structsChildren;
    structsChildren << PrimitiveFactory::newInstance(QLatin1String("first"), Type_UInt32, lwc)
            << PrimitiveFactory::newInstance(QLatin1String("second"), Type_UInt32, lwc);

    StructureDataInformation* structs = new StructureDataInformation(QLatin1String("vals"), structsChildren);

    complexSize = 64;
    complex = new ArrayDataInformation(QLatin1String("complex"), 0, structs);
    complexTop = new TopLevelDataInformation(complex);

    QCOMPARE(complex->isArray(), true);
    QCOMPARE(complex->isBitfield(), false);
    QCOMPARE(complex->isDummy(), false);
    QCOMPARE(complex->isEnum(), false);
    QCOMPARE(complex->isEnum(), false);
    QCOMPARE(complex->isString(), false);
    QCOMPARE(complex->isUnion(), false);
    QCOMPARE(complex->mainStructure(), complex);
    QCOMPARE(complex->canHaveChildren(), true);
    QCOMPARE(complex->childCount(), 0u);
    QCOMPARE(complex->length(), 0u);
    QCOMPARE(complex->size(), 0u);
    QCOMPARE(complex->isTopLevel(), false);
    QCOMPARE(complexTop->isTopLevel(), true);
    QCOMPARE(complex->positionInFile(3), BitCount64(24));
    QCOMPARE(complex->topLevelDataInformation(), complexTop);
}

void ArrayDataInformationTest::testResizeHelper(ArrayDataInformation* array, TopLevelDataInformation* top)
{
    QSignalSpy spyAboutToInsert(top, SIGNAL(childrenAboutToBeInserted(DataInformation*,uint,uint)));
    QSignalSpy spyInserted(top, SIGNAL(childrenInserted(const DataInformation*,uint,uint)));
    QSignalSpy spyAboutToRemove(top, SIGNAL(childrenAboutToBeRemoved(DataInformation*,uint,uint)));
    QSignalSpy spyRemoved(top, SIGNAL(childrenRemoved(const DataInformation*,uint,uint)));

    QFETCH(uint, newsize);
    QFETCH(uint, postsize);
    QFETCH(bool, insertCalled);
    QFETCH(bool, removeCalled);

    array->setArrayLength(newsize);
    QCOMPARE(array->length(), postsize);
    if (insertCalled)
    {
        QCOMPARE(spyAboutToInsert.size(), 1);
        QCOMPARE(spyInserted.size(), 1);

        QFETCH(uint, insertFirstArg);
        QFETCH(uint, insertSecondArg);

        QCOMPARE(spyAboutToInsert[0][1].toUInt(), insertFirstArg);
        QCOMPARE(spyAboutToInsert[0][2].toUInt(), insertSecondArg);
        QCOMPARE(spyInserted[0][1].toUInt(), insertFirstArg);
        QCOMPARE(spyInserted[0][2].toUInt(), insertSecondArg);
    }
    if (removeCalled)
    {
        QCOMPARE(spyAboutToRemove.size(), 1);
        QCOMPARE(spyRemoved.size(), 1);

        QFETCH(uint, removeFirstArg);
        QFETCH(uint, removeSecondArg);

        QCOMPARE(spyAboutToRemove[0][1].toUInt(), removeFirstArg);
        QCOMPARE(spyAboutToRemove[0][2].toUInt(), removeSecondArg);
        QCOMPARE(spyRemoved[0][1].toUInt(), removeFirstArg);
        QCOMPARE(spyRemoved[0][2].toUInt(), removeSecondArg);
    }
}


void ArrayDataInformationTest::testResize()
{
    testResizeHelper(primitive, primitiveTop);
    testResizeHelper(complex, complexTop);
}

void ArrayDataInformationTest::testResize_data()
{
    QTest::addColumn<uint>("newsize");
    QTest::addColumn<uint>("postsize");
    QTest::addColumn<bool>("insertCalled");
    QTest::addColumn<uint>("insertFirstArg");
    QTest::addColumn<uint>("insertSecondArg");
    QTest::addColumn<bool>("removeCalled");
    QTest::addColumn<uint>("removeFirstArg");
    QTest::addColumn<uint>("removeSecondArg");
    QTest::newRow("1. (10)") << 10u << 10u << true << 0u << 9u << false << -1u << -1u;
    QTest::newRow("2. (10)") << 10u << 10u << false << -1u << -1u << false << -1u << -1u;
    QTest::newRow("3. (9)") << 9u << 9u << false << -1u << -1u << true << 9u << 9u;
    QTest::newRow("4. (18)") << 18u << 18u << true << 9u << 17u << false << -1u << -1u;
    QTest::newRow("5. (0)") << 0u << 0u << false << -1u << -1u << true << 0u << 17u;
    QTest::newRow("6. (1)") << 1u << 1u << true << 0u << 0u << false << -1u << -1u;
    QTest::newRow("7. (0)") << 0u << 0u << false << -1u << -1u << true << 0u << 0u;
    QTest::newRow("8. (0)") << 0u << 0u << false << -1u << -1u << false << -1u << -1u;

}

void ArrayDataInformationTest::cleanupTestCase()
{
    delete complexTop;
    delete primitiveTop;
}



QTEST_MAIN(ArrayDataInformationTest)


#include "arraydatainformationtest.moc"
