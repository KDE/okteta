/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *    SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include <QTest>
#include <QScriptEngine>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
#include <QRandomGenerator>
#endif
#include <limits>

#include <Okteta/ByteArrayModel>

#include "view/structures/datatypes/array/arraydatainformation.hpp"
#include "view/structures/datatypes/array/primitivearraydata.hpp"
#include "view/structures/datatypes/topleveldatainformation.hpp"
#include "view/structures/datatypes/primitive/primitivetemplateinfo.hpp"
#include "view/structures/datatypes/primitivefactory.hpp"
#include "view/structures/script/scriptengineinitializer.hpp"

class PrimitiveArrayTest : public QObject
{
    Q_OBJECT

private:
    /** Tests user defined overrides of byteOrder, typeName, and toStringFunc. */
    template <PrimitiveDataType primType, typename T> void testReadCustomizedPrimitiveInternal();
    template <PrimitiveDataType primType, typename T> void testReadPrimitiveInternal();
    template <PrimitiveDataType primType> void testReadPrimitive();
    template <typename T> bool compareItems(T first, T second, uint index);

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
    QScopedArrayPointer<Okteta::Byte> endianData;
    QScopedPointer<Okteta::ByteArrayModel> endianModel;
};

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
#define CURRENT_BYTE_ORDER (DataInformation::DataInformationEndianess::EndianessLittle)
#define NON_CURRENT_BYTE_ORDER (DataInformation::DataInformationEndianess::EndianessBig)
#elif Q_BYTE_ORDER == Q_BIG_ENDIAN
#define CURRENT_BYTE_ORDER (DataInformation::DataInformationEndianess::EndianessBig)
#define NON_CURRENT_BYTE_ORDER (DataInformation::DataInformationEndianess::EndianessLittle)
#else
#error unknown byte order
#endif

static constexpr uint SIZE = 8192;
static constexpr uint ENDIAN_SIZE = 16;

void PrimitiveArrayTest::initTestCase()
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
    qsrand(QTime::currentTime().msec());
#endif
    data.reset(new Okteta::Byte[SIZE]);
    // ensure that we have at least one NaN (quiet + signalling)
    AllPrimitiveTypes quietDouble(std::numeric_limits<double>::quiet_NaN());
    AllPrimitiveTypes signallingDouble(std::numeric_limits<double>::signaling_NaN());
    for (int i = 0; i < 8; ++i) {
        data[i] = quietDouble.allBytes[i];
        data[8 + i] = signallingDouble.allBytes[i];
    }

    AllPrimitiveTypes quietFloat(std::numeric_limits<float>::quiet_NaN());
    AllPrimitiveTypes signallingFloat(std::numeric_limits<float>::signaling_NaN());
    for (int i = 0; i < 4; ++i) {
        data[16 + i] = quietFloat.allBytes[i];
        data[20 + i] = signallingFloat.allBytes[i];
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    auto* randomGenerator = QRandomGenerator::global();
#endif
    for (uint i = 24; i < SIZE; ++i) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        data[i] = static_cast<Okteta::Byte>(randomGenerator->bounded(256));
#else
        data[i] = char(qrand() & 0xff);
#endif
    }

    auto* copy = new Okteta::Byte[SIZE];
    memcpy(copy, data.data(), SIZE);
    model.reset(new Okteta::ByteArrayModel(copy, SIZE));
    model->setAutoDelete(true);
    QCOMPARE(model->size(), Okteta::Size(SIZE));

    endianData.reset(new Okteta::Byte[ENDIAN_SIZE]);
    for (uint i = 0; i < ENDIAN_SIZE; ++i) {
        endianData[i] = i;
    }

    auto* endianCopy = new Okteta::Byte[SIZE];
    memcpy(endianCopy, endianData.data(), ENDIAN_SIZE);
    endianModel.reset(new Okteta::ByteArrayModel(endianCopy, ENDIAN_SIZE));
    endianModel->setAutoDelete(true);
    QCOMPARE(endianModel->size(), Okteta::Size(ENDIAN_SIZE));
}

