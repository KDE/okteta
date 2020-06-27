/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012, 2013 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "defaultscriptclass.hpp"

#include "../../datatypes/datainformation.hpp"
#include "../../datatypes/topleveldatainformation.hpp"
#include "../../datatypes/uniondatainformation.hpp"
#include "../../datatypes/structuredatainformation.hpp"
#include "../../datatypes/primitive/pointerdatainformation.hpp"
#include "../../parsers/parserutils.hpp"
#include "../../parsers/scriptvalueconverter.hpp"
#include "../scriptlogger.hpp"
#include "../scripthandlerinfo.hpp"
#include "../safereference.hpp"
#include "../../structlogging.hpp"

DefaultScriptClass::DefaultScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : QScriptClass(engine)
    , mHandlerInfo(handlerInfo)
{
    s_valid = engine->toStringHandle(ParserStrings::PROPERTY_VALID());
    s_wasAbleToRead = engine->toStringHandle(ParserStrings::PROPERTY_ABLE_TO_READ());
    s_validationError = engine->toStringHandle(ParserStrings::PROPERTY_VALIDATION_ERROR());
    s_parent = engine->toStringHandle(ParserStrings::PROPERTY_PARENT());
    s_byteOrder = engine->toStringHandle(ParserStrings::PROPERTY_BYTEORDER());
    s_name = engine->toStringHandle(ParserStrings::PROPERTY_NAME());
    s_datatype = engine->toStringHandle(ParserStrings::PROPERTY_DATATYPE());
    s_updateFunc = engine->toStringHandle(ParserStrings::PROPERTY_UPDATE_FUNC());
    s_validationFunc = engine->toStringHandle(ParserStrings::PROPERTY_VALIDATION_FUNC());
    s_customTypeName = engine->toStringHandle(ParserStrings::PROPERTY_CUSTOM_TYPE_NAME());
    s_asStringFunc = engine->toStringHandle(ParserStrings::PROPERTY_TO_STRING_FUNC());

    // TODO remove, every subclass should have proto
    mDefaultPrototype = engine->newObject();
    mDefaultPrototype.setProperty(QStringLiteral("toString"), engine->newFunction(Default_proto_toString));
    // add all our properties
    mIterableProperties.reserve(11);
    mIterableProperties.append(qMakePair(s_parent, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_name, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_wasAbleToRead, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_byteOrder, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_valid, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_validationError, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    mIterableProperties.append(qMakePair(s_validationFunc, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_updateFunc, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_datatype, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_customTypeName, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
    mIterableProperties.append(qMakePair(s_asStringFunc, QScriptValue::PropertyFlags(QScriptValue::Undeletable)));
}

DefaultScriptClass::~DefaultScriptClass() = default;

DataInformation* DefaultScriptClass::toDataInformation(const QScriptValue& obj)
{
    if (!obj.scriptClass()) {
        return nullptr;
    }
    Q_ASSERT(obj.data().isVariant());
    const QVariant variant = obj.data().toVariant();
    if (variant.isValid() && variant.canConvert<SafeReference>() && variant.userType() == qMetaTypeId<SafeReference>()) {
        const SafeReference& ref = *reinterpret_cast<const SafeReference*>(variant.constData());
        return ref.data();
    }
    return nullptr;
}

QScriptClass::QueryFlags DefaultScriptClass::queryProperty(const QScriptValue& object,
                                                           const QScriptString& name, QScriptClass::QueryFlags flags, uint* id)
{
    const ScriptHandlerInfo::Mode mode = mHandlerInfo->mode();
    Q_ASSERT(mode != ScriptHandlerInfo::Mode::None);
    DataInformation* data = toDataInformation(object);
    if (!data) {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.data().toString();
        engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                                               QStringLiteral("Attempting to access an invalid object"));
        return {};
    }
    if (name == s_valid || name == s_validationError) {
        return mode == ScriptHandlerInfo::Mode::Validating ? flags : flags& ~HandlesWriteAccess;
    }
    if (mode != ScriptHandlerInfo::Mode::Updating) {
        // the only properties that are possibly writable when not updating are valid and validationError
        // but we checked them before so we remove handlesWriteAccess from the flags
        flags &= ~HandlesWriteAccess;
    }

    if (name == s_byteOrder || name == s_name || name == s_updateFunc || name == s_validationFunc
        || name == s_datatype || name == s_customTypeName || name == s_asStringFunc) {
        return flags;
    }
    if (name == s_wasAbleToRead || name == s_parent) {
        return flags & ~HandlesWriteAccess;
    }
    if (queryAdditionalProperty(data, name, &flags, id)) {
        return flags;
    }

    data->logError() << "could not find property with name" << name.toString();
    engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                                               QLatin1String("Could not find property with name ") + name.toString());
    return {};
}

QScriptValue DefaultScriptClass::property(const QScriptValue& object, const QScriptString& name, uint id)
{
    Q_ASSERT(mHandlerInfo->mode() != ScriptHandlerInfo::Mode::None);
    DataInformation* data = toDataInformation(object);
    if (!data) {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.data().toString();
        return engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                                                      QStringLiteral("Attempting to access an invalid object"));
    }
    if (name == s_valid) {
        return data->validationSuccessful();
    }
    if (name == s_wasAbleToRead) {
        return data->wasAbleToRead();
    }
    if (name == s_parent) {
        Q_CHECK_PTR(data->parent());
        // parent() cannot be null
        if (data->parent()->isTopLevel()) {
            return engine()->nullValue();
        }
        return data->parent()->asDataInformation()->toScriptValue(engine(), mHandlerInfo);
    }
    if (name == s_datatype) {
        return data->typeName();
    }
    if (name == s_updateFunc) {
        return data->updateFunc();
    }
    if (name == s_validationFunc) {
        return data->validationFunc();
    }
    if (name == s_validationError) {
        return data->validationError();
    }
    if (name == s_byteOrder) {
        return ParserUtils::byteOrderToString(data->byteOrder());
    }
    if (name == s_name) {
        return data->name();
    }
    if (name == s_customTypeName) {
        return data->typeName();
    }
    if (name == s_asStringFunc) {
        return data->toStringFunction();
    }
    QScriptValue other = additionalProperty(data, name, id);
    if (other.isValid()) {
        return other;
    }
    data->logError() << "could not find property with name" << name.toString();
    return engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                                                  QLatin1String("Cannot read property ") + name.toString());
}

