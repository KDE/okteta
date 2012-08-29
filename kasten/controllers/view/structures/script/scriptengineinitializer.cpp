/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#include "scriptengineinitializer.h"
#include "scriptutils.h"
#include "../datatypes/primitivefactory.h"
#include <QStringList>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptContext>
#include <QScriptValueIterator>

#include <KDebug>

namespace ScriptEngineInitializer
{
void addFuctionsToScriptEngine(QScriptEngine* engine)
{
    engine->globalObject().setProperty(QLatin1String("uint8"),
            engine->newFunction(Private::scriptNewUInt8));
    engine->globalObject().setProperty(QLatin1String("uint16"),
            engine->newFunction(Private::scriptNewUInt16));
    engine->globalObject().setProperty(QLatin1String("uint32"),
            engine->newFunction(Private::scriptNewUInt32));
    engine->globalObject().setProperty(QLatin1String("uint64"),
            engine->newFunction(Private::scriptNewUInt64));

    engine->globalObject().setProperty(QLatin1String("int8"),
            engine->newFunction(Private::scriptNewInt8));
    engine->globalObject().setProperty(QLatin1String("int16"),
            engine->newFunction(Private::scriptNewInt16));
    engine->globalObject().setProperty(QLatin1String("int32"),
            engine->newFunction(Private::scriptNewInt32));
    engine->globalObject().setProperty(QLatin1String("int64"),
            engine->newFunction(Private::scriptNewInt64));

    engine->globalObject().setProperty(QLatin1String("bool8"),
            engine->newFunction(Private::scriptNewBool8));
    engine->globalObject().setProperty(QLatin1String("bool16"),
            engine->newFunction(Private::scriptNewBool16));
    engine->globalObject().setProperty(QLatin1String("bool32"),
            engine->newFunction(Private::scriptNewBool32));
    engine->globalObject().setProperty(QLatin1String("bool64"),
            engine->newFunction(Private::scriptNewBool64));

    engine->globalObject().setProperty(QLatin1String("float"),
            engine->newFunction(Private::scriptNewFloat));
    engine->globalObject().setProperty(QLatin1String("double"),
            engine->newFunction(Private::scriptNewDouble));

    engine->globalObject().setProperty(QLatin1String("char"),
            engine->newFunction(Private::scriptNewChar));

    engine->globalObject().setProperty(QLatin1String("bitfield"),
            engine->newFunction(Private::scriptNewBitfield));

    engine->globalObject().setProperty(QLatin1String("array"),
            engine->newFunction(Private::scriptNewArray));
    engine->globalObject().setProperty(QLatin1String("struct"),
            engine->newFunction(Private::scriptNewStruct));
    engine->globalObject().setProperty(QLatin1String("union"),
            engine->newFunction(Private::scriptNewUnion));

    //if I use enum QtScript gives me syntax errors, I thought enum was not a keyword in JS
    engine->globalObject().setProperty(QLatin1String("enumeration"),
            engine->newFunction(Private::scriptNewEnum));
    engine->globalObject().setProperty(QLatin1String("flags"),
            engine->newFunction(Private::scriptNewFlags));
    engine->globalObject().setProperty(QLatin1String("string"),
            engine->newFunction(Private::scriptNewString));
}

QScriptEngine* newEngine()
{
    QScriptEngine* ret = new QScriptEngine();
    addFuctionsToScriptEngine(ret);
    return ret;
}


namespace Private
{

static const QString typePropertyString = QLatin1String("type");
static const QString setUpdatePropertyString = QLatin1String("setUpdate");
static const QString setValidationPropertyString = QLatin1String("setValidation");

QScriptValue scriptNewCommon(QScriptContext* ctx, QScriptEngine* eng, QLatin1String typeName)
{
    QScriptValue object = ctx->isCalledAsConstructor() ? ctx->thisObject() : eng->newObject();
    object.setProperty(typePropertyString, typeName);
    //add the setUpdate() and setValidation() functions
    object.setProperty(setUpdatePropertyString, eng->newFunction(addUpdateFunc, 1));
    object.setProperty(setValidationPropertyString, eng->newFunction(addValidationFunc, 1));
    return object;
}

/** create a new primitive of type @p type */
QScriptValue primitiveConstructor(QScriptContext* ctx, QScriptEngine* eng, const char* type)
{
    QScriptValue object = scriptNewCommon(ctx, eng, QLatin1String(type));
    return object;
}

#define PRIMITIVE_CONSTRUCTOR(type) QScriptValue scriptNew##type(QScriptContext* ctx, QScriptEngine* eng) \
        { return primitiveConstructor(ctx, eng, #type); }

PRIMITIVE_CONSTRUCTOR(UInt8)
PRIMITIVE_CONSTRUCTOR(UInt16)
PRIMITIVE_CONSTRUCTOR(UInt32)
PRIMITIVE_CONSTRUCTOR(UInt64)
PRIMITIVE_CONSTRUCTOR(Int8)
PRIMITIVE_CONSTRUCTOR(Int16)
PRIMITIVE_CONSTRUCTOR(Int32)
PRIMITIVE_CONSTRUCTOR(Int64)
PRIMITIVE_CONSTRUCTOR(Bool8)
PRIMITIVE_CONSTRUCTOR(Bool16)
PRIMITIVE_CONSTRUCTOR(Bool32)
PRIMITIVE_CONSTRUCTOR(Bool64)
PRIMITIVE_CONSTRUCTOR(Float)
PRIMITIVE_CONSTRUCTOR(Double)
PRIMITIVE_CONSTRUCTOR(Char)

#undef PRIMITIVE_CONSTRUCTOR

QScriptValue scriptNewBitfield(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, QLatin1String("bitfield"));

    object.setProperty(QLatin1String("bitfieldType"), ctx->argument(0)); //first argument is type
    object.setProperty(QLatin1String("width"), ctx->argument(1)); //second argument is width
    return object;
}

//with children:
QScriptValue scriptNewStruct(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, QLatin1String("struct"));
    object.setProperty(QLatin1String("child"), eng->newFunction(getChild));

    object.setProperty(QLatin1String("children"), ctx->argument(0)); //first argument is children
    return object;
}

QScriptValue scriptNewUnion(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, QLatin1String("union"));
    object.setProperty(QLatin1String("child"), eng->newFunction(getChild));