template <typename T>
bool PrimitiveArrayTest::compareItems(T first, T second, uint index)
{
    Q_UNUSED(index);
    return first == second;
}

template <>
bool PrimitiveArrayTest::compareItems(float first, float second, uint index)
{
    if (first != first) {
        qDebug() << "first was NaN at index" << index;
        // NaN
        if (second != second) {
            // second is NaN too;
            union
            {
                float floating;
                QIntegerForSizeof<float>::Unsigned integer;
            } firstUn, secondUn;
            firstUn.floating = first;
            secondUn.floating = second;
            return firstUn.integer == secondUn.integer;
        }

        return false;
    }
    return first == second;
}

template <>
bool PrimitiveArrayTest::compareItems(double first, double second, uint index)
{
    if (first != first) {
        qDebug() << "first was NaN at index" << index;
        // NaN
        if (second != second) {
            // second is NaN too;
            union
            {
                double floating;
                QIntegerForSizeof<double>::Unsigned integer;
            } firstUn, secondUn;
            firstUn.floating = first;
            secondUn.floating = second;
            return firstUn.integer == secondUn.integer;
        }

        return false;
    }
    return first == second;
}

template <PrimitiveDataType primType>
inline void PrimitiveArrayTest::testReadPrimitive()
{
    testReadPrimitiveInternal<primType, typename PrimitiveInfo<primType>::valueType>();
    testReadCustomizedPrimitiveInternal<primType, typename PrimitiveInfo<primType>::valueType>();
}

QScriptValue customToStringFunc(QScriptContext* context, QScriptEngine* engine)
{
    Q_UNUSED(context);
    Q_UNUSED(engine);
    return QStringLiteral("myvalue");
}

template <PrimitiveDataType primType, typename T>
void PrimitiveArrayTest::testReadCustomizedPrimitiveInternal()
{
    LoggerWithContext lwc(nullptr, QString());
    QScriptEngine* engine = ScriptEngineInitializer::newEngine();

    PrimitiveDataInformation* primInfo(PrimitiveFactory::newInstance(QStringLiteral("value"), primType, lwc));
    primInfo->setByteOrder(NON_CURRENT_BYTE_ORDER);
    primInfo->setCustomTypeName(QStringLiteral("mytype"));
    primInfo->setToStringFunction(engine->newFunction(customToStringFunc));

    auto* dataInf = new ArrayDataInformation(QStringLiteral("values"),
                                             endianModel->size() / sizeof(T),
                                             primInfo);
    QScopedPointer<TopLevelDataInformation> top(new TopLevelDataInformation(dataInf, nullptr, engine));

    QCOMPARE(dataInf->childCount(), uint(ENDIAN_SIZE / sizeof(T)));
    quint8 bitOffs = 0;
    qint64 result = dataInf->readData(endianModel.data(), 0, endianModel->size() * 8, &bitOffs);
    QCOMPARE(Okteta::Size(result), endianModel->size() * 8);
    T* dataAsT = reinterpret_cast<T*>(endianData.data());
    QVERIFY(!dataInf->mData->isComplex());
    auto* arrayData = static_cast<PrimitiveArrayData<primType>*>(dataInf->mData.data());

    // Verify byteOrder of values. The data is set up without palindromes.
    if (sizeof(T) > 1) {
        for (uint i = 0; i < dataInf->childCount(); ++i) {
            AllPrimitiveTypes childDataAll = arrayData->valueAt(i);
            T childData = childDataAll.value<T>();
            T expected = dataAsT[i];
            // TODO comparison for float and double: nan != nan
            if (compareItems<T>(childData, expected, i)) {
                QByteArray desc = "i=" + QByteArray::number(i) + ", model[i]="
                                  + QByteArray::number(childData)
                                  + ", data[i]=" + QByteArray::number(expected);
                QVERIFY2(!compareItems<T>(childData, expected, i), desc.constData());
            }
        }
    }
    // Verify typeName as the user will see it.
    QCOMPARE(arrayData->dataAt(0, DataInformation::ColumnType, Qt::DisplayRole).toString(),
             QStringLiteral("mytype"));
    // Verify value string as the user will see it.
    QCOMPARE(arrayData->dataAt(0, DataInformation::ColumnValue, Qt::DisplayRole).toString(),
             QStringLiteral("myvalue"));
}

