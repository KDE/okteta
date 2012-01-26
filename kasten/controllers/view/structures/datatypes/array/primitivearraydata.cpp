/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include "primitivearraydata.h"
#include "../datainformation.h"
#include "../primitive/primitivedatainformation.h"

#include <abstractbytearraymodel.h>


template<PrimitiveDataType type>
qint64 PrimitiveArrayData<type>::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
    BitCount64 bitsRemaining)
{
    Q_ASSERT(bitsRemaining % 8 == 0);
    if (this->length() == 0)
        return 0; //no need to read anything
    //integer division -> gives us the desired result, limited by the number of items in this array
    const quint64 maxRemaining = (bitsRemaining / 8) / sizeof(T);
    //since its 64 bits may be larger than a 32 bit value and have all lower 32 bits as zero
    //therefore we use std::numeric_limits::max()
    const quint32 maxRemaining32 = (maxRemaining > std::numeric_limits<quint32>::max())
            ? std::numeric_limits<quint32>::max() : quint32(maxRemaining);
    const quint32 maxNumItems = qMin(this->length(), maxRemaining32);
    if (maxNumItems == 0)
        return -1; //reached EOF
    const ByteOrder byteOrder = AbstractArrayData::mParent->effectiveByteOrder();
    const bool littleEndian = byteOrder == ByteOrderEnumClass::LittleEndian;
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    if (littleEndian)
        this->readDataNativeOrder(maxNumItems, input, address);
    else
        this->readDataNonNativeOrder(maxNumItems, input, address);
#elif Q_BYTE_ORDER == Q_BIG_ENDIAN
    if (littleEndian)
        this->readDataNonNativeOrder(maxNumItems, input, address);
    else
        this->readDataNativeOrder(maxNumItems, input, address);
#else
#warning Unknown byte order, doing nothing!
#endif
    this->mNumReadValues = maxNumItems;
    return maxNumItems * sizeof(T) * 8;
}

template<PrimitiveDataType type>
void PrimitiveArrayData<type>::readDataNativeOrder(uint numItems, Okteta::AbstractByteArrayModel* input,
        Okteta::Address address)
{
    Q_ASSERT(numItems <= length());
    const Okteta::Size numBytes = numItems * sizeof(T);
    Q_ASSERT(input->size() >= numBytes + address);
    Q_UNUSED(numBytes);
    Okteta::Byte* vectorBytes = reinterpret_cast<Okteta::Byte*>(this->mData.data());
    const Okteta::Size numCopied = input->copyTo(vectorBytes, address, numItems * sizeof(T));
    Q_ASSERT(numCopied == numBytes);
    Q_UNUSED(numCopied);
}

template<PrimitiveDataType type>
void PrimitiveArrayData<type>::readDataNonNativeOrder(uint numItems, Okteta::AbstractByteArrayModel* input,
        Okteta::Address address)
{
    Q_ASSERT(numItems <= length());
    const uint numBytes = numItems * sizeof(T);
    Q_ASSERT(uint(input->size()) >= numBytes + address);
    Okteta::Byte* vectorBytes = reinterpret_cast<Okteta::Byte*>(this->mData.data());
    for (uint itemOffs = 0; itemOffs < numBytes; itemOffs += sizeof(T))
    {
        //the compiler should unroll this loop
        for (uint byte = 0; byte < sizeof(T); byte++)
        {
            vectorBytes[itemOffs + byte] = input->byte(address + itemOffs + (sizeof(T) - byte - 1));
        }
    }
}

template<PrimitiveDataType type>
bool PrimitiveArrayData<type>::setChildData(uint row, QVariant value, Okteta::AbstractByteArrayModel* out,
        Okteta::Address address, BitCount64 bitsRemaining)
{
    Q_ASSERT(row < length());
    Q_ASSERT(bitsRemaining % 8 == 0);
    if ((row + 1) * sizeof(T) * 8 >= bitsRemaining)
    {
        kDebug() << AbstractArrayData::mParent->name() << " not enough bits remaining ("
                << bitsRemaining << ") need " << ((row + 1) * sizeof(T) * 8);
        return false;
    }
    ByteOrder byteOrder =  AbstractArrayData::mParent->effectiveByteOrder();
    bool littleEndian = byteOrder == ByteOrderEnumClass::LittleEndian;
    T convertedVal = DisplayClass::fromVariant(value);
    kDebug() << AbstractArrayData::mParent->name() << "setting index" << row << "to" << value << "(= " << convertedVal << ")";
    this->mData[row] = convertedVal;
    this->writeOneItem(convertedVal, address + (row * sizeof(T)), out, littleEndian);
    return true;
}



