/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "arraydatainformation.hpp"

#include "../primitivefactory.hpp"
#include "../dummydatainformation.hpp"
#include "../../parsers/scriptvalueconverter.hpp"
#include "../../script/classes/arrayscriptclass.hpp"
#include "../../script/scripthandlerinfo.hpp"
#include "../../script/scriptlogger.hpp"

#include "complexarraydata.hpp"
#include "primitivearraydata.hpp"

ArrayDataInformation::ArrayDataInformation(const QString& name, uint length, DataInformation* childType,
                                           DataInformation* parent, const QScriptValue& lengthFunction)
    : DataInformationWithDummyChildren(name, parent)
{
    if (lengthFunction.isValid()) {
        Q_ASSERT(lengthFunction.isFunction());
        setLengthFunction(lengthFunction);
    }
    if (length > MAX_LEN) {
        logWarn() << length << "exceeds maximum length of" << MAX_LEN
                  << ". Setting it to" << MAX_LEN << "instead";
        length = MAX_LEN;
    }
    Q_CHECK_PTR(childType);
    childType->setParent(this);
    mData.reset(AbstractArrayData::newArrayData(length, childType, this));
}

ArrayDataInformation::ArrayDataInformation(const ArrayDataInformation& d)
    : DataInformationWithDummyChildren(d)
    , mData(nullptr)
{
    uint length = d.mData->length();
    DataInformation* childType = d.mData.data()->childType();
    mData.reset(AbstractArrayData::newArrayData(length, childType->clone(), this));
}

ArrayDataInformation::~ArrayDataInformation() = default;

bool ArrayDataInformation::setArrayLength(uint newLength)
{
    if (newLength > MAX_LEN) {
        logWarn() << QStringLiteral("new array length is too large (%1), limiting to (%2)")
            .arg(QString::number(newLength), QString::number(MAX_LEN));
        newLength = MAX_LEN;
    }
    uint oldLength = mData->length();
    topLevelDataInformation()->_childCountAboutToChange(this, oldLength, newLength);
    mData->setLength(newLength);
    topLevelDataInformation()->_childCountChanged(this, oldLength, newLength);
    return true;
}

void ArrayDataInformation::setArrayType(DataInformation* newChildType)
{
    Q_CHECK_PTR(newChildType);
    if (newChildType->isPrimitive() && newChildType->asPrimitive()->type() == mData->primitiveType()) {
        // there is no need to change the type
        logInfo() << "New and old child type are identical, skipping: " << mData->primitiveType();
        delete newChildType;
        return;
    }
    newChildType->setParent(this);
    uint len = mData->length();
    TopLevelDataInformation* topLevel = topLevelDataInformation();
    if (len > 0) {
        // first create with length of 0, then change length to actual length (to ensure model is correct)
        topLevel->_childCountAboutToChange(this, len, 0);
        mData.reset(AbstractArrayData::newArrayData(0, newChildType, this));
        topLevel->_childCountChanged(this, len, 0);

        topLevel->_childCountAboutToChange(this, 0, len);
        mData->setLength(len);
        topLevel->_childCountChanged(this, 0, len);
    } else {
        // no need to emit the signals, which cause expensive model update
        mData.reset(AbstractArrayData::newArrayData(len, newChildType, this));
        // only the type of the array changed -> emit that this has changed data
        topLevel->setChildDataChanged();
    }
}

QScriptValue ArrayDataInformation::childType() const
{
    return mData->typeName().toLower();
}

QVariant ArrayDataInformation::childData(int row, int column, int role) const
{
    Q_ASSERT(uint(row) < mData->length());
    return mData->dataAt(row, column, role);
}

QWidget* ArrayDataInformation::createEditWidget(QWidget*) const
{
    Q_ASSERT_X(false, "ArrayDataInformation::createEditWidget", "this should never happen!");
    return nullptr;
}

QVariant ArrayDataInformation::dataFromWidget(const QWidget*) const
{
    Q_ASSERT_X(false, "ArrayDataInformation::dataFromWidget", "this should never happen!");
    return {};
}

void ArrayDataInformation::setWidgetData(QWidget*) const
{
    Q_ASSERT_X(false, "ArrayDataInformation::setWidgetData", "this should never happen!");
}

BitCount64 ArrayDataInformation::childPosition(const DataInformation* child, Okteta::Address start) const
{
    if (mParent->isTopLevel()) {
        return start * 8 + mData->offset(child);
    } 

    return mParent->asDataInformation()->childPosition(this, start) + mData->offset(child);
}

qint64 ArrayDataInformation::readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                                      BitCount64 bitsRemaining, quint8* bitOffset)
{
    if (*bitOffset != 0) {
        // TODO remove this, it will probably cause issues
        logWarn() << "bit offset != 0 (" << *bitOffset << "), adding padding,"
            " arrays always start at full bytes";
        bitsRemaining &= BitCount64(~7); // unset lower 3 bits to make it divisible by 8
        address++;
    }

    // update the length of the array
    topLevelDataInformation()->scriptHandler()->updateLength(this);

    // FIXME do not add this padding
    qint64 ret = mData->readData(input, address, bitsRemaining);
    mWasAbleToRead = ret >= 0; // if ret is -1 reading failed
    return ret;
}

bool ArrayDataInformation::setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                                        Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset)
{
    if (bitOffset != 0) {
        logWarn() << "bit offset != 0 (" << bitOffset << "), adding padding,"
            " arrays always start at full bytes";
        bitsRemaining -= bitOffset;
        address++;
    }
    return mData->setChildData(row, value, out, address, bitsRemaining);
}

bool ArrayDataInformation::setData(const QVariant&, Okteta::AbstractByteArrayModel*,
                                   Okteta::Address, BitCount64, quint8)
{
    Q_ASSERT_X(false, "ArrayDataInformation::setData()", "this should never be called");
    return false;
}

QScriptClass* ArrayDataInformation::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->mArrayClass.data();
}

QScriptValue ArrayDataInformation::childToScriptValue(uint index, QScriptEngine* engine,
                                                      ScriptHandlerInfo* handlerInfo) const
{
    return mData->toScriptValue(index, engine, handlerInfo);
}

QString ArrayDataInformation::typeNameImpl() const
{
    return mData->typeName();
}

BitCount32 ArrayDataInformation::size() const
{
    return mData->size();
}

bool ArrayDataInformation::isArray() const
{
    return true;
}

DataInformation* ArrayDataInformation::childAt(unsigned int idx) const
{
    return mData->childAt(idx);
}

unsigned int ArrayDataInformation::childCount() const
{
    return mData->length();
}

bool ArrayDataInformation::canHaveChildren() const
{
    return true;
}

QString ArrayDataInformation::childTypeName(uint index) const
{
    return mData->dataAt(index, DataInformation::ColumnType, Qt::DisplayRole).toString();
}

int ArrayDataInformation::indexOf(const DataInformation* const data) const
{
    return mData->indexOf(data);
}

BitCount32 ArrayDataInformation::childSize(uint index) const
{
    return mData->sizeAt(index);
}

Qt::ItemFlags ArrayDataInformation::childFlags(int row, int column, bool fileLoaded) const
{
    return mData->childFlags(row, column, fileLoaded);
}

QWidget* ArrayDataInformation::createChildEditWidget(uint index, QWidget* parent) const
{
    return mData->createChildEditWidget(index, parent);
}

QVariant ArrayDataInformation::dataFromChildWidget(uint index, const QWidget* w) const
{
    return mData->dataFromChildWidget(index, w);
}

void ArrayDataInformation::setChildWidgetData(uint index, QWidget* w) const
{
    mData->setChildWidgetData(index, w);
}
