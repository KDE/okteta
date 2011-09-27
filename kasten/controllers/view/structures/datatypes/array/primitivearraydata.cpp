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
qint64 PrimitiveArrayData<type>::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address, quint64 bitsRemaining)
{
    Q_ASSERT(bitsRemaining % 8 == 0);
    //vector has been resized to hold all elements
    uint maxReadCount = (bitsRemaining / 8) / sizeof(T);
    if (maxReadCount == 0)
        return -1;
    uint max = qMin(length(), maxReadCount);
    uint currAddr = address;
    ByteOrder byteOrder = AbstractArrayData::mParent->byteOrder();
    bool littleEndian = byteOrder == ByteOrderEnumClass::LittleEndian;
    for (uint i = 0; i < max; ++i)
    {
        mData[i] = readOneItem(address, input, littleEndian);
        currAddr += sizeof(T);
    }
    mNumReadValues = maxReadCount; //could not read any more
    if (maxReadCount < length())
    {
        //could not read all values -> all bits have been read
        return bitsRemaining;
    }
    else
        return maxReadCount * 8 * sizeof(T);
}

template<PrimitiveDataType type>
bool PrimitiveArrayData<type>::setChildData(uint row, QVariant value, Okteta::AbstractByteArrayModel* out,
        Okteta::Address address, BitCount64 bitsRemaining)
{
    Q_ASSERT(row < length());
    Q_ASSERT(bitsRemaining % 8 == 0);
    if ((row + 1) * sizeof(T) * 8 < bitsRemaining)
    {
        kDebug() << AbstractArrayData::mParent->name() << " not enough bits remaining ("
                << bitsRemaining << ") need " << ((row + 1) * sizeof(T) * 8);
        return false;
    }
    ByteOrder byteOrder =  AbstractArrayData::mParent->byteOrder();
    bool littleEndian = byteOrder == ByteOrderEnumClass::LittleEndian;
    writeOneItem(DisplayClass::fromVariant(value), address + (row * sizeof(T)), out, littleEndian);
    return true;
}



template<PrimitiveDataType type>
typename PrimitiveArrayData<type>::T PrimitiveArrayData<type>::readOneItem(Okteta::Address addr,
        Okteta::AbstractByteArrayModel* input, bool littleEndian)
{
    T result = 0;
    if (littleEndian)
    {
        for (uint i = 0; i < sizeof(T); ++i)
        {
            //compiler should be smart enough not to create a loop
            result |= T(input->byte(addr + i)) << (8 * i);
        }
    }
    else
    {
        for (uint i = 0; i < sizeof(T); ++i)
        {
            //compiler should be smart enough not to create a loop
            result |= T(input->byte(addr + i)) << (8 * (sizeof(T) - i));
        }
    }
    return result;
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

template<>
float PrimitiveArrayData<Type_Float>::readOneItem(Okteta::Address addr,
        Okteta::AbstractByteArrayModel* input, bool littleEndian)
{
    Q_ASSERT(sizeof(float) == sizeof(quint32));
    union { quint32 intVal; float floatVal; } un;
    un.intVal = PrimitiveArrayData<Type_UInt32>::readOneItem(addr, input, littleEndian);
    return un.floatVal;
}

template<>
double PrimitiveArrayData<Type_Double>::readOneItem(Okteta::Address addr,
        Okteta::AbstractByteArrayModel* input, bool littleEndian)
{
    Q_ASSERT(sizeof(double) == sizeof(quint64));
    union { quint64 intVal; double doubleVal; } un;
    un.intVal = PrimitiveArrayData<Type_UInt64>::readOneItem(addr, input, littleEndian);
    return un.doubleVal;
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
            if (uint(index) >= mNumReadValues)
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
            + QString::number(length()) + QLatin1Char(']'));
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
    //TODO fix this
    Q_UNUSED(index);
    Q_UNUSED(engine);
    Q_UNUSED(handlerInfo);
    Q_ASSERT_X(false, "PrimitiveArrayData::toScriptValue", "Not implemented yet");
    return false;

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