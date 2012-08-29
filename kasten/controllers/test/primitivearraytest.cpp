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
#include <limits>

#include <bytearraymodel.h>

#include "view/structures/datatypes/array/arraydatainformation.h"
#include "view/structures/datatypes/array/primitivearraydata.h"
#include "view/structures/datatypes/topleveldatainformation.h"
#include "view/structures/datatypes/primitive/primitivetemplateinfo.h"
#include "view/structures/datatypes/primitivefactory.h"
#include "view/structures/script/scriptengineinitializer.h"

class PrimitiveArrayTest : public QObject
{
Q_OBJECT

private:
    template<PrimitiveDataTypeEnum primType, typename T> void testReadPrimitiveInternal();
    template<PrimitiveDataTypeEnum primType> void testReadPrimitive();
    template<typename T> bool compareItems(T first, T second, uint index);

private Q_SLOTS:
    void initTestCase();
    void testReadFloat();
    void testReadDouble();
    void testReadChar();
    void testReadInt8();
    void testReadInt16();
    void testReadInt32();
    void testReadInt64();
    void testReadUInt8();
    void testReadUInt16();
    void testReadUInt32();
    void testReadUInt64();
    void testReadBool8();
    void testReadBool16();
    void testReadBool32();
    void testReadBool64();

private:
    QScopedArrayPointer<Okteta::Byte> data;
    QScopedPointer<Okteta::ByteArrayModel> model;
};

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
#define CURRENT_BYTE_ORDER (DataInformation::EndianessLittle)
#elif Q_BYTE_ORDER == Q_BIG_ENDIAN
#define CURRENT_BYTE_ORDER (DataInformation::EndianessBig)
#else
#error unknown byte order
#endif

static const uint SIZE = 8192;

void PrimitiveArrayTest::initTestCase()
{
    qsrand(QTime::currentTime().msec());
    data.reset(new Okteta::Byte[SIZE]);
    //ensure that we have at least one NaN (quiet + signalling)
    AllPrimitiveTypes quietDouble(std::numeric_limits<double>::quiet_NaN());
    AllPrimitiveTypes signallingDouble(std::numeric_limits<double>::signaling_NaN());
    for (int i = 0; i < 8; ++i)
    {
        data[i] = quietDouble.allBytes[i];
        data[8 + i] = signallingDouble.allBytes[i];
    }
    AllPrimitiveTypes quietFloat(std::numeric_limits<float>::quiet_NaN());
    AllPrimitiveTypes signallingFloat(std::numeric_limits<float>::signaling_NaN());
    for (int i = 0; i < 4; ++i)
    {
        data[16 + i] = quietFloat.allBytes[i];
        data[20 + i] = signallingFloat.allBytes[i];
    }
    for (uint i = 24; i < SIZE; ++i)
    {
        data[i] = (char(qrand() & 0xff));
    }
    Okteta::Byte* copy = new Okteta::Byte[SIZE];
    memcpy(copy, data.data(), SIZE);
    model.reset(new Okteta::ByteArrayModel(copy, SIZE));
    model->setAutoDelete(true);
    QCOMPARE(model->size(), Okteta::Size(SIZE));
}

template<typename T>
bool PrimitiveArrayTest::compareItems(T first, T second, uint index)
{
    Q_UNUSED(index);
    return first == second;
}

template<>
bool PrimitiveArrayTest::compareItems(float first, float second, uint index)
{
    if (first != first)
    {
        qDebug() << "first was NaN at index" << index;
        //NaN
        if (second != second)
        {
            //second is NaN too;
            union
            {
                float floating;
                QIntegerForSizeof<float>::Unsigned integer;
            } firstUn, secondUn;
            firstUn.floating = first;
            secondUn.floating = second;
            return firstUn.integer == secondUn.integer;
        }
        else
        {
            return false;
        }
    }
    return first == second;
}

