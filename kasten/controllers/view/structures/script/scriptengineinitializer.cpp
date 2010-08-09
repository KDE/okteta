/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include <QtCore/QStringList>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValueIterator>

#include <KDebug>

const QString ScriptEngineInitializer::typePropertyString("type");
const QString ScriptEngineInitializer::toStringPropertyString("toString");

/** create a new primitive of type @p type */
QScriptValue ScriptEngineInitializer::primitiveConstructor(QScriptContext* ctx,
        QScriptEngine* eng, const QLatin1String type)
{
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();
    object.setProperty(typePropertyString, type);
    object.setProperty(toStringPropertyString, eng->newFunction(primitiveToString));

    //add validation and update function
    if (ctx->argumentCount() > 0)
        addValidationFunction(ctx, eng, object, 0);
    return object;
}

QScriptValue ScriptEngineInitializer::scriptNewUInt8(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("UInt8"));
}
QScriptValue ScriptEngineInitializer::scriptNewUInt16(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("UInt16"));
}
QScriptValue ScriptEngineInitializer::scriptNewUInt32(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("UInt32"));
}
QScriptValue ScriptEngineInitializer::scriptNewUInt64(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("UInt64"));
}

QScriptValue ScriptEngineInitializer::scriptNewInt8(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Int8"));
}
QScriptValue ScriptEngineInitializer::scriptNewInt16(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Int16"));
}
QScriptValue ScriptEngineInitializer::scriptNewInt32(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Int32"));
}
QScriptValue ScriptEngineInitializer::scriptNewInt64(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Int64"));
}

QScriptValue ScriptEngineInitializer::scriptNewBool8(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Bool8"));
}
QScriptValue ScriptEngineInitializer::scriptNewBool16(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Bool16"));
}
QScriptValue ScriptEngineInitializer::scriptNewBool32(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Bool32"));
}
QScriptValue ScriptEngineInitializer::scriptNewBool64(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Bool64"));
}

QScriptValue ScriptEngineInitializer::scriptNewFloat(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Float"));
}
QScriptValue ScriptEngineInitializer::scriptNewDouble(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Double"));
}

QScriptValue ScriptEngineInitializer::scriptNewChar(QScriptContext* ctx,
        QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Char"));
}

QScriptValue ScriptEngineInitializer::scriptNewBitfield(QScriptContext* ctx,
        QScriptEngine* eng)
{
    //arg1 is type
    //arg2 is width
    //check arguments first
    if (ctx->argumentCount() < 2)
        return ctx->throwError("bitfield initializer takes at least two arguments.");
    if (!ctx->argument(1).isNumber())
        return ctx->throwError(QScriptContext::TypeError,
                "bitfield(): second argument is not a number");
    QString typeArg = ctx->argument(0).toString();
    if (typeArg != "bool" && typeArg != "unsigned" && typeArg != "signed"
            && !typeArg.isEmpty())
        return ctx->throwError("type must be 'bool', 'unsigned' 'signed'"
            " or '', actual value was '" + typeArg + '\'');

    //all arguments are valid
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    object.setProperty(typePropertyString, "bitfield");
    //has been validated, so it must contain valid values
    object.setProperty("bitfieldType", typeArg.isEmpty() ? "signed" : typeArg);
    object.setProperty("width", ctx->argument(1));
    object.setProperty(toStringPropertyString, eng->newFunction(bitfieldToString));

    //add validation and update function
    if (ctx->argumentCount() > 2)
        addValidationFunction(ctx, eng, object, 2);
    if (ctx->argumentCount() > 3)
        addUpdateFunction(ctx, eng, object, 3);

    return object;
}

//with children:
QScriptValue ScriptEngineInitializer::scriptNewStruct(QScriptContext* ctx,
        QScriptEngine* eng)
{
    //takes one argument, the children
    if (ctx->argumentCount() < 1)
        return ctx->throwError("struct initializer takes at least one argument.");
    QScriptValue children = ctx->argument(0);
    if (!children.isObject())
        return ctx->throwError(QScriptContext::TypeError,
                "struct(): argument is not an object");

    //all arguments are valid
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    object.setProperty(typePropertyString, "struct");
    object.setProperty("children", children);
    object.setProperty(toStringPropertyString, eng->newFunction(structToString));

    //add validation and update function
    if (ctx->argumentCount() > 1)
        addValidationFunction(ctx, eng, object, 1);
    if (ctx->argumentCount() > 2)
        addUpdateFunction(ctx, eng, object, 2);

    return object;
}

