/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#include "defaultscriptclass.h"

#include "../../datatypes/datainformation.h"
#include "../../datatypes/topleveldatainformation.h"
#include "../../datatypes/uniondatainformation.h"
#include "../../datatypes/structuredatainformation.h"
#include "../../datatypes/primitive/pointerdatainformation.h"
#include "../../parsers/parserutils.h"
#include "../../parsers/scriptvalueconverter.h"
#include "../scriptlogger.h"
#include "../scripthandlerinfo.h"
#include "../safereference.h"

#include <KDebug>

DefaultScriptClass::DefaultScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : QScriptClass(engine), mHandlerInfo(handlerInfo)
{
    s_valid = engine->toStringHandle(ParserStrings::PROPERTY_VALID);
    s_wasAbleToRead = engine->toStringHandle(ParserStrings::PROPERTY_ABLE_TO_READ);
    s_validationError = engine->toStringHandle(ParserStrings::PROPERTY_VALIDATION_ERROR);
    s_parent = engine->toStringHandle(ParserStrings::PROPERTY_PARENT);
    s_byteOrder = engine->toStringHandle(ParserStrings::PROPERTY_BYTEORDER);
    s_name = engine->toStringHandle(ParserStrings::PROPERTY_NAME);
    s_datatype = engine->toStringHandle(ParserStrings::PROPERTY_DATATYPE);
    s_updateFunc = engine->toStringHandle(ParserStrings::PROPERTY_UPDATE_FUNC);
    s_validationFunc = engine->toStringHandle(ParserStrings::PROPERTY_VALIDATION_FUNC);

    //TODO remove, every subclass should have proto
    mDefaultPrototype = engine->newObject();
    mDefaultPrototype.setProperty(QLatin1String("toString"), engine->newFunction(Default_proto_toString));
    //add all our properties
    mIterableProperties.append(qMakePair(s_parent, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_name, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_wasAbleToRead, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_byteOrder, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_valid, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_validationError, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_validationFunc, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_updateFunc, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_datatype, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
}

DefaultScriptClass::~DefaultScriptClass()
{
}

DataInformation* DefaultScriptClass::toDataInformation(const QScriptValue& obj)
{
    if (!obj.scriptClass())
        return 0;
    Q_ASSERT(obj.data().isVariant());
    const QVariant variant = obj.data().toVariant();
    if (variant.isValid() && variant.canConvert<SafeReference>() && variant.userType() == qMetaTypeId<SafeReference>())
    {
        const SafeReference& ref = *reinterpret_cast<const SafeReference*>(variant.constData());
        return ref.data();
    }
    return 0;
}

QScriptClass::QueryFlags DefaultScriptClass::queryProperty(const QScriptValue& object,
            const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    const ScriptHandlerInfo::Mode mode = mHandlerInfo->mode();
    Q_ASSERT(mode != ScriptHandlerInfo::None);
    DataInformation* data = toDataInformation(object);
    if (!data)
    {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.data().toString();
        engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                QLatin1String("Attempting to access an invalid object"));
        return 0;
    }
    if (name == s_valid || name == s_validationError)
    {
        return mode == ScriptHandlerInfo::Validating ? flags : flags & ~HandlesWriteAccess;
    }
    if (mode != ScriptHandlerInfo::Updating)
    {
        //the only properties that are possibly writable when not updating are valid and validationError
        //but we checked them before so we remove handlesWriteAccess from the flags
        flags &= ~HandlesWriteAccess;
    }

    if (name == s_byteOrder || name == s_name || name == s_updateFunc
            || name == s_validationFunc || name == s_datatype)
    {
        return flags;
    }
    else if (name == s_wasAbleToRead || name == s_parent)
    {
        return flags &= ~HandlesWriteAccess;
    }
    else if (queryAdditionalProperty(data, name, &flags, id))
    {
        return flags;
    }
    else
    {
        data->logError() << "could not find property with name" << name.toString();
        engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                QLatin1String("Could not find property with name ") + name.toString());
        return 0;
    }
}

QScriptValue DefaultScriptClass::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    Q_ASSERT(mHandlerInfo->mode() != ScriptHandlerInfo::None);
    DataInformation* data = toDataInformation(object);
    if (!data)
    {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.data().toString();
        return engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                QLatin1String("Attempting to access an invalid object"));
    }
    if (name == s_valid)
    {
        return data->validationSuccessful();
    }
    else if (name == s_wasAbleToRead)
    {
        return data->wasAbleToRead();
    }
    else if (name == s_parent)
    {
        Q_CHECK_PTR(data->parent());
        //parent() cannot be null
        if (data->parent()->isTopLevel())
            return engine()->nullValue();
        return data->parent()->asDataInformation()->toScriptValue(engine(), mHandlerInfo);
    }
    else if (name == s_datatype)
    {
        return data->typeName();
    }
    else if (name == s_updateFunc)
    {
        return data->updateFunc();
    }
    else if (name == s_validationFunc)
    {
        return data->validationFunc();
    }
    else if (name == s_validationError)
    {
        return data->validationError();
    }
    else if (name == s_byteOrder)
    {
        return ParserUtils::byteOrderToString(data->byteOrder());
    }
    else if (name == s_name)
    {
        return data->name();
    }
    QScriptValue other = additionalProperty(data, name, id);
    if (other.isValid())
        return other;
    else
    {
        data->logError() << "could not find property with name" << name.toString();
        return engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                QLatin1String("Cannot read property ") + name.toString());
    }
}