template <PrimitiveDataType primType, typename T>
void PrimitiveArrayTest::testReadPrimitiveInternal()
{
    LoggerWithContext lwc(nullptr, QString());
    auto* dataInf = new ArrayDataInformation(QStringLiteral("values"),
                                                             model->size() / sizeof(T),
                                                             PrimitiveFactory::newInstance(QStringLiteral("value"), primType, lwc));
    dataInf->setByteOrder(CURRENT_BYTE_ORDER);
    QScopedPointer<TopLevelDataInformation> top(new TopLevelDataInformation(dataInf, nullptr,
                                                                            ScriptEngineInitializer::newEngine()));
    QCOMPARE(dataInf->childCount(), uint(SIZE / sizeof(T)));
    quint8 bitOffs = 0;
    qint64 result = dataInf->readData(model.data(), 0, model->size() * 8, &bitOffs);
    QCOMPARE(Okteta::Size(result), model->size() * 8);
    T* dataAsT = reinterpret_cast<T*>(data.data());
    QVERIFY(!dataInf->mData->isComplex());
    auto* arrayData = static_cast<PrimitiveArrayData<primType>*>(dataInf->mData.data());
    for (uint i = 0; i < dataInf->childCount(); ++i) {
        AllPrimitiveTypes childDataAll = arrayData->valueAt(i);
        T childData = childDataAll.value<T>();
        T expected = dataAsT[i];
        // TODO comparison for float and double: nan != nan
        if (!compareItems<T>(childData, expected, i)) {
            QByteArray desc = "i=" + QByteArray::number(i) + ", model[i]="
                              + QByteArray::number(childData)
                              + ", data[i]=" + QByteArray::number(expected);
            QVERIFY2(compareItems<T>(childData, expected, i), desc.constData());
        }
    }
}

void PrimitiveArrayTest::testReadFloat()
{
    testReadPrimitive<PrimitiveDataType::Float>();
}

void PrimitiveArrayTest::testReadDouble()
{
    testReadPrimitive<PrimitiveDataType::Double>();
}

void PrimitiveArrayTest::testReadChar()
{
    testReadPrimitive<PrimitiveDataType::Char>();
}

void PrimitiveArrayTest::testReadInt8()
{
    testReadPrimitive<PrimitiveDataType::Int8>();
}

void PrimitiveArrayTest::testReadInt16()
{
    testReadPrimitive<PrimitiveDataType::Int16>();
}

void PrimitiveArrayTest::testReadInt32()
{
    testReadPrimitive<PrimitiveDataType::Int32>();
}

void PrimitiveArrayTest::testReadInt64()
{
    testReadPrimitive<PrimitiveDataType::Int64>();
}

void PrimitiveArrayTest::testReadUInt8()
{
    testReadPrimitive<PrimitiveDataType::UInt8>();
}

void PrimitiveArrayTest::testReadUInt16()
{
    testReadPrimitive<PrimitiveDataType::UInt16>();
}

void PrimitiveArrayTest::testReadUInt32()
{
    testReadPrimitive<PrimitiveDataType::UInt32>();
}

void PrimitiveArrayTest::testReadUInt64()
{
    testReadPrimitive<PrimitiveDataType::UInt64>();
}

void PrimitiveArrayTest::testReadBool8()
{
    testReadPrimitive<PrimitiveDataType::Bool8>();
}

void PrimitiveArrayTest::testReadBool16()
{
    testReadPrimitive<PrimitiveDataType::Bool16>();
}

void PrimitiveArrayTest::testReadBool32()
{
    testReadPrimitive<PrimitiveDataType::Bool32>();
}

void PrimitiveArrayTest::testReadBool64()
{
    testReadPrimitive<PrimitiveDataType::Bool64>();
}

QTEST_GUILESS_MAIN(PrimitiveArrayTest)

#include "primitivearraytest.moc"
