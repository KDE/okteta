/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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
#include "primitivedatainformation.h"
#include "datainformation.h"
#include "staticlengtharraydatainformation.h"
#include "bool8datainformation.h"
#include "bool16datainformation.h"
#include "bool32datainformation.h"
#include "bool64datainformation.h"
#include "chardatainformation.h"
#include "doubledatainformation.h"
#include "floatdatainformation.h"
#include "int8datainformation.h"
#include "int16datainformation.h"
#include "int32datainformation.h"
#include "int64datainformation.h"
#include "uint8datainformation.h"
#include "uint16datainformation.h"
#include "uint32datainformation.h"
#include "uint64datainformation.h"

PrimitiveDataType PrimitiveDataInformation::typeStringToType(QString& typeStr)
{
    typeStr = typeStr.trimmed();
    if (typeStr == "Bool8")
        return Type_Bool8;
    if (typeStr == "Bool16")
        return Type_Bool16;
    if (typeStr == "Bool32")
        return Type_Bool32;
    if (typeStr == "Bool64")
        return Type_Bool64;
    if (typeStr == "Int8")
        return Type_Int8;
    if (typeStr == "UInt8")
        return Type_UInt8;
    if (typeStr == "Int16")
        return Type_Int16;
    if (typeStr == "UInt16")
        return Type_UInt16;
    if (typeStr == "Int32")
        return Type_Int32;
    if (typeStr == "UInt32")
        return Type_UInt32;
    if (typeStr == "Int64")
        return Type_Int64;
    if (typeStr == "UInt64")
        return Type_UInt64;
    if (typeStr == "Char")
        return Type_Char;
    if (typeStr == "Float")
        return Type_Float;
    if (typeStr == "Double")
        return Type_Double;

    kWarning() << "PrimitiveDataInformation::typeStringToType(): could not find"
        " correct value (typeStr=" << typeStr << ")";
    return Type_NotPrimitive; //just return a default value
}

QVariant PrimitiveDataInformation::primitiveValue() const
{
    return mValue.toQByteArray();
}

QVariant PrimitiveDataInformation::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == 0)
        {
            StaticLengthArrayDataInformation* par =
                    dynamic_cast<StaticLengthArrayDataInformation*> (parent());
            if (par)
                return QString("[%1]").arg(mIndex);
            return getName();
        }
        else if (column == 1)
            return getTypeName();
        else if (column == 2)
            return getValueString();
        else
            return QVariant();
    }
    else if (role == Qt::ToolTipRole)
    {
        return i18n("Name: %1\nValue: %2\n\nType: %3\nSize: %4", getName(),
                getValueString(), getTypeName(), getSizeString());
    }
    else
        return QVariant();
}

bool PrimitiveDataInformation::setData(const QVariant& value, DataInformation* inf,
        Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining)
{
    //FIXME move to the correct subclasses, switches are evil
    if (this != inf)
        return false;

    QString str = value.toString();

#define SETDATA(arg,method,cast)    newVal= AllPrimitiveTypes(cast method); \
    if (mValue != newVal && ok) \
    {   \
        mValue.arg = newVal.arg;    \
        setModelData(newVal,out,byteOrder,address,remaining); \
        emit dataChanged(); \
    } \
    break;

    bool ok = false;
    AllPrimitiveTypes newVal;
    int base = displayBase();
    switch (mType)
    {
    case Type_Float:
        SETDATA(floatValue,str.toFloat(&ok),(float))
    case Type_Double:
        SETDATA(doubleValue,str.toDouble(&ok),(double))
    case Type_Char:
        SETDATA(byteValue,str.toShort(&ok,base),(qint8))
    case Type_Int8:
        SETDATA(byteValue,value.toInt(&ok),(qint8))
    case Type_UInt8:
        SETDATA(ubyteValue,value.toUInt(&ok),(quint8))
    case Type_Int16:
        SETDATA(shortValue,value.toInt(&ok),(qint16))
    case Type_UInt16:
        SETDATA(ushortValue,value.toUInt(&ok),(quint16))
    case Type_Int32:
        SETDATA(intValue,value.toInt(&ok),(qint32))
    case Type_UInt32:
        SETDATA(uintValue,value.toUInt(&ok),(quint32))
    case Type_Int64:
        SETDATA(longValue,value.toLongLong(&ok),(qint64))
    case Type_UInt64:
        SETDATA(ulongValue,value.toULongLong(&ok),(quint64))
    case Type_Bool8:
        SETDATA(ubyteValue,value.toUInt(&ok),(quint8))
    case Type_Bool16:
        SETDATA(ushortValue,value.toUInt(&ok),(quint16))
    case Type_Bool32:
        SETDATA(uintValue,value.toUInt(&ok),(quint32))
    case Type_Bool64:
        SETDATA(ulongValue,value.toULongLong(&ok),(quint64))
    default:
        break;
    }
    return true;
}