void DefaultScriptClass::setDataType(const QScriptValue& value, DataInformation* data)
{
    DataInformation* thisObj = toDataInformation(engine()->currentContext()->thisObject());
    Q_CHECK_PTR(thisObj);
    const bool isThisObj = thisObj == data;
    //this object always has mHasBeenUpdated set just before calling updateFunc, so in that case it is okay
    if (data->hasBeenUpdated() && !isThisObj)
    {
        //this element has already been updated (and probably read, replacing it could cause crazy errors
        data->logError() << "Attempting to replace an already updated object. This could cause errors."
                "Current this object: " << (thisObj ? thisObj->fullObjectPath() : QString());
        return;
    }
    //change the type of the underlying object
    DataInformation* newType = ScriptValueConverter::convert(value, data->name(), data->logger(), data);
    if (!newType)
    {
        data->logError() << "Failed to set new type, could not convert value!";
        return;
    }

    DataInformationBase* parent = data->parent();
    Q_CHECK_PTR(parent);
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_CHECK_PTR(top);
    //only if parent is toplevel, struct or union, can we replace
    bool replaced = false;
    if (parent->isTopLevel())
    {
        Q_ASSERT(isThisObj); //we can only do this if we are currently at the top level element
        parent->asTopLevel()->setActualDataInformation(newType);
        replaced = true;
    }
    else if (parent->isStruct())
    {
        StructureDataInformation* stru = parent->asStruct();
        int index = stru->indexOf(data);
        Q_ASSERT(index != -1);
        Q_ASSERT(uint(index) < stru->childCount());
        replaced = stru->replaceChildAt(index, newType);
        if (!replaced)
            stru->logError() << "failed to replace child at index" << index;
    }
    else if (parent->isUnion())
    {
        UnionDataInformation* un = parent->asUnion();
        int index = un->indexOf(data);
        Q_ASSERT(index != -1);
        Q_ASSERT(uint(index) < un->childCount());
        replaced = un->replaceChildAt(index, newType);
        if (!replaced)
            un->logError() << "failed to replace child at index" << index;
    }
    else if (parent->isPointer())
    {
        parent->asPointer()->setPointerTarget(newType);
        replaced = true;
    }
    else
    {
        data->logError() << "Failed to set data type since element is not toplevel and parent"
                " is neither struct nor union nor pointer.";
    }
    if (replaced)
    {
        top->setChildDataChanged();
        //if the current object was "this" in javascript we have to replace it
        if (isThisObj)
            engine()->currentContext()->setThisObject(newType->toScriptValue(engine(), mHandlerInfo));
    }
    else
    {
        delete newType; //could not set new type
    }
    newType->mHasBeenUpdated = true;
}

void DefaultScriptClass::setProperty(QScriptValue& object, const QScriptString& name, uint id, const QScriptValue& value)
{
    const ScriptHandlerInfo::Mode mode = mHandlerInfo->mode();
    Q_ASSERT(mode != ScriptHandlerInfo::None);
    DataInformation* data = toDataInformation(object);
    if (!data)
    {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.data().toString();
        engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                QLatin1String("Attempting to access an invalid object"));
        return;
    }
    if (mode == ScriptHandlerInfo::Validating)
    {
        //only way write access is allowed is when validating: valid and validationError
        if (data->hasBeenValidated())
            data->logError() << "Cannot modify this object, it has already been validated!";
        else if (name == s_valid)
            data->mValidationSuccessful = value.toBool();
        else if (name == s_validationError)
            data->setValidationError(value.toString());
        else
            data->logError() << "Cannot write to property" << name.toString() << "while validating!";
        return;
    }

    if (mode != ScriptHandlerInfo::Updating)
    {
        data->logError() << "Writing to property" << name.toString() << "is only allowed when updating.";
        return;
    }
    Q_ASSERT(mode == ScriptHandlerInfo::Updating);

    if (name == s_byteOrder)
    {
        data->setByteOrder(ParserUtils::byteOrderFromString(value.toString(),
                LoggerWithContext(data->logger(), data->fullObjectPath())));
    }
    else if (name == s_datatype)
    {
        //change the type of the underlying object
        setDataType(value, data);
    }
    else if (name == s_updateFunc)
    {
        data->setUpdateFunc(value);
    }
    else if (name == s_validationFunc)
    {
        data->setValidationFunc(value);
    }
    else if (name == s_name)
    {
        data->setName(value.toString());
    }
    else if (name == s_wasAbleToRead || name == s_valid || name == s_validationError)
    {
        data->logError() << "Writing to property " << s_valid.toString() << "is not allowed when updating.";
        return; //can't write
    }
    else
    {
        bool setAdditional = setAdditionalProperty(data, name, id, value);
        if (setAdditional)
            return;
        else
        {
            data->logError() << "could not set property with name" << name.toString();
            engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                QLatin1String("Cannot write property ") + name.toString());
        }
    }
}