void DefaultScriptClass::setDataType(const QScriptValue& value, DataInformation* data)
{
    DataInformation* thisObj = toDataInformation(engine()->currentContext()->thisObject());
    Q_CHECK_PTR(thisObj);
    const bool isThisObj = thisObj == data;
    // this object always has mHasBeenUpdated set just before calling updateFunc, so in that case it is okay
    if (data->hasBeenUpdated() && !isThisObj) {
        // this element has already been updated (and probably read, replacing it could cause crazy errors
        data->logError() << "Attempting to replace an already updated object. This could cause errors."
            "Current this object: " << (thisObj ? thisObj->fullObjectPath() : QString());
        return;
    }
    // change the type of the underlying object
    DataInformation* newType = ScriptValueConverter::convert(value, data->name(), data->logger(), data);
    if (!newType) {
        data->logError() << "Failed to set new type, could not convert value!";
        return;
    }

    DataInformationBase* parent = data->parent();
    Q_CHECK_PTR(parent);
    TopLevelDataInformation* top = data->topLevelDataInformation();
    Q_CHECK_PTR(top);
    // only if parent is toplevel, struct or union, can we replace
    bool replaced = false;
    if (parent->isTopLevel()) {
        Q_ASSERT(isThisObj); // we can only do this if we are currently at the top level element
        parent->asTopLevel()->setActualDataInformation(newType);
        replaced = true;
    } else if (parent->isStruct()) {
        StructureDataInformation* stru = parent->asStruct();
        int index = stru->indexOf(data);
        Q_ASSERT(index != -1);
        Q_ASSERT(uint(index) < stru->childCount());
        replaced = stru->replaceChildAt(index, newType);
        if (!replaced) {
            stru->logError() << "failed to replace child at index" << index;
        }
    } else if (parent->isUnion()) {
        UnionDataInformation* un = parent->asUnion();
        int index = un->indexOf(data);
        Q_ASSERT(index != -1);
        Q_ASSERT(uint(index) < un->childCount());
        replaced = un->replaceChildAt(index, newType);
        if (!replaced) {
            un->logError() << "failed to replace child at index" << index;
        }
    } else if (parent->isPointer()) {
        parent->asPointer()->setPointerTarget(newType);
        replaced = true;
    } else {
        data->logError() << "Failed to set data type since element is not toplevel and parent"
            " is neither struct nor union nor pointer.";
    }
    if (replaced) {
        top->setChildDataChanged();
        // if the current object was "this" in javascript we have to replace it
        if (isThisObj) {
            engine()->currentContext()->setThisObject(newType->toScriptValue(engine(), mHandlerInfo));
        }
        newType->mHasBeenUpdated = true;
    } else {
        delete newType; // could not set new type
    }
}