template<PrimitiveDataType type>
void PrimitiveArrayData<type>::writeOneItem(T value, Okteta::Address addr,
        Okteta::AbstractByteArrayModel* out, bool littleEndian)
{
    if (littleEndian)
   {
        for (uint i = 0; i < sizeof(T); ++i)
        {
            //compiler should be smart enough not to create a loop
            quint8 val = (quint64(value) & (quint64(0xff) << (8 * i))) >> (8 * i);
            out->setByte(addr + i, val);
        }
    }
    else
    {
        for (uint i = 0; i < sizeof(T); ++i)
        {
            //compiler should be smart enough not to create a loop
            quint8 val = (quint64(value) & (quint64(0xff) << (8 * (sizeof(T) - i)))) >> (8 * (sizeof(T) - i));
            out->setByte(addr + i, val);
        }
    }
}

template<>
void PrimitiveArrayData<Type_Float>::writeOneItem(float value, Okteta::Address addr,
        Okteta::AbstractByteArrayModel* out, bool littleEndian)
{
    Q_ASSERT(sizeof(float) == sizeof(quint32));
    union { quint32 intVal; float floatVal; } un;
    un.floatVal = value;
    PrimitiveArrayData<Type_UInt32>::writeOneItem(un.intVal, addr, out, littleEndian);
}

template<>
void PrimitiveArrayData<Type_Double>::writeOneItem(double value, Okteta::Address addr,
        Okteta::AbstractByteArrayModel* out, bool littleEndian)
{
    Q_ASSERT(sizeof(double) == sizeof(quint64));
    union { quint64 intVal; double doubleVal; } un;
    un.doubleVal = value;
    PrimitiveArrayData<Type_UInt64>::writeOneItem(un.intVal, addr, out, littleEndian);
}

template<PrimitiveDataType type>
QVariant PrimitiveArrayData<type>::dataAt(int index, int column, int role)
{
    Q_ASSERT(index >= 0 && (unsigned)index < length());
    if (role == Qt::DisplayRole)
    {
        if (column == DataInformation::ColumnName)
            return QString(AbstractArrayData::mParent->name() + QLatin1Char('[')
                    + QString::number(index) + QLatin1Char(']'));
        if (column == DataInformation::ColumnType)
            return PrimitiveDataInformation::typeName(type);
        if (column == DataInformation::ColumnValue)
        {
            if (uint(index) >= this->mNumReadValues)
            {
                //we are outside the valid range
                return i18nc("invalid value (out of range)", "&lt;invalid&gt;");
            }
            return DisplayClass::valueString(mData.at(index));
        }
    }
    return QVariant();
}

template<PrimitiveDataType type>
QString PrimitiveArrayData<type>::typeName() const
{
    return QString(PrimitiveDataInformation::typeName(type) + QLatin1Char('[')
            + QString::number(this->length()) + QLatin1Char(']'));
}

template<PrimitiveDataType type>
int PrimitiveArrayData<type>::indexOf(const DataInformation* data) const
{
    Q_UNUSED(data);
    Q_ASSERT_X(false, "PrimitiveArrayData::indexOf", "This should never be called");
    return 0;
}

template<PrimitiveDataType type>
QScriptValue PrimitiveArrayData<type>::toScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const
{
    Q_ASSERT(index < length());
    return DisplayClass::asScriptValue(mData.at(index), engine, handlerInfo);

}

template<PrimitiveDataType type>
QWidget* PrimitiveArrayData<type>::createChildEditWidget(uint index, QWidget* parent) const
{
    Q_ASSERT(index < length());
    return DisplayClass::staticCreateEditWidget(parent);
}

template<PrimitiveDataType type>
QVariant PrimitiveArrayData<type>::dataFromChildWidget(uint index, const QWidget* w) const
{
    Q_ASSERT(index < length());
    return DisplayClass::staticDataFromWidget(w);
}

template<PrimitiveDataType type>
void PrimitiveArrayData<type>::setChildWidgetData(uint index, QWidget* w) const
{
    Q_ASSERT(index < length());
    DisplayClass::staticSetWidgetData(mData.at(index), w);
}


//now instantiate all the template instances
template class PrimitiveArrayData<Type_Bool8>;
template class PrimitiveArrayData<Type_Bool16>;
template class PrimitiveArrayData<Type_Bool32>;
template class PrimitiveArrayData<Type_Bool64>;
template class PrimitiveArrayData<Type_Int8>;
template class PrimitiveArrayData<Type_Int16>;
template class PrimitiveArrayData<Type_Int32>;
template class PrimitiveArrayData<Type_Int64>;
template class PrimitiveArrayData<Type_UInt8>;
template class PrimitiveArrayData<Type_UInt16>;
template class PrimitiveArrayData<Type_UInt32>;
template class PrimitiveArrayData<Type_UInt64>;
template class PrimitiveArrayData<Type_Char>;
template class PrimitiveArrayData<Type_Float>;
template class PrimitiveArrayData<Type_Double>;
