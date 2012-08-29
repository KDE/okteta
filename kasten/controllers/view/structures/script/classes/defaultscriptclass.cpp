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

#include "../../parsers/parserutils.h"
#include "../../datatypes/datainformation.h"
#include "../scriptlogger.h"

DefaultScriptClass::DefaultScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : QScriptClass(engine), mHandlerInfo(handlerInfo)
{
    s_valid = engine->toStringHandle(QLatin1String("valid"));
    s_wasAbleToRead = engine->toStringHandle(QLatin1String("wasAbleToRead"));
    s_validationError = engine->toStringHandle(QLatin1String("validationError"));
    s_parent = engine->toStringHandle(QLatin1String("parent"));
    s_byteOrder = engine->toStringHandle(QLatin1String("byteOrder"));
    s_name = engine->toStringHandle(QLatin1String("name"));
    qScriptRegisterMetaType<DataInfPtr>(engine, DefaultScriptClass::toScriptValue, DefaultScriptClass::fromScriptValue);

    //TODO remove, every subclass should have proto
    mDefaultPrototype = engine->newObject();
    mDefaultPrototype.setProperty(QLatin1String("toString"), engine->newFunction(Default_proto_toString));
    //add all our properties
    //TODO make name writable
    mIterableProperties.append(qMakePair(s_parent, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_name, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_wasAbleToRead, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_byteOrder, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_valid, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_validationError, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
}

DefaultScriptClass::~DefaultScriptClass()
{
}

QScriptValue DefaultScriptClass::toScriptValue(QScriptEngine* eng, const DataInfPtr& data)
{
    QScriptValue obj = eng->newObject();
    obj.setData(eng->newVariant(QVariant::fromValue(data)));
    return obj;
}

void DefaultScriptClass::fromScriptValue(const QScriptValue& obj, DataInfPtr& data)
{
    data = qvariant_cast<DataInfPtr>(obj.data().toVariant());
}

QScriptClass::QueryFlags DefaultScriptClass::queryProperty(const QScriptValue& object,
            const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        kWarning() << "could not cast data from" << object.toString();
        return 0;
    }
    if (name == s_valid || name == s_validationError || name == s_byteOrder)
    {
        return flags;
    }
    else if (name == s_wasAbleToRead || name == s_parent || name == s_name)
    {
        return flags &= ~HandlesWriteAccess;
    }
    else if (name == s_parent)
    {
        return flags &= ~HandlesWriteAccess; //TODO allow write access
    }
    else if (queryAdditionalProperty(data, name, &flags, id))
    {
        return flags;
    }
    else
    {
        data->logger()->error(data) << "could not find property with name" << name.toString();
        return 0;
    }
}

QScriptValue DefaultScriptClass::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        kWarning() << "could not cast data from" << object.toString();
        return QScriptValue();
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
            return QScriptValue::NullValue;
        return data->parent()->asDataInformation()->toScriptValue(engine(), mHandlerInfo);
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
        data->logger()->error(data) << "could not find property with name" << name.toString();
        return engine()->undefinedValue();
    }
}

void DefaultScriptClass::setProperty(QScriptValue& object, const QScriptString& name, uint id, const QScriptValue& value)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return;
    }

    if (name == s_valid)
    {
        data->setValidationSuccessful(value.toBool());
    }
    else if (name == s_validationError)
    {
        data->setValidationError(value.toString());
    }
    else if (name == s_byteOrder)
    {
        ParserInfo parInfo(data->fullObjectPath(), data->logger(), 0);
        data->setByteOrder(ParserUtils::byteOrderFromString(value.toString(), parInfo));
    }
    else if (name == s_wasAbleToRead || name == s_name)
    {
        return; //can't write
    }
    else
    {
        //TODO access by name
        bool setAdditional = setAdditionalProperty(data, name, id, value);
        if (setAdditional)
            return;
        else
        {
            data->logger()->error(data) << "could not set property with name" << name.toString();
        }
    }
}

QScriptValue::PropertyFlags DefaultScriptClass::propertyFlags(const QScriptValue& object, const QScriptString& name, uint id)
{
    QScriptValue::PropertyFlags result;
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    if (!data)
    {
        kDebug() << "could not cast data";
        return result;
    }
    for (int i = 0, size = mIterableProperties.size(); i < size; ++i) {
        if (mIterableProperties.at(i).first == name)
            return mIterableProperties.at(i).second;
    }
    if (additionalPropertyFlags(data, name, id, &result))
        return result; //is a child element
    else
    {
        data->logger()->error(data) << "could not find flags for property with name" << name.toString();
        return 0;
    }
}

QScriptValue DefaultScriptClass::prototype() const
{
    return mDefaultPrototype;
}

QScriptValue DefaultScriptClass::Default_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(ctx->thisObject().data());
    if (!data)
    {
        kWarning() << "could not cast data";
        return eng->undefinedValue();
    }
    return QString(data->typeName() + QLatin1Char(' ') + data->name());
}


QScriptClassPropertyIterator* DefaultScriptClass::newIterator(const QScriptValue& object)
{
    return new DefaultscriptClassIterator(object, mIterableProperties, engine());
}

DefaultscriptClassIterator::DefaultscriptClassIterator(const QScriptValue& object, const DefaultScriptClass::PropertyInfoList& list,
                                                       QScriptEngine* engine)
    : QScriptClassPropertyIterator(object), mCurrent(-1), mList(list), mEngine(engine)
{
    DataInformation* data = qscriptvalue_cast<DataInformation*>(object.data());
    Q_CHECK_PTR(data);
    mData = data;
}

DefaultscriptClassIterator::~DefaultscriptClassIterator()
{
}

bool DefaultscriptClassIterator::hasNext() const
{
    return mCurrent < mList.size() - 1;
}

bool DefaultscriptClassIterator::hasPrevious() const
{
    return mCurrent > 0;
}

QScriptString DefaultscriptClassIterator::name() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mList.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mList.size() + mData->childCount())
        return QScriptString();
    if (mCurrent < mList.size())
        return mList.at(mCurrent).first;
    int index = mCurrent - mList.size();
    Q_ASSERT(index >= 0);
    DataInformation* child = mData->childAt(index);
    return mEngine->toStringHandle(child->name());
}

QScriptValue::PropertyFlags DefaultscriptClassIterator::flags() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mList.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mList.size() + mData->childCount())
        return 0;
    if (mCurrent < mList.size())
        return mList.at(mCurrent).second;
    return QScriptValue::ReadOnly;
}

uint DefaultscriptClassIterator::id() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mList.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mList.size() + mData->childCount())
        return 0;
    //only children have an id assigned
    if (mCurrent < mList.size())
        return 0;
    return mCurrent - mList.size() + 1;
}

void DefaultscriptClassIterator::next()
{
    Q_ASSERT(mCurrent == -1 || (uint)mCurrent < mList.size() + mData->childCount());
    mCurrent++;
}

void DefaultscriptClassIterator::previous()
{
    Q_ASSERT(mCurrent >= 0);
    mCurrent--;
}

void DefaultscriptClassIterator::toBack()
{
    mCurrent = mList.size() + mData->childCount();
}

void DefaultscriptClassIterator::toFront()
{
    mCurrent = -1;
}



