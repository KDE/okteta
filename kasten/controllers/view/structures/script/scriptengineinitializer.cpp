/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011, 2012, 2013 Alex Richardson <alex.richardson@gmx.de>
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
#include "../parsers/parserutils.h"
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

    //enum is a reserved keyword in JavaScript, cannot use it
    engine->globalObject().setProperty(QLatin1String("enumeration"),
            engine->newFunction(Private::scriptNewEnum));
    engine->globalObject().setProperty(QLatin1String("flags"),
            engine->newFunction(Private::scriptNewFlags));
    engine->globalObject().setProperty(QLatin1String("string"),
            engine->newFunction(Private::scriptNewString));
    engine->globalObject().setProperty(QLatin1String("pointer"),
            engine->newFunction(Private::scriptNewPointer));
    engine->globalObject().setProperty(QLatin1String("taggedUnion"),
            engine->newFunction(Private::scriptNewTaggedUnion));

    engine->globalObject().setProperty(QLatin1String("alternative"),
            engine->newFunction(Private::alternativeFunc));
}

QScriptEngine* newEngine()
{
    QScriptEngine* ret = new QScriptEngine();
    addFuctionsToScriptEngine(ret);
    return ret;
}

namespace Private
{

static const QString setUpdatePropertyString = QLatin1String("setUpdate");
static const QString setValidationPropertyString = QLatin1String("setValidation");
static const QString setPropertyString = QLatin1String("set");

namespace
{

QScriptValue scriptNewCommon(QScriptContext* ctx, QScriptEngine* eng, const QString& typeName)
{
    QScriptValue object = ctx->isCalledAsConstructor() ? ctx->thisObject() : eng->newObject();
    object.setProperty(ParserStrings::PROPERTY_INTERNAL_TYPE, typeName);
    //add the setUpdate() and setValidation() functions
    object.setProperty(setUpdatePropertyString, eng->newFunction(addUpdateFunc, 1));
    object.setProperty(setValidationPropertyString, eng->newFunction(addValidationFunc, 1));
    object.setProperty(setPropertyString, eng->newFunction(addCustomPropertiesFunc, 1));
    return object;
}

/** create a new primitive of type @p type */
QScriptValue primitiveConstructor(QScriptContext* ctx, QScriptEngine* eng, const QString& type)
{
    QScriptValue object = scriptNewCommon(ctx, eng, ParserStrings::TYPE_PRIMITIVE);
    object.setProperty(ParserStrings::PROPERTY_TYPE, type);
    return object;
}

}
#define PRIMITIVE_CONSTRUCTOR(type) QScriptValue scriptNew##type(QScriptContext* ctx, QScriptEngine* eng) \
        { return primitiveConstructor(ctx, eng, QLatin1String(#type)); }

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
    QScriptValue object = scriptNewCommon(ctx, eng, ParserStrings::TYPE_BITFIELD);

    object.setProperty(ParserStrings::PROPERTY_TYPE, ctx->argument(0)); //first argument is type
    object.setProperty(ParserStrings::PROPERTY_WIDTH, ctx->argument(1)); //second argument is width
    return object;
}

//with children:
QScriptValue scriptNewStruct(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, ParserStrings::TYPE_STRUCT);
    object.setProperty(ParserStrings::PROPERTY_CHILD, eng->newFunction(getChild));

    object.setProperty(ParserStrings::PROPERTY_CHILDREN, ctx->argument(0)); //first argument is children
    return object;
}

QScriptValue scriptNewUnion(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, ParserStrings::TYPE_UNION);
    object.setProperty(ParserStrings::PROPERTY_TYPE, eng->newFunction(getChild));

    object.setProperty(ParserStrings::PROPERTY_CHILDREN, ctx->argument(0)); //first argument is children
    return object;
}

QScriptValue scriptNewArray(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, ParserStrings::TYPE_ARRAY);

    object.setProperty(ParserStrings::PROPERTY_TYPE, ctx->argument(0)); //first argument is child type
    object.setProperty(ParserStrings::PROPERTY_LENGTH, ctx->argument(1)); //second argument is length
    return object;
}