QScriptValue ScriptEngineInitializer::scriptNewUnion(QScriptContext* ctx,
        QScriptEngine* eng)
{
    //takes one argument, the children
    if (ctx->argumentCount() < 1)
        return ctx->throwError("union initializer takes at least one argument.");
    QScriptValue children = ctx->argument(0);
    if (!children.isObject())
        return ctx->throwError(QScriptContext::TypeError,
                "union(): argument is not an object");

    //all arguments are valid
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    object.setProperty(typePropertyString, "union");
    object.setProperty("children", children);
    object.setProperty(toStringPropertyString, eng->newFunction(unionToString));

    //add validation and update function
    if (ctx->argumentCount() > 1)
        addValidationFunction(ctx, eng, object, 1);
    if (ctx->argumentCount() > 2)
        addUpdateFunction(ctx, eng, object, 2);

    return object;
}

QScriptValue ScriptEngineInitializer::scriptNewArray(QScriptContext* ctx,
        QScriptEngine* eng)
{
    //arg 1 is child type, arg2 is length
    if (ctx->argumentCount() < 2)
        return ctx->throwError("array initializer takes at least two arguments.");
    QScriptValue childType = ctx->argument(0);
    if (!childType.isObject())
        return ctx->throwError(QScriptContext::TypeError,
                "array(): childType argument is not an object");
    QScriptValue length = ctx->argument(1);
    if (!length.isNumber())
        return ctx->throwError(QScriptContext::TypeError,
                "array(): length Type argument is not a number");

    //all arguments are valid
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    object.setProperty(typePropertyString, "array");
    object.setProperty("childType", childType);
    object.setProperty("length", length);
    object.setProperty(toStringPropertyString, eng->newFunction(arrayToString));

    //add validation and update function
    if (ctx->argumentCount() > 2)
        addValidationFunction(ctx, eng, object, 2);
    if (ctx->argumentCount() > 3)
        addUpdateFunction(ctx, eng, object, 3);

    return object;
}

QScriptValue ScriptEngineInitializer::scriptNewEnum(QScriptContext* ctx,
        QScriptEngine* eng)
{
    if (ctx->argumentCount() < 3)
        return ctx->throwError("enum initializer takes at least three arguments.");
    QScriptValue name = ctx->argument(0);
    if (!name.isString())
        return ctx->throwError(QScriptContext::TypeError,
                "enum(): enum name (argument 1) is not a string");

    QScriptValue enumType = ctx->argument(1);
    if (enumType.isString())
        /* do nothing */;
    else if (enumType.isObject())
        enumType = enumType.property(typePropertyString);
    else
        return ctx->throwError(QScriptContext::TypeError,
                "enum(): enum type (argument 2) is not a valid string or object");

    QString enumTypeString = enumType.toString();
    PrimitiveDataType pdt = PrimitiveFactory::typeStringToType(
            enumTypeString);
    if (pdt == Type_NotPrimitive)
        return ctx->throwError("enum(): enum type string'" + enumTypeString
                + "' is not a valid type");

    QScriptValue enumValues = ctx->argument(2);
    if (!enumValues.isObject())
        return ctx->throwError(QScriptContext::TypeError,
                "enum(): enum definition argument is not an object");

    //all arguments are valid
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    object.setProperty(typePropertyString, "enum");
    object.setProperty("enumType", enumType);
    object.setProperty("enumValues", enumValues);
    object.setProperty("enumName", name);
    object.setProperty(toStringPropertyString, eng->newFunction(enumToString));

    //add validation and update function
    if (ctx->argumentCount() > 3)
        addValidationFunction(ctx, eng, object, 3);
    if (ctx->argumentCount() > 4)
        addUpdateFunction(ctx, eng, object, 4);

    return object;
}

//toString functions


QScriptValue ScriptEngineInitializer::primitiveToString(QScriptContext* ctx,
        QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QString type =
            ctx->thisObject().property(typePropertyString).toString().toLower();
    if (ctx->argumentCount() == 1)
    {
        //name passed as parameter -> C/C++ string
        QString name = ctx->argument(0).toString();
        return type + ' ' + name + ';';
    }
    else
        return type;
}