    object.setProperty(QLatin1String("children"), ctx->argument(0)); //first argument is children
    return object;
}

QScriptValue scriptNewArray(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, QLatin1String("array"));

    object.setProperty(QLatin1String("childType"), ctx->argument(0)); //first argument is child type
    object.setProperty(QLatin1String("length"), ctx->argument(1)); //second argument is length
    return object;
}

QScriptValue createEnumObject(QScriptContext* ctx, QScriptEngine* eng, QLatin1String typeName)
{
    QScriptValue object = scriptNewCommon(ctx, eng, typeName);

    object.setProperty(QLatin1String("enumName"), ctx->argument(0)); //first argument is the name of the underlying enum
    QScriptValue enumType = ctx->argument(1);
    if (!enumType.isString() && enumType.isObject())
        enumType = enumType.property(typePropertyString);
    object.setProperty(QLatin1String("enumType"), enumType); //second argument is the type of the enum
    object.setProperty(QLatin1String("enumValues"), ctx->argument(2)); //third argument is the enum values
    return object;
}

QScriptValue scriptNewEnum(QScriptContext* ctx, QScriptEngine* eng)
{
    return createEnumObject(ctx, eng, QLatin1String("enum"));
}

QScriptValue scriptNewFlags(QScriptContext* ctx, QScriptEngine* eng)
{
    return createEnumObject(ctx, eng, QLatin1String("flags"));
}

QScriptValue scriptNewString(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, QLatin1String("string"));

    object.setProperty(QLatin1String("encoding"), ctx->argument(0));
    return object;
}

QScriptValue getChild(QScriptContext* ctx, QScriptEngine* eng)
{
    if (ctx->argumentCount() < 1)
        return ctx->throwError(QLatin1String("child(): name of child must be passed as first parameter"));
    QString nameString = ctx->argument(0).toString();
    QScriptValue ret = ctx->thisObject().property(QLatin1String("children")).property(nameString);
    if (ret.isValid())
        return ret;
    else
        return ctx->throwError(QString(QLatin1String("child(): could not find child with name=") + nameString));
}

QScriptValue addUpdateFunc(QScriptContext* ctx, QScriptEngine*)
{
    if (ctx->argumentCount() != 1)
        return ctx->throwError(QLatin1String("setUpdate(): needs one argument!"));
    QScriptValue thisObj = ctx->thisObject();
    Q_ASSERT(thisObj.isValid());
    QScriptValue func = ctx->argument(0);
    if (!func.isFunction())
    {
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String("setUpdate(): argument must be a function!"));
    }
    thisObj.setProperty(QLatin1String("updateFunc"), func);
    return thisObj;
}

QScriptValue addValidationFunc(QScriptContext* ctx, QScriptEngine*)
{
    if (ctx->argumentCount() != 1)
        return ctx->throwError(QLatin1String("setValidation(): needs one argument!"));
    QScriptValue thisObj = ctx->thisObject();
    Q_ASSERT(thisObj.isValid());
    QScriptValue func = ctx->argument(0);
    if (!func.isFunction())
    {
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String("setValidation(): argument must be a function!"));
    }
    thisObj.setProperty(QLatin1String("validationFunc"), func);
    return thisObj;
}

} //namespace Private

} //namespace ScriptEngine Initializer
