/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include "topleveldatainformation.h"
#include "../script/scriptutils.h"

#include <QtScript/QScriptEngine>
#include <KIcon>

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

QVariant PrimitiveDataInformation::primitiveValue() const //XXX: unused, remove?
{
    return mValue.toQByteArray();
}

QVariant PrimitiveDataInformation::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == 0)
        {
            AbstractArrayDataInformation* par =
                    dynamic_cast<AbstractArrayDataInformation*> (parent());
            if (par)
                return QString("[%1]").arg(mIndex);
            return name();
        }
        else if (column == 1)
            return typeName();
        else if (column == 2)
            return valueString();
        else
            return QVariant();
    }
    else if (role == Qt::ToolTipRole)
    {
        if (mHasBeenValidated && !mValidationSuccessful)
        {
            QString validationError;
            if (additionalData() && !additionalData()->validationError().isEmpty())
                validationError = i18nc("not all values in this structure"
                    " are as they should be", "Validation failed: \"%1\"",
                        additionalData()->validationError());
            else
                validationError = i18nc("not all values in this structure"
                    " are as they should be", "Validation failed.");

            return i18n("Name: %1\nValue: %2\n\nType: %3\nSize: %4\n\n%5", name(),
                    valueString(), typeName(), sizeString(), validationError);
        }
        else
            return i18n("Name: %1\nValue: %2\n\nType: %3\nSize: %4", name(),
                    valueString(), typeName(), sizeString());
    }
    //TODO status tip is not displayed ??
    //    else if (role == Qt::StatusTipRole)
    //    {
    //        if (mHasBeenValidated && !mValidationSuccessful)
    //            return i18nc("not all values in this structure are as they should be",
    //                    "Validation failed.");
    //    }
    else if (role == Qt::DecorationRole && column == 0)
    {
        if (mHasBeenValidated && !mValidationSuccessful)
            return KIcon("dialog-warning"); //XXX is this the best way to display a validation error
    }
    return QVariant();
}

bool PrimitiveDataInformation::setData(const QVariant& value, DataInformation* inf,
        Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
        Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset)
{
    if (this != inf)
    {
        //make sure bitOffset is always incremented
        *bitOffset = (*bitOffset + size()) % 8;
        return false;
    }
    AllPrimitiveTypes oldVal(mValue);
    bool wasValid = mWasAbleToRead;
    // this is implemented in the subclasses
    AllPrimitiveTypes newVal = qVariantToAllPrimitiveTypes(value);

    //this handles remaining < size() for us
    mWasAbleToRead = mValue.writeBits(size(), newVal, out, byteOrder, address,
            bitsRemaining, bitOffset);

    if (oldVal != mValue || wasValid != mWasAbleToRead)
        emit dataChanged();
    return true;
}

Qt::ItemFlags PrimitiveDataInformation::flags(int column, bool fileLoaded) const
{
    if (column == 2 && fileLoaded)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

qint64 PrimitiveDataInformation::readData(Okteta::AbstractByteArrayModel *input,
        ByteOrder byteOrder, Okteta::Address address, quint64 bitsRemaining,
        quint8* bitOffset)
{
    if (bitsRemaining < size()) //TODO make size() unsigned
    {
        mWasAbleToRead = false;
        mValue = 0;
        return -1;
    }
    bool wasValid = mWasAbleToRead;
    AllPrimitiveTypes oldVal(mValue);

    mWasAbleToRead = mValue.readBits(size(), input, byteOrder, address,
            bitsRemaining, bitOffset);

    if (oldVal != mValue || wasValid != mWasAbleToRead)
        emit dataChanged();

    return size();
}

PrimitiveDataInformation::PrimitiveDataInformation(QString name,
        PrimitiveDataType type, int index, DataInformation* parent) :
    DataInformation(name, index, parent), mType(type), mWasAbleToRead(false)
{
    if (type == Type_NotPrimitive)
    {
        kWarning() << "attempting to add non-primitive type to "
            "PrimitiveDataInformation. Default value (int) will be used instead";
        mType = Type_Int32;
    }
}

PrimitiveDataInformation::PrimitiveDataInformation(const PrimitiveDataInformation& d) :
    DataInformation(d), mType(d.mType), mWasAbleToRead(false)
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

QScriptValue PrimitiveDataInformation::scriptValue()
{
    QScriptEngine* engine = topLevelDataInformation()->scriptEngine();
    if (!engine)
        return QScriptValue();
    QScriptValue wrapObj = engine->newObject();
    ScriptUtils::object()->wrapAllPrimitiveTypes(wrapObj, mValue, mType);
    return wrapObj;
}