QScriptValue::PropertyFlags DefaultScriptClass::propertyFlags(const QScriptValue& object, const QScriptString& name, uint id)
{
    QScriptValue::PropertyFlags result;
    const ScriptHandlerInfo::Mode mode = mHandlerInfo->mode();
    Q_ASSERT(mode != ScriptHandlerInfo::None);
    DataInformation* data = toDataInformation(object);
    if (!data)
    {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.data().toString();
        engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                QLatin1String("Attempting to access an invalid object"));
        return 0;
    }
    if (name == s_valid || name == s_validationError)
    {
        if (mode != ScriptHandlerInfo::Validating)
            result |= QScriptValue::ReadOnly;
    }
    else if (mode != ScriptHandlerInfo::Updating)
    {
        result |= QScriptValue::ReadOnly;
    }

    for (int i = 0, size = mIterableProperties.size(); i < size; ++i) {
        if (mIterableProperties.at(i).first == name)
            return result | mIterableProperties.at(i).second;
    }
    if (additionalPropertyFlags(data, name, id, &result))
        return result; //is a child element
    else
    {
        data->logError() << "could not find flags for property with name" << name.toString();
        return 0;
    }
}

QScriptValue DefaultScriptClass::prototype() const
{
    return mDefaultPrototype;
}

QScriptValue DefaultScriptClass::Default_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = toDataInformation(ctx->thisObject());
    if (!data)
    {
        kWarning() << "could not cast data";
        return eng->undefinedValue();
    }
    return QString(data->typeName() + QLatin1Char(' ') + data->name());
}


QScriptClassPropertyIterator* DefaultScriptClass::newIterator(const QScriptValue& object)
{
    return new DefaultscriptClassIterator(object, this);
}

DefaultscriptClassIterator::DefaultscriptClassIterator(const QScriptValue& object, DefaultScriptClass* cls)
        : QScriptClassPropertyIterator(object), mCurrent(-1), mClass(cls)
{
    DataInformation* data = DefaultScriptClass::toDataInformation(object);
    Q_CHECK_PTR(data);
    mData = data;
}

DefaultscriptClassIterator::~DefaultscriptClassIterator()
{
}

bool DefaultscriptClassIterator::hasNext() const
{
    return mCurrent < mClass->mIterableProperties.size() - 1;
}

bool DefaultscriptClassIterator::hasPrevious() const
{
    return mCurrent > 0;
}

QScriptString DefaultscriptClassIterator::name() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mClass->mIterableProperties.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mClass->mIterableProperties.size() + mData->childCount())
        return QScriptString();
    if (mCurrent < mClass->mIterableProperties.size())
        return mClass->mIterableProperties.at(mCurrent).first;
    int index = mCurrent - mClass->mIterableProperties.size();
    Q_ASSERT(index >= 0);
    DataInformation* child = mData->childAt(index);
    return mClass->engine()->toStringHandle(child->name());
}

QScriptValue::PropertyFlags DefaultscriptClassIterator::flags() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mClass->mIterableProperties.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mClass->mIterableProperties.size() + mData->childCount())
        return 0;
    if (mCurrent < mClass->mIterableProperties.size())
        return mClass->propertyFlags(object(), mClass->mIterableProperties.at(mCurrent).first, id());
    return QScriptValue::ReadOnly;
}

uint DefaultscriptClassIterator::id() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mClass->mIterableProperties.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mClass->mIterableProperties.size() + mData->childCount())
        return 0;
    //only children have an id assigned
    if (mCurrent < mClass->mIterableProperties.size())
        return 0;
    return mCurrent - mClass->mIterableProperties.size() + 1;
}

void DefaultscriptClassIterator::next()
{
    Q_ASSERT(mCurrent == -1 || (uint)mCurrent < mClass->mIterableProperties.size() + mData->childCount());
    mCurrent++;
}

void DefaultscriptClassIterator::previous()
{
    Q_ASSERT(mCurrent >= 0);
    mCurrent--;
}

void DefaultscriptClassIterator::toBack()
{
    mCurrent = mClass->mIterableProperties.size() + mData->childCount();
}

void DefaultscriptClassIterator::toFront()
{
    mCurrent = -1;
}