template<>
bool PrimitiveArrayTest::compareItems(double first, double second, uint index)
{
    if (first != first)
    {
        qDebug() << "first was NaN at index" << index;
        //NaN
        if (second != second)
        {
            //second is NaN too;
            union
            {
                double floating;
                QIntegerForSizeof<double>::Unsigned integer;
            } firstUn, secondUn;
            firstUn.floating = first;
            secondUn.floating = second;
            return firstUn.integer == secondUn.integer;
        }
        else
        {
            return false;
        }
    }
    return first == second;
}

template<PrimitiveDataTypeEnum primType>
inline void PrimitiveArrayTest::testReadPrimitive()
{
    testReadPrimitiveInternal<primType, typename PrimitiveInfo<primType>::valueType>();
}

template<PrimitiveDataTypeEnum primType, typename T>
void PrimitiveArrayTest::testReadPrimitiveInternal()
{
    LoggerWithContext lwc(0, QString());
    ArrayDataInformation* dataInf = new ArrayDataInformation(QLatin1String("values"),
            model->size() / sizeof(T),
            PrimitiveFactory::newInstance(QLatin1String("value"), primType, lwc));
    dataInf->setByteOrder(CURRENT_BYTE_ORDER);
    QScopedPointer<TopLevelDataInformation> top(new TopLevelDataInformation(dataInf, 0,
            ScriptEngineInitializer::newEngine()));
    QCOMPARE(dataInf->childCount(), uint(SIZE / sizeof(T)));
    quint8 bitOffs = 0;
    qint64 result = dataInf->readData(model.data(), 0, model->size() * 8, &bitOffs);
    QCOMPARE(Okteta::Size(result), model->size() * 8);
    T* dataAsT = reinterpret_cast<T*>(data.data());
    PrimitiveArrayData<primType>* arrayData =
            dynamic_cast<PrimitiveArrayData<primType>*>(dataInf->mData.data());
    QVERIFY(arrayData);
    for (uint i = 0; i < dataInf->childCount(); ++i)
    {
        AllPrimitiveTypes childDataAll = arrayData->valueAt(i);
        T childData = childDataAll.value<T>();
        T expected = dataAsT[i];
        //TODO comparison for float and double: nan != nan
        if (!compareItems<T>(childData, expected, i))
        {
            QByteArray desc = "i=" + QByteArray::number(i) + ", model[i]="
                    + QByteArray::number(childData)
                    + ", data[i]=" + QByteArray::number(expected);
            QVERIFY2(compareItems<T>(childData, expected, i), desc.constData());
        }
    }
}

void PrimitiveArrayTest::testReadFloat()
{
    testReadPrimitive<Type_Float>();
}

void PrimitiveArrayTest::testReadDouble()
{
    testReadPrimitive<Type_Double>();
}

void PrimitiveArrayTest::testReadChar()
{
    testReadPrimitive<Type_Char>();
}

void PrimitiveArrayTest::testReadInt8()
{
    testReadPrimitive<Type_Int8>();
}

void PrimitiveArrayTest::testReadInt16()
{
    testReadPrimitive<Type_Int16>();
}

void PrimitiveArrayTest::testReadInt32()
{
    testReadPrimitive<Type_Int32>();
}

void PrimitiveArrayTest::testReadInt64()
{
    testReadPrimitive<Type_Int64>();
}

void PrimitiveArrayTest::testReadUInt8()
{
    testReadPrimitive<Type_UInt8>();
}

void PrimitiveArrayTest::testReadUInt16()
{
    testReadPrimitive<Type_UInt16>();
}

void PrimitiveArrayTest::testReadUInt32()
{
    testReadPrimitive<Type_UInt32>();
}

void PrimitiveArrayTest::testReadUInt64()
{
    testReadPrimitive<Type_UInt64>();
}

void PrimitiveArrayTest::testReadBool8()
{
    testReadPrimitive<Type_Bool8>();
}

void PrimitiveArrayTest::testReadBool16()
{
    testReadPrimitive<Type_Bool16>();
}

void PrimitiveArrayTest::testReadBool32()
{
    testReadPrimitive<Type_Bool32>();
}

void PrimitiveArrayTest::testReadBool64()
{
    testReadPrimitive<Type_Bool64>();
}

QTEST_MAIN(PrimitiveArrayTest)

#include "primitivearraytest.moc"