void DefaultScriptClass::setProperty(QScriptValue& object, const QScriptString& name, uint id, const QScriptValue& value)
{
    const ScriptHandlerInfo::Mode mode = mHandlerInfo->mode();
    Q_ASSERT(mode != ScriptHandlerInfo::Mode::None);
    DataInformation* data = toDataInformation(object);
    if (!data) {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.data().toString();
        engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                                               QStringLiteral("Attempting to access an invalid object"));
        return;
    }
    if (mode == ScriptHandlerInfo::Mode::Validating) {
        // only way write access is allowed is when validating: valid and validationError
        if (data->hasBeenValidated()) {
            data->logError() << "Cannot modify this object, it has already been validated!";
        } else if (name == s_valid) {
            data->mValidationSuccessful = value.toBool();
        } else if (name == s_validationError) {
            data->setValidationError(value.toString());
        } else {
            data->logError() << "Cannot write to property" << name.toString() << "while validating!";
        }
        return;
    }

    if (mode != ScriptHandlerInfo::Mode::Updating) {
        data->logError() << "Writing to property" << name.toString() << "is only allowed when updating.";
        return;
    }
    Q_ASSERT(mode == ScriptHandlerInfo::Mode::Updating);

    if (name == s_byteOrder) {
        data->setByteOrder(ParserUtils::byteOrderFromString(value.toString(),
                                                            LoggerWithContext(data->logger(), data->fullObjectPath())));
    } else if (name == s_datatype) {
        // change the type of the underlying object
        setDataType(value, data);
    } else if (name == s_updateFunc) {
        data->setUpdateFunc(value);
    } else if (name == s_validationFunc) {
        data->setValidationFunc(value);
    } else if (name == s_name) {
        data->setName(value.toString());
    } else if (name == s_customTypeName) {
        if (!value.isValid() || value.isNull() || value.isUndefined()) {
            data->setCustomTypeName(QString()); // unset
        } else {
            data->setCustomTypeName(value.toString());
        }
    } else if (name == s_asStringFunc) {
        data->setToStringFunction(value);
    } else {
        bool setAdditional = setAdditionalProperty(data, name, id, value);
        if (setAdditional) {
            return;
        }
        data->logError() << "could not set property with name" << name.toString();
        engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                                               QLatin1String("Cannot write property ") + name.toString());
    }
}

QScriptValue::PropertyFlags DefaultScriptClass::propertyFlags(const QScriptValue& object, const QScriptString& name, uint id)
{
    QScriptValue::PropertyFlags result;
    const ScriptHandlerInfo::Mode mode = mHandlerInfo->mode();
    Q_ASSERT(mode != ScriptHandlerInfo::Mode::None);
    DataInformation* data = toDataInformation(object);
    if (!data) {
        mHandlerInfo->logger()->error() << "could not cast data from" << object.data().toString();
        engine()->currentContext()->throwError(QScriptContext::ReferenceError,
                                               QStringLiteral("Attempting to access an invalid object"));
        return {};
    }
    if (name == s_valid || name == s_validationError) {
        if (mode != ScriptHandlerInfo::Mode::Validating) {
            result |= QScriptValue::ReadOnly;
        }
    } else if (mode != ScriptHandlerInfo::Mode::Updating) {
        result |= QScriptValue::ReadOnly;
    }

    for (const auto& property : qAsConst(mIterableProperties)) {
        if (property.first == name) {
            return result | property.second;
        }
    }

    if (additionalPropertyFlags(data, name, id, &result)) {
        return result; // is a child element
    }
    data->logError() << "could not find flags for property with name" << name.toString();
    return {};
}

QScriptValue DefaultScriptClass::prototype() const
{
    return mDefaultPrototype;
}

QScriptValue DefaultScriptClass::Default_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = toDataInformation(ctx->thisObject());
    if (!data) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not cast data";
        return eng->undefinedValue();
    }
    return QString(data->typeName() + QLatin1Char(' ') + data->name());
}

QScriptClassPropertyIterator* DefaultScriptClass::newIterator(const QScriptValue& object)
{
    return new DefaultscriptClassIterator(object, this);
}

DefaultscriptClassIterator::DefaultscriptClassIterator(const QScriptValue& object, DefaultScriptClass* cls)
    : QScriptClassPropertyIterator(object)
    , mClass(cls)
{
    DataInformation* data = DefaultScriptClass::toDataInformation(object);
    Q_CHECK_PTR(data);
    mData = data;
}

DefaultscriptClassIterator::~DefaultscriptClassIterator() = default;

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
    if (mCurrent < 0 || (uint)mCurrent >= mClass->mIterableProperties.size() + mData->childCount()) {
        return {};
    }
    if (mCurrent < mClass->mIterableProperties.size()) {
        return mClass->mIterableProperties.at(mCurrent).first;
    }
    int index = mCurrent - mClass->mIterableProperties.size();
    Q_ASSERT(index >= 0);
    DataInformation* child = mData->childAt(index);
    return mClass->engine()->toStringHandle(child->name());
}

QScriptValue::PropertyFlags DefaultscriptClassIterator::flags() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mClass->mIterableProperties.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mClass->mIterableProperties.size() + mData->childCount()) {
        return {};
    }
    if (mCurrent < mClass->mIterableProperties.size()) {
        return mClass->propertyFlags(object(), mClass->mIterableProperties.at(mCurrent).first, id());
    }
    return QScriptValue::ReadOnly;
}

uint DefaultscriptClassIterator::id() const
{
    Q_ASSERT(mCurrent >= 0 && (uint)mCurrent < mClass->mIterableProperties.size() + mData->childCount());
    if (mCurrent < 0 || (uint)mCurrent >= mClass->mIterableProperties.size() + mData->childCount()) {
        return 0;
    }
    // only children have an id assigned
    if (mCurrent < mClass->mIterableProperties.size()) {
        return 0;
    }
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
