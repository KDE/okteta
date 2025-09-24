/*
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

// sut
#include <arraydatainformation.hpp>
#include <primitivearraydata.hpp>
#include <topleveldatainformation.hpp>
#include <primitivetemplateinfo.hpp>
#include <primitivefactory.hpp>
#include <structuredatainformation.hpp>
// Qt
#include <QTest>
#include <QSignalSpy>

class ArrayDataInformationTest : public QObject
{
    Q_OBJECT

private:
    void testResizeHelper(ArrayDataInformation* array, TopLevelDataInformation* top);

private Q_SLOTS:
    void initTestCase();
    void testResize();
    void testResize_data();

private:
    ArrayDataInformation* primitive;
    ArrayDataInformation* complex;
    std::unique_ptr<TopLevelDataInformation> primitiveTop;
    std::unique_ptr<TopLevelDataInformation> complexTop;
    uint primitiveSize;
    uint complexSize;
};

void ArrayDataInformationTest::initTestCase()
{
    qRegisterMetaType<const DataInformation*>();
    qRegisterMetaType<DataInformation*>();
    LoggerWithContext lwc(nullptr, QString());

    auto managedPrimitive = std::make_unique<ArrayDataInformation>(QStringLiteral("primitives"), 0,
                                                                   PrimitiveFactory::newInstance(QStringLiteral("child"), PrimitiveDataType::UInt32, lwc));
    primitive = managedPrimitive.get();
    primitiveSize = 32;
    primitiveTop = std::make_unique<TopLevelDataInformation>(std::move(managedPrimitive));

    QCOMPARE(primitive->isArray(), true);
    QCOMPARE(primitive->isBitfield(), false);
    QCOMPARE(primitive->isDummy(), false);
    QCOMPARE(primitive->isEnum(), false);
    QCOMPARE(primitive->isEnum(), false);
    QCOMPARE(primitive->isString(), false);
    QCOMPARE(primitive->isUnion(), false);
    QCOMPARE(primitive->mainStructure(), primitive);
    QCOMPARE(primitive->childCount(), 0U);
    QCOMPARE(primitive->length(), 0U);
    QCOMPARE(primitive->size(), 0U);
    QCOMPARE(primitive->isTopLevel(), false);
    QCOMPARE(primitiveTop->isTopLevel(), true);
    QCOMPARE(primitive->positionInFile(3), BitCount64(24));
    QCOMPARE(primitive->topLevelDataInformation(), primitiveTop.get());

    std::vector<std::unique_ptr<DataInformation>> structsChildren;
    structsChildren.emplace_back(PrimitiveFactory::newInstance(QStringLiteral("first"), PrimitiveDataType::UInt32, lwc));
    structsChildren.emplace_back(PrimitiveFactory::newInstance(QStringLiteral("second"), PrimitiveDataType::UInt32, lwc));

    auto structs = std::make_unique<StructureDataInformation>(QStringLiteral("vals"), std::move(structsChildren));

    complexSize = 64;
    auto managedComplex = std::make_unique<ArrayDataInformation>(QStringLiteral("complex"), 0, std::move(structs));
    complex = managedComplex.get();
    complexTop = std::make_unique<TopLevelDataInformation>(std::move(managedComplex));

    QCOMPARE(complex->isArray(), true);
    QCOMPARE(complex->isBitfield(), false);
    QCOMPARE(complex->isDummy(), false);
    QCOMPARE(complex->isEnum(), false);
    QCOMPARE(complex->isEnum(), false);
    QCOMPARE(complex->isString(), false);
    QCOMPARE(complex->isUnion(), false);
    QCOMPARE(complex->mainStructure(), complex);
    QCOMPARE(complex->childCount(), 0U);
    QCOMPARE(complex->length(), 0U);
    QCOMPARE(complex->size(), 0U);
    QCOMPARE(complex->isTopLevel(), false);
    QCOMPARE(complexTop->isTopLevel(), true);
    QCOMPARE(complex->positionInFile(3), BitCount64(24));
    QCOMPARE(complex->topLevelDataInformation(), complexTop.get());
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

    std::ignore = array->setArrayLength(newsize);
    QCOMPARE(array->length(), postsize);
    if (insertCalled) {
        QCOMPARE(spyAboutToInsert.size(), 1);
        QCOMPARE(spyInserted.size(), 1);

        QFETCH(uint, insertFirstArg);
        QFETCH(uint, insertSecondArg);

        QCOMPARE(spyAboutToInsert[0][1].toUInt(), insertFirstArg);
        QCOMPARE(spyAboutToInsert[0][2].toUInt(), insertSecondArg);
        QCOMPARE(spyInserted[0][1].toUInt(), insertFirstArg);
        QCOMPARE(spyInserted[0][2].toUInt(), insertSecondArg);
    }
    if (removeCalled) {
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
    testResizeHelper(primitive, primitiveTop.get());
    testResizeHelper(complex, complexTop.get());
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
    QTest::newRow("1. (10)") << 10U << 10U << true << 0U << 9U << false << -1U << -1U;
    QTest::newRow("2. (10)") << 10U << 10U << false << -1U << -1U << false << -1U << -1U;
    QTest::newRow("3. (9)") << 9U << 9U << false << -1U << -1U << true << 9U << 9U;
    QTest::newRow("4. (18)") << 18U << 18U << true << 9U << 17U << false << -1U << -1U;
    QTest::newRow("5. (0)") << 0U << 0U << false << -1U << -1U << true << 0U << 17U;
    QTest::newRow("6. (1)") << 1U << 1U << true << 0U << 0U << false << -1U << -1U;
    QTest::newRow("7. (0)") << 0U << 0U << false << -1U << -1U << true << 0U << 0U;
    QTest::newRow("8. (0)") << 0U << 0U << false << -1U << -1U << false << -1U << -1U;

}

QTEST_GUILESS_MAIN(ArrayDataInformationTest)

#include "arraydatainformationtest.moc"
