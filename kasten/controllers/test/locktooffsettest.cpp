/*
 *  This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *  Copyright 2013  Alex Richardson <alex.richardson@gmx.de>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <QtTest/QtTest>

#include <limits>

#include <bytearraymodel.h>
#include "view/structures/datatypes/topleveldatainformation.h"
#include "view/structures/datatypes/structuredatainformation.h"
#include "view/structures/datatypes/primitive/primitivetemplateinfo.h"

class LockToOffsetTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
    void testReadingNecessary_data();
    void testReadingNecessary();
    void cleanupTestCase();
private:
    TopLevelDataInformation* newStructure(Okteta::AbstractByteArrayModel* lastModel, quint64 lastReadOffset);
private:
    Okteta::ByteArrayModel* model;
    Okteta::ByteArrayModel* model2;
};

const unsigned char testData[128] =
{
    0x17, 0xa7, 0x0b, 0xa7, 0x8d, 0x96, 0x15, 0x9e,
    0xc0, 0x75, 0x02, 0x5b, 0xfe, 0xa4, 0xf5, 0x44,
    0x66, 0xbc, 0xb0, 0x1e, 0x84, 0x47, 0x35, 0xd1,
    0xcf, 0x5f, 0x4b, 0xdb, 0x72, 0x2d, 0xe8, 0x89,
    0xd4, 0xf3, 0x31, 0x61, 0x8a, 0x46, 0x00, 0x4a,
    0xbb, 0x02, 0xa5, 0xb9, 0xa6, 0x9a, 0xfe, 0x0c,
    0x56, 0xae, 0x2a, 0xdb, 0xf5, 0x5f, 0xac, 0xc4,
    0xbf, 0xf7, 0x9f, 0x31, 0x25, 0x87, 0xbb, 0xf9,
    0x7b, 0xec, 0x5b, 0x05, 0x32, 0x5b, 0x4f, 0xee,
    0x5d, 0xf5, 0xa7, 0x03, 0x8f, 0xa5, 0x0f, 0xe6,
    0x53, 0x39, 0xc1, 0x48, 0x99, 0x6d, 0x0c, 0x58,
    0x64, 0xac, 0x89, 0x89, 0x33, 0x44, 0x83, 0xae,
    0x30, 0xde, 0xb3, 0x63, 0x39, 0x03, 0x51, 0x96,
    0xf8, 0xf8, 0x99, 0x87, 0x9e, 0xa8, 0x6d, 0xf1,
    0xe1, 0x2e, 0x3a, 0x7a, 0x9b, 0x46, 0xd2, 0x00,
    0xf2, 0x5c, 0x89, 0x26, 0xa0, 0x0c, 0xd4, 0xd1
};

const unsigned char testData2[128] =
{
    0x4c, 0x4a, 0x64, 0x02, 0x98, 0x65, 0xb8, 0x56,
    0x94, 0x57, 0xd8, 0x4b, 0x02, 0x12, 0xc3, 0x65,
    0xad, 0xe2, 0xad, 0x0a, 0x69, 0xbb, 0x73, 0xec,
    0xa7, 0x0a, 0x14, 0x1c, 0xc4, 0xe1, 0x7c, 0x11,
    0x2b, 0xe0, 0x13, 0xc3, 0x45, 0xcc, 0x1a, 0xd9,
    0x23, 0xf2, 0x24, 0x25, 0x04, 0xe7, 0x8b, 0xb1,
    0xc9, 0x38, 0xbb, 0x33, 0xf4, 0x2f, 0x1f, 0x9b,
    0x39, 0x34, 0xb7, 0xfd, 0x15, 0x33, 0x0e, 0x40,
    0x13, 0x22, 0x03, 0x58, 0xee, 0x1d, 0x32, 0x11,
    0x0f, 0x56, 0x36, 0x13, 0x3e, 0xc1, 0xc4, 0x07,
    0xfa, 0x80, 0x3a, 0xee, 0xaf, 0x5a, 0x89, 0xe8,
    0x8e, 0x41, 0xe5, 0xa3, 0x74, 0xf4, 0xe3, 0x88,
    0x16, 0xe6, 0xe0, 0x04, 0x04, 0x12, 0x15, 0x13,
    0x69, 0x4b, 0x27, 0xa7, 0x0d, 0xeb, 0xae, 0x07,
    0x6b, 0xe9, 0xf5, 0x1a, 0x43, 0x7e, 0x02, 0xd1,
    0xbf, 0xe8, 0x74, 0x34, 0xdc, 0x57, 0xbc, 0xf2
};




void LockToOffsetTest::initTestCase()
{
    model = new Okteta::ByteArrayModel(testData, sizeof(testData));
    model->setAutoDelete(false);
    model2 = new Okteta::ByteArrayModel(testData2, sizeof(testData2));
    model2->setAutoDelete(false);
}

void LockToOffsetTest::cleanupTestCase()
{
    delete model;
    delete model2;
}


TopLevelDataInformation* LockToOffsetTest::newStructure(Okteta::AbstractByteArrayModel* lastModel, quint64 lastReadOffset)
{
    QVector<DataInformation*> children;
    children.append(new UInt16DataInformation(QLatin1String("first")));
    children.append(new UInt64DataInformation(QLatin1String("second")));
    DataInformation* data = new StructureDataInformation(QLatin1String("container"), children);
    data->setByteOrder(DataInformation::EndiannessBig);
    TopLevelDataInformation* top = new TopLevelDataInformation(data);
    top->mLastModel = lastModel;
    if (lastModel)
        top->newModelActivated(lastModel);
    top->mLastReadOffset = lastReadOffset;
    return top;
}

Q_DECLARE_METATYPE(Okteta::ArrayChangeMetricsList)
Q_DECLARE_METATYPE(Okteta::AbstractByteArrayModel*)
Q_DECLARE_METATYPE(TopLevelDataInformation*)

static inline void addRow(const char* tag, TopLevelDataInformation* structure, Okteta::Address addr,
        Okteta::AbstractByteArrayModel* model, Okteta::ArrayChangeMetricsList changes, bool expected)
{
    QTest::newRow(tag) << structure << addr << model << changes << expected;
}

static Okteta::ArrayChangeMetricsList oneReplacement(int start, int length, int replacementSize)
{
    Okteta::ArrayChangeMetricsList ret;
    ret << Okteta::ArrayChangeMetrics::asReplacement(start, length, replacementSize);
    return ret;
}



void LockToOffsetTest::testReadingNecessary_data()
{
    QTest::addColumn<TopLevelDataInformation*>("structure");
    QTest::addColumn<Okteta::Address>("address");
    QTest::addColumn<Okteta::AbstractByteArrayModel*>("model");
    QTest::addColumn<Okteta::ArrayChangeMetricsList>("changes");
    QTest::addColumn<bool>("expected");
    Okteta::ArrayChangeMetricsList noChanges;

    TopLevelDataInformation* top = newStructure(0, 5);
    addRow("new model, same offset, no changes", top, 5, model, noChanges, true);

    top = newStructure(model, 5);
    addRow("same model, different offset, no changes", top, 6, model, noChanges, true);

    top = newStructure(model, 5);
    addRow("same model, same offset, no changes", top, 5, model, noChanges, false);

    top = newStructure(model, 5);
    top->lockPositionToOffset(5, model);
    addRow("same model, same offset (because of locking), no changes", top, 50, model, noChanges, false);

    top = newStructure(model, 5);
    top->newModelActivated(model2);
    addRow("different model, same offset, no changes", top, 5, model2, noChanges, true);

    top = newStructure(model, TopLevelDataInformation::INVALID_OFFSET);
    addRow("same model, invalid offset before, no changes", top, 5, model2, noChanges, true);


    top = newStructure(model, 5);
    addRow("same model, same offset, changes before", top, 5, model, oneReplacement(0, 4, 4), false);

    top = newStructure(model, 5);
    addRow("same model, same offset, changes before (right until start)", top, 5, model, oneReplacement(0, 5, 5), false);


    top = newStructure(model, 5);
    addRow("same model, same offset, changes before (1 byte removed!)", top, 5, model, oneReplacement(0, 5, 4), true);

    top = newStructure(model, 5);
    addRow("same model, same offset, changes before (1 byte added!)", top, 5, model, oneReplacement(0, 1, 2), true);

    top = newStructure(model, 5);
    addRow("same model, same offset, changes 2 bytes after (1 byte removed!)", top, 5, model, oneReplacement(16, 5, 4), false);
    top = newStructure(model, 5);
    addRow("same model, same offset, changes 1 byte after (1 byte removed!)", top, 5, model, oneReplacement(15, 5, 4), false);

    top = newStructure(model, 5);
    addRow("same model, same offset, changes 2 bytes after (1 byte added!)", top, 5, model, oneReplacement(16, 1, 2), false);
    top = newStructure(model, 5);
    addRow("same model, same offset, changes 1 byte after (1 byte added!)", top, 5, model, oneReplacement(15, 1, 2), false);

}

void LockToOffsetTest::testReadingNecessary()
{
    QFETCH(Okteta::AbstractByteArrayModel*, model);
    QFETCH(TopLevelDataInformation*, structure);
    QFETCH(Okteta::ArrayChangeMetricsList, changes);
    QFETCH(Okteta::Address, address);
    QFETCH(bool, expected);
    if (!structure->mLockedPositions.contains(model))
        structure->newModelActivated(model); //add the model, otherwise we crash
    QCOMPARE(structure->isReadingNecessary(model, address, changes), expected);
    structure->read(model, address, changes, false);
    //no changes after read -> no reading necessary
    QVERIFY(!structure->isReadingNecessary(model, address, Okteta::ArrayChangeMetricsList()));
    delete structure;
}

QTEST_MAIN(LockToOffsetTest)

#include "locktooffsettest.moc"
