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
#include "abstractarraydatainformation.h"
#include "topleveldatainformation.h"
#include "../script/scriptutils.h"

#include <QtScript/QScriptEngine>
#include <KIcon>

QVariant PrimitiveDataInformation::primitiveValue() const //XXX: unused, remove?
{
    return mValue.toQByteArray();
}

QVariant PrimitiveDataInformation::data(int column, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (column == ColumnName)
        {
            AbstractArrayDataInformation* par =
                    dynamic_cast<AbstractArrayDataInformation*> (parent());
            if (par)
                return QString("[%1]").arg(mIndex);
            return name();
        }
        else if (column == ColumnType)
            return typeName();
        else if (column == ColumnValue)
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
    else if (role == Qt::DecorationRole && column == ColumnName)
    {
        //XXX better icons?
        if (mHasBeenValidated)
            return mValidationSuccessful ? KIcon("task-complete") : KIcon(
                    "dialog-warning");
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
    DataInformation(name, index, parent), mType(type)
{
    if (type == Type_NotPrimitive)
    {
        kWarning() << "attempting to add non-primitive type to "
            "PrimitiveDataInformation. Default value (int) will be used instead";
        mType = Type_Int32;
    }
}

PrimitiveDataInformation::PrimitiveDataInformation(const PrimitiveDataInformation& d) :
    DataInformation(d), mType(d.mType)
{
}

PrimitiveDataInformation::~PrimitiveDataInformation()
{
}

QScriptValue PrimitiveDataInformation::scriptValue() const
{
    QScriptEngine* eng = engine();
    if (!eng)
        return QScriptValue();
    if (!mWasAbleToRead)
    {
        if (context())
            return context()->throwError("Attempting to read value from element"
                " that has not been read yet: " + name());
        else
            return QScriptValue("fail");
    }
    QScriptValue wrapObj = eng->newObject();
    ScriptUtils::object()->wrapAllPrimitiveTypes(wrapObj, mValue, mType);
    return wrapObj;
}
