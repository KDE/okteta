/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structunionscriptclass.hpp"
#include "../../datatypes/datainformationwithchildren.hpp"
#include "../../datatypes/topleveldatainformation.hpp"
#include "../../parsers/parserutils.hpp"
#include "../scriptlogger.hpp"
#include <structureslogging.hpp>

StructUnionScriptClass::StructUnionScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
    : DefaultScriptClass(engine, handlerInfo)
{
    s_childCount = engine->toStringHandle(ParserStrings::PROPERTY_CHILD_COUNT()); // read-only
    mIterableProperties.append(qMakePair(s_childCount, QScriptValue::ReadOnly | QScriptValue::Undeletable));
    s_children = engine->toStringHandle(ParserStrings::PROPERTY_CHILDREN()); // write-only

    mStructUnionPrototype = engine->newObject();
    mStructUnionPrototype.setProperty(QStringLiteral("toString"),
                                      engine->newFunction(StructUnion_proto_toString));
    mStructUnionPrototype.setProperty(QStringLiteral("setChildren"),
                                      engine->newFunction(StructUnion_proto_setChildren));
    mStructUnionPrototype.setProperty(QStringLiteral("child"),
                                      engine->newFunction(StructUnion_proto_child));
}

StructUnionScriptClass::~StructUnionScriptClass() = default;

bool StructUnionScriptClass::queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QScriptClass::QueryFlags* flags, uint* id)
{
    // no need to modify flags since both read and write are handled
    if (name == s_childCount) {
        *flags &= ~HandlesWriteAccess;
        return true;
    }
    if (name == s_children) {
        *flags &= ~HandlesReadAccess;
        return true;
    }

    bool isArrayIndex;
    quint32 pos = name.toArrayIndex(&isArrayIndex);
    uint count = data->childCount();
    bool isValidChild = false;
    if (isArrayIndex && pos < count) {
        isValidChild = true;
    } else {
        // compare name, names that match special properties/functions will be
        // hidden since these were checked before
        QString objName = name.toString();
        for (uint i = 0; i < count; ++i) {
            if (objName == data->childAt(i)->name()) {
                isValidChild = true;
                pos = i;
                break;
            }
        }
    }
    if (isValidChild) {
        *id = pos + 1; // add 1 to distinguish from the default value of 0
        *flags &= ~HandlesWriteAccess; // writing is not yet supported
        return true;
    }
    return false; // not found
}

bool StructUnionScriptClass::additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint id, QScriptValue::PropertyFlags* flags)
{
    // no need to modify flags since both read and write are handled
    if (id != 0) {
        *flags |= QScriptValue::ReadOnly;
        return true;
    }
    // TODO is this necessary, will there be any way a child has no id set?
    // check named children
    QString objName = name.toString();
    uint count = data->childCount();
    for (uint i = 0; i < count; ++i) {
        DataInformation* child = data->childAt(i);
        Q_CHECK_PTR(child);
        if (objName == child->name()) {
            *flags |= QScriptValue::ReadOnly;
            return true;
        }
    }

    return false;
}

QScriptValue StructUnionScriptClass::additionalProperty(const DataInformation* data, const QScriptString& name, uint id)
{
    const auto* dataW = static_cast<const DataInformationWithChildren*>(data);
    // do a dynamic cast in debug mode to ensure the static cast was valid
    Q_CHECK_PTR(dynamic_cast<const DataInformationWithChildren*>(dataW));

    if (id != 0) {
        quint32 pos = id - 1;
        if (pos >= data->childCount()) {
            dataW->logError() << "attempting to access out of bounds child: index was" << pos
                              << ", maximum is" << (data->childCount() - 1);
            return engine()->currentContext()->throwError(QScriptContext::RangeError,
                                                          QStringLiteral("Attempting to access struct index %1, but length is %2").arg(
                                                              QString::number(pos), QString::number(data->childCount())));
        }
        Q_CHECK_PTR(data->childAt(pos));
        return data->childAt(pos)->toScriptValue(engine(), mHandlerInfo);
    }
    if (name == s_childCount) {
        return dataW->childCount();
    }
    if (name == s_children) {
        dataW->logError() << "attempting to read read-only property" << s_children.toString();
        return engine()->undefinedValue();
    }
    // TODO is this necessary, will there be any way a child has no id set?
    // TODO testing seems to indicate this is not necessary, will leave it thought until I'm sure
    // check named children
    QString objName = name.toString();
    uint count = data->childCount();
    for (uint i = 0; i < count; ++i) {
        DataInformation* child = data->childAt(i);
        Q_CHECK_PTR(child);
        if (objName == child->name()) {
            return child->toScriptValue(engine(), mHandlerInfo);
        }
    }
    return {};
}

bool StructUnionScriptClass::setAdditionalProperty(DataInformation* data, const QScriptString& name, uint, const QScriptValue& value)
{
    auto* dataW = static_cast<DataInformationWithChildren*>(data);
    // do a dynamic cast in debug mode to ensure the static cast was valid
    Q_CHECK_PTR(dynamic_cast<DataInformationWithChildren*>(dataW));

    if (name == s_children) {
        dataW->setChildren(value);
        return true;
    }
    // TODO set children!!
    return false;
}

QScriptValue StructUnionScriptClass::prototype() const
{
    return mStructUnionPrototype;
}

QScriptValue StructUnionScriptClass::StructUnion_proto_toString(QScriptContext* ctx, QScriptEngine* eng)
{
    DataInformation* data = toDataInformation(ctx->thisObject());
    if (!data) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not cast data";
        return eng->undefinedValue();
    }
    return data->typeName(); // TODO better toString
}

QScriptValue StructUnionScriptClass::StructUnion_proto_child(QScriptContext* ctx, QScriptEngine* eng)
{
    if (ctx->argumentCount() < 1) {
        ctx->throwError(QScriptContext::RangeError,
                        QStringLiteral("(struct/union).child(name) needs at least one argument"));
        return eng->undefinedValue();
    }
    QScriptValue arg = ctx->argument(0);
    if (!arg.isString()) {
        ctx->throwError(QScriptContext::TypeError,
                        QStringLiteral("(struct/union).child(name) argument has to be a string"));
        return QScriptValue::UndefinedValue;
    }
    DataInformation* data = toDataInformation(ctx->thisObject());
    if (!data) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not cast data";
        return eng->undefinedValue();
    }
    uint count = data->childCount();
    QString name = arg.toString();
    for (uint i = 0; i < count; i++) {
        DataInformation* child = data->childAt(i);
        if (child->name() == name) {
            return child->toScriptValue(eng, data->topLevelDataInformation()->scriptHandler()->handlerInfo());
        }
    }

    return eng->nullValue();
}

QScriptValue StructUnionScriptClass::StructUnion_proto_setChildren(QScriptContext* ctx, QScriptEngine* eng)
{
    if (ctx->argumentCount() < 1) {
        return ctx->throwError(QScriptContext::RangeError,
                               QStringLiteral("(struct/union).child(children) needs one argument"));
    }
    DataInformation* data = toDataInformation(ctx->thisObject());
    if (!data) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "could not cast data";
        return eng->undefinedValue();
    }
    auto* dataW = static_cast<DataInformationWithChildren*>(data);
    // do a dynamic cast in debug mode to ensure the static cast was valid
    Q_CHECK_PTR(dynamic_cast<DataInformationWithChildren*>(dataW));

    dataW->setChildren(ctx->argument(0));
    return eng->undefinedValue();
}