QScriptValue ScriptEngineInitializer::bitfieldToString(QScriptContext* ctx,
        QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QString type = ctx->thisObject().property("bitfieldType").toString();
    QString width = ctx->thisObject().property("width").toString();
    if (ctx->argumentCount() == 1)
    {
        //name passed as parameter -> C/C++ string
        QString name = ctx->argument(0).toString();
        return QString::fromLatin1("%1 %2 : %3;").arg(type, name, width);
    }
    else
        return type + " : " + width + ';';
}

QScriptValue ScriptEngineInitializer::arrayToString(QScriptContext* ctx,
        QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QScriptValue thisObj = ctx->thisObject();
    QScriptValue childType = thisObj.property("childType");
    QString type = childType.toString();
    QString length = thisObj.property("length").toString();
    //arg1 is name, arg2 is indentation lvl (number of tabs needed, only needed when childType is struct/union)
    if (ctx->argumentCount() == 1 || ctx->argumentCount() == 2)
    {
        QString name = ctx->argument(0).toString();
        //name passed as parameter -> C/C++ string
        if (type == "struct" || type == "union")
        {
            /*show the struct and the union too:
             * i.e. struct {
             *          int i;
             *          char c;
             *       } val[13];
             */
            QScriptValue toString = childType.property(toStringPropertyString);
            if (!toString.isFunction())
                return QString::fromLatin1("%1 %2[%3];").arg(type, name, length);
            QScriptValueList args;
            args << "" << ctx->argument(1); //empty name + current indentation lvl
            QScriptValue structStr = toString.call(childType, args);
            QString structString = structStr.toString();
            structString = structString.remove(structString.length() - 1, 1); //remove last char, the ';'
            structString = structString.remove(type.length(), 1); //remove 1 unneeded space
            structString += ' ' + name + '[' + length + ']';
            return structString;
        }
        return QString::fromLatin1("%1 %2[%3];").arg(type, name, length);
    }
    else
        return QString::fromLatin1("%1[%2];").arg(type, length);
}

QScriptValue ScriptEngineInitializer::unionOrStructToCPPString(QScriptContext* ctx,
        QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QString type = ctx->thisObject().property(typePropertyString).toString(); // "union" or "struct"
    QScriptValue children = ctx->thisObject().property("children");
    QString name = ctx->argument(0).toString();
    int indentationLevel = 1;
    if (ctx->argumentCount() == 2)
    {
        //has second argument, which is indentation count
        QScriptValue idtLvl = ctx->argument(1);
        if (!idtLvl.isNumber())
            return ctx->throwError(QScriptContext::TypeError,
                    "second argument must be a number");
        indentationLevel = idtLvl.toUInt32();
    }
    QString indent = QString(indentationLevel, '\t');
    QString baseIndent = QString(qMax(0, indentationLevel - 2), '\t');

    QString completeString = baseIndent + type + ' ' + name + " {\n";
    QScriptValueIterator iter(children);
    while (iter.hasNext())
    {
        iter.next();
        QString name = iter.name();
        QScriptValue val = iter.value();
        if (!val.isObject() || children.propertyFlags(name)
                & QScriptValue::SkipInEnumeration)
            continue;

        QScriptValue toString = val.property(toStringPropertyString);
        if (!toString.isFunction())
            continue;

        QScriptValueList arg;
        arg << name;

        //value is a union/struct  -> increase indentation level
        QString valType = val.property(typePropertyString).toString();
        if (valType == "union" || valType == "struct" || valType == "array")
            arg << indentationLevel + 1;

        QString result = toString.call(val, arg).toString();
        completeString.append(indent + result + '\n');
    }

    completeString += QString(qMax(0, indentationLevel - 1), '\t') + "};";
    if (eng->hasUncaughtException())
        ScriptUtils::object()->logScriptError(eng->uncaughtExceptionBacktrace());
    return completeString;
}

QScriptValue ScriptEngineInitializer::unionToString(QScriptContext* ctx,
        QScriptEngine* eng)
{
    //arg1 is name, arg2 is indentation lvl (number of tabs needed)
    if (ctx->argumentCount() == 1 || ctx->argumentCount() == 2)
        return unionOrStructToCPPString(ctx, eng);
    else
        return "union";
}