void PrimitiveDataInformation::setModelData(AllPrimitiveTypes value,
        Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining)
{
    int size = getSize() / 8;
    if (remaining < size)
    {
        mValue.ulongValue = 0;
        mIsValid = false;
        return;
    }
    for (int i = 0; i < size; ++i)
    {
        int index = (byteOrder == ByteOrderEnumClass::LittleEndian) ? i
                : ((size - 1) - i);
        out->setByte(address + i, value.allBytes[index]);
    }
}

Qt::ItemFlags PrimitiveDataInformation::flags(int column, bool fileLoaded) const
{
    if (column == 2 && fileLoaded)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

Okteta::Size PrimitiveDataInformation::readData(
        Okteta::AbstractByteArrayModel* input, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining)
{
    int bytes = getSize() / 8;
    if (remaining < bytes)
    {
        mValue.ulongValue = 0;
        mIsValid = false;
        //use all the remaining bytes
        return remaining < 0 ? 0 : remaining;
    }
    bool changed = false;
    //always use unsigned value
    for (int i = 0; i < bytes; i++)
    {
        int index = (byteOrder == ByteOrderEnumClass::LittleEndian) ? i : ((bytes
                - 1) - i);
        Okteta::Byte readByte = input->byte(address + i);
        if (mValue.allBytes[index] != readByte)
        {
            mValue.allBytes[index] = readByte;
            changed = true;
        }
    }
    //fill remaining bytes with 0
    for (unsigned int i = bytes; i < sizeof(qint64); i++)
    {
        mValue.allBytes[i] = 0;
    }

    if (changed)
        emit dataChanged();
    mIsValid = true;
    return bytes;
}

PrimitiveDataInformation::PrimitiveDataInformation(QString name,
        PrimitiveDataType type, int index, DataInformation* parent) :
    DataInformation(name, index, parent), mType(type), mIsValid(false)
{
    if (type == Type_NotPrimitive)
    {
        kWarning() << "attempting to add non-primitive type to "
            "PrimitiveDataInformation. Default value (int) will be used instead";
        mType = Type_Int32;
    }
}

PrimitiveDataInformation::PrimitiveDataInformation(const PrimitiveDataInformation& d) :
    DataInformation(d), mType(d.mType), mIsValid(false)
{
}

PrimitiveDataInformation::~PrimitiveDataInformation()
{
}

PrimitiveDataInformation* PrimitiveDataInformation::newInstance(QString name,
        PrimitiveDataType type, int index, DataInformation* parent)
{
    switch (type)
    {
    case Type_Char:
        return new CharDataInformation(name, type, index, parent);
    case Type_Int8:
        return new Int8DataInformation(name, type, index, parent);
    case Type_Int16:
        return new Int16DataInformation(name, type, index, parent);
    case Type_Int32:
        return new Int32DataInformation(name, type, index, parent);
    case Type_Int64:
        return new Int64DataInformation(name, type, index, parent);
    case Type_UInt8:
        return new UInt8DataInformation(name, type, index, parent);
    case Type_UInt16:
        return new UInt16DataInformation(name, type, index, parent);
    case Type_UInt32:
        return new UInt32DataInformation(name, type, index, parent);
    case Type_UInt64:
        return new UInt64DataInformation(name, type, index, parent);
    case Type_Bool8:
        return new Bool8DataInformation(name, type, index, parent);
    case Type_Bool16:
        return new Bool16DataInformation(name, type, index, parent);
    case Type_Bool32:
        return new Bool32DataInformation(name, type, index, parent);
    case Type_Bool64:
        return new Bool64DataInformation(name, type, index, parent);
    case Type_Float:
        return new FloatDataInformation(name, type, index, parent);
    case Type_Double:
        return new DoubleDataInformation(name, type, index, parent);
    default:
        return NULL;
    }
}

//int PrimitiveDataInformation::getStringConversionBase(PrimitiveDataType type)
//{
//    int enumVal = 0;
//    switch (type)
//    {
//    case Bool8:
//    case Bool16:
//    case Bool32:
//    case Bool64:
//    case UInt8:
//    case UInt16:
//    case UInt32:
//    case UInt64:
//        enumVal = Kasten::StructViewPreferences::unsignedDisplayBase();
//        if (enumVal == StructViewPreferences::EnumUnsignedDisplayBase::Binary)
//            return 2;
//        if (enumVal == StructViewPreferences::EnumUnsignedDisplayBase::Decimal)
//            return 10;
//        if (enumVal == StructViewPreferences::EnumUnsignedDisplayBase::Hexadecimal)
//            return 16;
//        break;
//    case Int8:
//    case Int16:
//    case Int32:
//    case Int64:
//        enumVal = Kasten::StructViewPreferences::signedDisplayBase();
//        if (enumVal == StructViewPreferences::EnumSignedDisplayBase::Binary)
//            return 2;
//        if (enumVal == StructViewPreferences::EnumSignedDisplayBase::Decimal)
//            return 10;
//        if (enumVal == StructViewPreferences::EnumSignedDisplayBase::Hexadecimal)
//            return 16;
//        break;
//
//    case Char:
//        enumVal = Kasten::StructViewPreferences::charDisplayBase();
//        if (enumVal == StructViewPreferences::EnumCharDisplayBase::Binary)
//            return 2;
//        if (enumVal == StructViewPreferences::EnumCharDisplayBase::Decimal)
//            return 10;
//        if (enumVal == StructViewPreferences::EnumCharDisplayBase::Hexadecimal)
//            return 16;
//        break;
//    case Float:
//    case Double:
//        //just return -1, it won't be read anyway.
//        return -1;
//    default:
//        break;
//    }
//    kWarning() << "PrimitiveDataInformation::getStringConversionBase():"
//        " no match found (type=" << type << ", enumVal=" << enumVal << ")";
//    return 10;
//}

//QString PrimitiveDataInformation::getValueString() const
//{
//    if (!mIsValid)
//    {
//        return i18nc("invalid value (out of range)", "<invalid>");
//    }
//    QString number;
//    int base = getStringConversionBase(mType);
//    return PrimitiveDataInformation::getValueString(mValue, mType, base);
//}

//QString PrimitiveDataInformation::getValueString(AllPrimitiveTypes value,
//        PrimitiveDataType type, int base)
//{
//    QString number;
//    switch (type)
//    {
//    case Int8:
//        number = QString::number(value.byteValue, base);
//        break;
//    case Int16:
//        number = QString::number(value.shortValue, base);
//        break;
//    case Int32:
//        number = QString::number(value.intValue, base);
//        break;
//    case Int64:
//        number = QString::number(value.longValue, base);
//        break;
//    case Bool8:
//    case UInt8:
//        number = QString::number(value.ubyteValue, base);
//        break;
//    case Bool16:
//    case UInt16:
//        number = QString::number(value.ushortValue, base);
//        break;
//    case Bool32:
//    case UInt32:
//        number = QString::number(value.uintValue, base);
//        break;
//    case Bool64:
//    case UInt64:
//        number = QString::number(value.ulongValue, base);
//        break;
//    case Char:
//        number = QString::number(value.ubyteValue, base);
//        break;
//    case Float:
//        if (Kasten::StructViewPreferences::localeAwareFloatFormatting())
//        {
//            number = KGlobal::locale()->formatNumber(value.floatValue,
//                    Kasten::StructViewPreferences::floatPrecision());
//        }
//        else
//            number = QString::number(value.floatValue, 'g',
//                    Kasten::StructViewPreferences::floatPrecision());
//        break;
//    case Double:
//        if (Kasten::StructViewPreferences::localeAwareFloatFormatting())
//        {
//            number = KGlobal::locale()->formatNumber(value.doubleValue,
//                    Kasten::StructViewPreferences::floatPrecision());
//        }
//        else
//            number = QString::number(value.doubleValue, 'g',
//                    Kasten::StructViewPreferences::floatPrecision());
//        break;
//    default:
//        kWarning("default case reached, should never happen");
//        number = QString();
//    }
//    if (base == 16)
//    {
//        if ((getSize(type) / 4) > number.length())
//        {
//            //2 digits per byte in hex
//            number = number.right((getSize(type) / 4));
//        }
//        number = "0x" + number;
//    }
//
//    if (type == Char)
//    {
//        //TODO char codec
//        QChar qchar = value.ubyteValue;
//        qchar = qchar.isPrint() ? qchar : QChar((char) QChar::ReplacementCharacter);
//        QString charStr = '\'' + qchar + '\'';
//        if (Kasten::StructViewPreferences::showCharNumericalValue())
//            charStr += " (" + number + ")";
//        return charStr;
//    }
//    if (type == Bool8 || type == Bool16 || type == Bool32 || type == Bool64)
//    {
//        //other bytes are zero, so just compare ulongvalue
//        QString boolStr;
//        if (value.ulongValue == 0)
//            boolStr = i18nc("value of boolean data type", "false");
//        else if (value.ulongValue == 1)
//            boolStr = i18nc("value of boolean data type", "true");
//        else
//        {
//            boolStr = i18nc("value of boolean data type with value", "true (%1)");
//            boolStr = boolStr.arg(number);
//        }
//        return boolStr;
//    }
//    //otherwise just return number (float and double)
//    if (StructViewPreferences::localeAwareDecimalFormatting() && base == 10)
//    {
//        //FIXME is this safe? it seems the value is converted to a double and then formatted
//        return KGlobal::locale()->formatNumber(number, false, 0);
//    }
//    //float and double:
//    return number;
//}

//int PrimitiveDataInformation::getSize(PrimitiveDataType type)
//{
//    int bytes;
//    switch (type)
//    {
//    case Int8:
//    case UInt8:
//    case Bool8:
//    case Char:
//        bytes = 1;
//        break;
//    case Bool16:
//    case Int16:
//    case UInt16:
//        bytes = 2;
//        break;
//    case Bool32:
//    case Float:
//    case Int32:
//    case UInt32:
//        bytes = 4;
//        break;
//    case Bool64:
//    case Double:
//    case Int64:
//    case UInt64:
//        bytes = 8;
//        break;
//    default:
//        bytes = 0;
//    }
//    return bytes * 8;
//}

//QString PrimitiveDataInformation::getTypeName(PrimitiveDataType type)
//{
//    switch (type)
//    {
//    case Int8:
//        return i18nc("data type", "byte");
//    case Int16:
//        return i18nc("data type", "short");
//    case Int32:
//        return i18nc("data type", "int");
//    case Int64:
//        return i18nc("data type", "long");
//    case UInt8:
//        return i18nc("data type", "unsigned byte");
//    case UInt16:
//        return i18nc("data type", "unsigned short");
//    case UInt32:
//        return i18nc("data type", "unsigned int");
//    case UInt64:
//        return i18nc("data type", "unsigned long");
//    case Char:
//        return i18nc("data type", "char");
//    case Float:
//        return i18nc("data type", "float");
//    case Double:
//        return i18nc("data type", "double");
//    case Bool8:
//        return i18nc("data type", "bool (1 byte)");
//    case Bool16:
//        return i18nc("data type", "bool (2 bytes)");
//    case Bool32:
//        return i18nc("data type", "bool (4 bytes)");
//    case Bool64:
//        return i18nc("data type", "bool (8 bytes)");
//    default:
//        return QString();
//    }
//}