QScriptValue createEnumObject(QScriptContext* ctx, QScriptEngine* eng, const QString& typeName)
{
    QScriptValue object = scriptNewCommon(ctx, eng, typeName);

    object.setProperty(ParserStrings::PROPERTY_ENUM_NAME, ctx->argument(0)); //first argument is the name of the underlying enum
    object.setProperty(ParserStrings::PROPERTY_TYPE, ctx->argument(1)); //second argument is the type of the enum
    object.setProperty(ParserStrings::PROPERTY_ENUM_VALUES, ctx->argument(2)); //third argument is the enum values
    return object;
}

QScriptValue scriptNewEnum(QScriptContext* ctx, QScriptEngine* eng)
{
    return createEnumObject(ctx, eng, ParserStrings::TYPE_ENUM);
}

QScriptValue scriptNewFlags(QScriptContext* ctx, QScriptEngine* eng)
{
    return createEnumObject(ctx, eng, ParserStrings::TYPE_FLAGS);
}

QScriptValue scriptNewString(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, ParserStrings::TYPE_STRING);

    object.setProperty(ParserStrings::PROPERTY_ENCODING, ctx->argument(0));
    return object;
}

QScriptValue scriptNewPointer(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, ParserStrings::TYPE_POINTER);

    object.setProperty(ParserStrings::PROPERTY_TYPE, ctx->argument(0));
    object.setProperty(ParserStrings::PROPERTY_TARGET, ctx->argument(1));
    return object;
}

QScriptValue scriptNewTaggedUnion(QScriptContext* ctx, QScriptEngine* eng)
{
    QScriptValue object = scriptNewCommon(ctx, eng, ParserStrings::TYPE_TAGGED_UNION);

    object.setProperty(ParserStrings::PROPERTY_CHILDREN, ctx->argument(0));
    object.setProperty(ParserStrings::PROPERTY_ALTERNATIVES, ctx->argument(1));
    object.setProperty(ParserStrings::PROPERTY_DEFAULT_CHILDREN, ctx->argument(2));
    return object;
}

QScriptValue getChild(QScriptContext* ctx, QScriptEngine* eng)
{
    Q_UNUSED(eng)
    if (ctx->argumentCount() < 1)
        return ctx->throwError(QLatin1String("child(): name of child must be passed as first parameter"));
    QString nameString = ctx->argument(0).toString();
    QScriptValue ret = ctx->thisObject().property(ParserStrings::PROPERTY_CHILDREN).property(nameString);
    if (ret.isValid())
        return ret;
    else
        return ctx->throwError(
                QString(QLatin1String("child(): could not find child with name=") + nameString));
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
    thisObj.setProperty(ParserStrings::PROPERTY_UPDATE_FUNC, func);
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
    thisObj.setProperty(ParserStrings::PROPERTY_VALIDATION_FUNC, func);
    return thisObj;
}

QScriptValue addCustomPropertiesFunc(QScriptContext* ctx, QScriptEngine*)
{
    if (ctx->argumentCount() != 1)
        return ctx->throwError(QLatin1String("set(): needs one argument!"));
    QScriptValue thisObj = ctx->thisObject();
    Q_ASSERT(thisObj.isValid());
    QScriptValue arg = ctx->argument(0);
    if (!arg.isValid() || !arg.isObject())
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String("set(): argument must be an object!"));
    int count = 0;
    QScriptValueIterator it(arg);
    while (it.hasNext())
    {
        it.next();
        thisObj.setProperty(it.scriptName(), it.value());
        count++;
    }
    if (count == 0)
        return ctx->throwError(QLatin1String("set(): must set at least one property!"));
    return thisObj;
}

QScriptValue alternativeFunc(QScriptContext* ctx, QScriptEngine* eng)
{
    if (ctx->argumentCount() < 2)
        return ctx->throwError(QLatin1String("alternative(): needs at least 2 arguments!"));
    QScriptValue object = ctx->isCalledAsConstructor() ? ctx->thisObject() : eng->newObject();
    object.setProperty(ParserStrings::PROPERTY_SELECT_IF, ctx->argument(0));
    object.setProperty(ParserStrings::PROPERTY_CHILDREN, ctx->argument(1));
    if (ctx->argumentCount() > 2)
        object.setProperty(ParserStrings::PROPERTY_STRUCT_NAME, ctx->argument(2));
    return object;
}

} //namespace Private

} //namespace ScriptEngine Initializer