QScriptValue ScriptEngineInitializer::structToString(QScriptContext* ctx,
        QScriptEngine* eng)
{
    //arg1 is name, arg2 is indentation lvl (number of tabs needed)
    if (ctx->argumentCount() == 1 || ctx->argumentCount() == 2)
        return unionOrStructToCPPString(ctx, eng);
    else
        return "struct";
}

QScriptValue ScriptEngineInitializer::enumToString(QScriptContext* ctx,
        QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QString enumType = ctx->thisObject().property("enumType").toString();
    QString enumName = ctx->thisObject().property("enumName").toString();
    if (ctx->argumentCount() > 0)
    {
        //as c++
        QString name = ctx->argument(0).toString();
        return enumName + " /* " + enumType + " */ " + name + ';';
    }
    //otherwise we also list all values
    QString result = "enum " + enumName + ":\n";
    QScriptValue children = ctx->thisObject().property("enumValues");
    QScriptValueIterator iter(children);
    while (iter.hasNext())
    {
        iter.next();
        result.append(iter.name() + " = " + iter.value().toString() + '\n');
    }
    return result;
}

void addFunctionAsMemberProperty(QScriptContext* ctx, QScriptEngine* eng,
        QScriptValue& val, int argIndex, QString propertyName)
{
    Q_UNUSED(eng)
    if (ctx->argumentCount() <= argIndex)
    {
        val = ctx->throwError("not enough arguments passed: needs at least"
                + QString::number(argIndex));
        return;
    }
    QScriptValue func = ctx->argument(argIndex);
    if (func.isNull())
    {
        //do not add it to the script value
        return;
    }
    if (!func.isFunction())
    {
        val = ctx->throwError(QScriptContext::TypeError, QString(
                "argument %1 is not a function").arg(argIndex));
        return;
    }
    // argument is valid -> set the property
    val.setProperty(propertyName, func);
}

void ScriptEngineInitializer::addValidationFunction(QScriptContext* ctx,
        QScriptEngine* eng, QScriptValue& val, int argIndex)
{
    addFunctionAsMemberProperty(ctx, eng, val, argIndex, "validationFunc");
}

void ScriptEngineInitializer::addUpdateFunction(QScriptContext* ctx,
        QScriptEngine* eng, QScriptValue& val, int argIndex)
{
    addFunctionAsMemberProperty(ctx, eng, val, argIndex, "updateFunc");
}

void ScriptEngineInitializer::addFuctionsToScriptEngine(QScriptEngine& engine)
{

    //TODO use the  prototype to set the toString function
    engine.globalObject().setProperty("uint8", engine.newFunction(scriptNewUInt8));
    engine.globalObject().setProperty("uint16", engine.newFunction(scriptNewUInt16));
    engine.globalObject().setProperty("uint32", engine.newFunction(scriptNewUInt32));
    engine.globalObject().setProperty("uint64", engine.newFunction(scriptNewUInt64));

    engine.globalObject().setProperty("int8", engine.newFunction(scriptNewInt8));
    engine.globalObject().setProperty("int16", engine.newFunction(scriptNewInt16));
    engine.globalObject().setProperty("int32", engine.newFunction(scriptNewInt32));
    engine.globalObject().setProperty("int64", engine.newFunction(scriptNewInt64));

    engine.globalObject().setProperty("bool8", engine.newFunction(scriptNewBool8));
    engine.globalObject().setProperty("bool16", engine.newFunction(scriptNewBool16));
    engine.globalObject().setProperty("bool32", engine.newFunction(scriptNewBool32));
    engine.globalObject().setProperty("bool64", engine.newFunction(scriptNewBool64));

    engine.globalObject().setProperty("float", engine.newFunction(scriptNewFloat));
    engine.globalObject().setProperty("double", engine.newFunction(scriptNewDouble));

    engine.globalObject().setProperty("char", engine.newFunction(scriptNewChar));

    engine.globalObject().setProperty("bitfield", engine.newFunction(
            scriptNewBitfield));

    engine.globalObject().setProperty("array", engine.newFunction(scriptNewArray));
    engine.globalObject().setProperty("struct", engine.newFunction(scriptNewStruct));
    engine.globalObject().setProperty("union", engine.newFunction(scriptNewUnion));
    //XXX: if I use enunm QtScript gives me syntax errors, I thought enum was not a keyword in JS
    engine.globalObject().setProperty("enumeration", engine.newFunction(scriptNewEnum));
}
