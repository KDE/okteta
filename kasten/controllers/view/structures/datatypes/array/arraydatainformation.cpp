/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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
#include "arraydatainformation.h"
#include "../primitivefactory.h"
#include "../dummydatainformation.h"
#include "../../script/scriptvalueconverter.h"
#include "../../script/classes/arrayscriptclass.h"
#include "../../script/scripthandlerinfo.h"

#include <QtScript/QScriptContext>
#include "complexarraydata.h"

ArrayDataInformation::ArrayDataInformation(QString name, uint length, DataInformation* childType,
        DataInformation* parent) : DataInformation(name, parent), mData(0)
{
    if (length > MAX_LEN)
    {
        kWarning() << "array " << name << ": " << length << "exceeds maximum length of " << MAX_LEN;
        length = MAX_LEN;
    }
    childType->setParent(this);
    mData = new ComplexArrayData(length, childType, this);
}

ArrayDataInformation::ArrayDataInformation(const ArrayDataInformation& d) :
    DataInformation(d), mData(0)
{
    if (d.mData)
    {
        mData = d.mData->clone();
        mData->setParent(this);
    }
}

ArrayDataInformation::~ArrayDataInformation()
{
    delete mData;
}

QScriptValue ArrayDataInformation::setArrayLength(int newLength, QScriptContext* context)
{
    kDebug() << "old child count: " << childCount();

    //arrays with length zero are useless -> minimum is 0
    if (newLength < 0)
        return context ? context->throwError(QLatin1String("new Array length is less than zero: ")
            + QString::number(newLength)) : QScriptValue();
    if (uint(newLength) > MAX_LEN)
    {
        kWarning() << "attempting to set the length of the array" << name() << "to "
                << newLength << " which would use too much memory";

        return context ? context->throwError(QLatin1String("new Array length is to large: ")
                + QString::number(newLength)) : QScriptValue();
    }
    mData->setLength(newLength);
    return true;
}

QScriptValue ArrayDataInformation::setArrayType(QScriptValue type, QScriptContext* context)
{
    ScriptValueConverter conv(type, QLatin1String("dummy"));
    DataInformation* newChildType = conv.convert();
    //return if conversion failed
    if (!newChildType)
    {
        if (context)
            return context->throwError(QLatin1String("String '") + type.toString()
                    + QLatin1String("' is not a valid identifier for a primitive data type"));
        else
            return QScriptValue();
    }
    //childType is valid -> begin changing the children to the new type
    mData->setChildType(newChildType);
    return true; //success
}

QScriptValue ArrayDataInformation::childType() const
{
    if (mData)
        return mData->strictTypeName().toLower();
    return QString();
}

QVariant ArrayDataInformation::childData(int row, int column, int role) const
{
    Q_ASSERT(uint(row) < mData->length());
    if (column == 0 && role == Qt::DisplayRole)
    {
        //name is being asked for 
        return QString(QLatin1Char('[') + QString::number(row) + QLatin1Char(']'));
    }
    else 
    {
        //just delegate to child
        return mData->dataAt(row, column, role);
    }
}

QScriptValue ArrayDataInformation::toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    QScriptValue ret = engine->newObject(handlerInfo->mArrayClass);
    ret.setData(engine->toScriptValue(static_cast<DataInformation*>(this)));
    return ret;
}

QWidget* ArrayDataInformation::createEditWidget(QWidget* parent) const
{
    Q_ASSERT_X(false, "ArrayDataInformation::createEditWidget", "this should never happen!");
    return 0;
}

QVariant ArrayDataInformation::dataFromWidget(const QWidget* w) const
{
    Q_ASSERT_X(false, "ArrayDataInformation::dataFromWidget", "this should never happen!");
    return QVariant();
}

void ArrayDataInformation::setWidgetData(QWidget* w) const
{
    Q_ASSERT_X(false, "ArrayDataInformation::setWidgetData", "this should never happen!");
}

quint64 ArrayDataInformation::offset(unsigned int index) const
{
    return mData->offset(index);
}

qint64 ArrayDataInformation::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset)
{
    if (*bitOffset != 0)
    {
        kWarning() << "in array " << name() << ": bit offset != 0 (" << *bitOffset << "), adding padding,"
                " arrays always start at full bytes";
        bitsRemaining -= *bitOffset;
    }
    qint64 ret = mData->readData(input, address, bitsRemaining);
    mWasAbleToRead = ret > 0; //if ret is -1 reading failed
    return ret;
}


bool ArrayDataInformation::setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
        Okteta::Address address, quint64 bitsRemaining, quint8 bitOffset)
{
    if (bitOffset != 0)
    {
        kWarning() << "in array " << name() << ": bit offset != 0 (" << bitOffset << "), adding padding,"
                " arrays always start at full bytes";
        bitsRemaining -= bitOffset;
    }
    return mData->setChildData(row, value, out, address, bitsRemaining);
}

bool ArrayDataInformation::setData(const QVariant&, Okteta::AbstractByteArrayModel*,
        Okteta::Address, quint64, quint8)
{
    Q_ASSERT_X(false, "ArrayDataInformation::setData()", "this should never be called");
    return false;
}
