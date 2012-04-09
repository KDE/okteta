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
#include <QtCore/QStringList>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptValueIterator>

#include <KDebug>

namespace ScriptEngineInitializer
{

void addFuctionsToScriptEngine(QScriptEngine& engine)
{
    engine.globalObject().setProperty(QLatin1String("uint8"),
            engine.newFunction(Private::scriptNewUInt8));
    engine.globalObject().setProperty(QLatin1String("uint16"),
            engine.newFunction(Private::scriptNewUInt16));
    engine.globalObject().setProperty(QLatin1String("uint32"),
            engine.newFunction(Private::scriptNewUInt32));
    engine.globalObject().setProperty(QLatin1String("uint64"),
            engine.newFunction(Private::scriptNewUInt64));

    engine.globalObject().setProperty(QLatin1String("int8"),
            engine.newFunction(Private::scriptNewInt8));
    engine.globalObject().setProperty(QLatin1String("int16"),
            engine.newFunction(Private::scriptNewInt16));
    engine.globalObject().setProperty(QLatin1String("int32"),
            engine.newFunction(Private::scriptNewInt32));
    engine.globalObject().setProperty(QLatin1String("int64"),
            engine.newFunction(Private::scriptNewInt64));

    engine.globalObject().setProperty(QLatin1String("bool8"),
            engine.newFunction(Private::scriptNewBool8));
    engine.globalObject().setProperty(QLatin1String("bool16"),
            engine.newFunction(Private::scriptNewBool16));
    engine.globalObject().setProperty(QLatin1String("bool32"),
            engine.newFunction(Private::scriptNewBool32));
    engine.globalObject().setProperty(QLatin1String("bool64"),
            engine.newFunction(Private::scriptNewBool64));

    engine.globalObject().setProperty(QLatin1String("float"),
            engine.newFunction(Private::scriptNewFloat));
    engine.globalObject().setProperty(QLatin1String("double"),
            engine.newFunction(Private::scriptNewDouble));

    engine.globalObject().setProperty(QLatin1String("char"),
            engine.newFunction(Private::scriptNewChar));

    engine.globalObject().setProperty(QLatin1String("bitfield"),
            engine.newFunction(Private::scriptNewBitfield));

    engine.globalObject().setProperty(QLatin1String("array"),
            engine.newFunction(Private::scriptNewArray));
    engine.globalObject().setProperty(QLatin1String("struct"),
            engine.newFunction(Private::scriptNewStruct));
    engine.globalObject().setProperty(QLatin1String("union"),
            engine.newFunction(Private::scriptNewUnion));

    //if I use enum QtScript gives me syntax errors, I thought enum was not a keyword in JS
    engine.globalObject().setProperty(QLatin1String("enumeration"),
            engine.newFunction(Private::scriptNewEnum));
    engine.globalObject().setProperty(QLatin1String("flags"),
            engine.newFunction(Private::scriptNewFlags));
    engine.globalObject().setProperty(QLatin1String("string"),
            engine.newFunction(Private::scriptNewString));
}

namespace Private
{

static const QString typePropertyString = QLatin1String("type");
static const QString toStringPropertyString = QLatin1String("toString");
static const QString setUpdatePropertyString = QLatin1String("setUpdate");
static const QString setValidationPropertyString = QLatin1String("setValidation");

/** create a new primitive of type @p type */
QScriptValue primitiveConstructor(QScriptContext* ctx, QScriptEngine* eng, const QLatin1String type)
{
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();
    object.setProperty(typePropertyString, type);
    object.setProperty(toStringPropertyString, eng->newFunction(primitiveToString));
    object.setProperty(setUpdatePropertyString, eng->newFunction(addUpdateFunc, 1));
    object.setProperty(setValidationPropertyString, eng->newFunction(addValidationFunc, 1));

    return object;
}

QScriptValue scriptNewUInt8(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("UInt8"));
}

QScriptValue scriptNewUInt16(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("UInt16"));
}

QScriptValue scriptNewUInt32(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("UInt32"));
}

QScriptValue scriptNewUInt64(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("UInt64"));
}

QScriptValue scriptNewInt8(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Int8"));
}

QScriptValue scriptNewInt16(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Int16"));
}

QScriptValue scriptNewInt32(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Int32"));
}

QScriptValue scriptNewInt64(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Int64"));
}

QScriptValue scriptNewBool8(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Bool8"));
}

QScriptValue scriptNewBool16(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Bool16"));
}

QScriptValue scriptNewBool32(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Bool32"));
}

QScriptValue scriptNewBool64(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Bool64"));
}

QScriptValue scriptNewFloat(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Float"));
}

QScriptValue scriptNewDouble(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Double"));
}

QScriptValue scriptNewChar(QScriptContext* ctx, QScriptEngine* eng)
{
    return primitiveConstructor(ctx, eng, QLatin1String("Char"));
}

QScriptValue scriptNewBitfield(QScriptContext* ctx, QScriptEngine* eng)
{
    //arg1 is type
    //arg2 is width
    //check arguments first
    if (ctx->argumentCount() < 2)
        return ctx->throwError(QLatin1String("bitfield initializer takes at least two arguments."));
    if (!ctx->argument(1).isNumber())
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String("bitfield(): second argument is not a number"));
    QString typeArg = ctx->argument(0).toString();
    if (typeArg != QLatin1String("bool") && typeArg != QLatin1String("unsigned")
            && typeArg != QLatin1String("signed"))
    {
        return ctx->throwError(QLatin1String("type must be 'bool', 'unsigned' 'signed'"
                " or '', actual value was '") + typeArg + QLatin1Char('\''));
    }
    //all arguments are valid
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    object.setProperty(typePropertyString, QLatin1String("bitfield"));
    //has been validated, so it must contain valid values
    object.setProperty(QLatin1String("bitfieldType"),
            typeArg.isEmpty() ? QLatin1String("signed") : typeArg);
    object.setProperty(QLatin1String("width"), ctx->argument(1));
    object.setProperty(toStringPropertyString, eng->newFunction(bitfieldToString));
    object.setProperty(setUpdatePropertyString, eng->newFunction(addUpdateFunc, 1));
    object.setProperty(setValidationPropertyString, eng->newFunction(addValidationFunc, 1));

    return object;
}

//with children:
QScriptValue scriptNewStruct(QScriptContext* ctx, QScriptEngine* eng)
{
    //takes one argument, the children
    if (ctx->argumentCount() < 1)
        return ctx->throwError(QLatin1String("struct initializer takes at least one argument."));
    QScriptValue children = ctx->argument(0);
    if (!children.isObject())
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String("struct(): argument is not an object"));

    //all arguments are valid
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    object.setProperty(typePropertyString, QLatin1String("struct"));
    object.setProperty(QLatin1String("children"), children);
    object.setProperty(toStringPropertyString, eng->newFunction(structToString));
    object.setProperty(QLatin1String("child"), eng->newFunction(getChild));
    object.setProperty(setUpdatePropertyString, eng->newFunction(addUpdateFunc, 1));
    object.setProperty(setValidationPropertyString, eng->newFunction(addValidationFunc, 1));

    return object;
}

QScriptValue scriptNewUnion(QScriptContext* ctx, QScriptEngine* eng)
{
    //takes one argument, the children
    if (ctx->argumentCount() < 1)
        return ctx->throwError(QLatin1String("union initializer takes at least one argument."));
    QScriptValue children = ctx->argument(0);
    if (!children.isObject())
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String("union(): argument is not an object"));

    //all arguments are valid
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    object.setProperty(typePropertyString, QLatin1String("union"));
    object.setProperty(QLatin1String("children"), children);
    object.setProperty(toStringPropertyString, eng->newFunction(unionToString));
    object.setProperty(QLatin1String("child"), eng->newFunction(getChild));
    object.setProperty(setUpdatePropertyString, eng->newFunction(addUpdateFunc, 1));
    object.setProperty(setValidationPropertyString, eng->newFunction(addValidationFunc, 1));

    return object;
}

QScriptValue scriptNewArray(QScriptContext* ctx, QScriptEngine* eng)
{
    //arg 1 is child type, arg2 is length
    if (ctx->argumentCount() < 2)
        return ctx->throwError(QLatin1String("array initializer takes at least two arguments."));
    QScriptValue childType = ctx->argument(0);
    if (!childType.isObject())
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String("array(): childType argument is not an object"));
    QScriptValue length = ctx->argument(1);
    if (!length.isNumber())
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String("array(): length Type argument is not a number"));

    //all arguments are valid
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    object.setProperty(typePropertyString, QLatin1String("array"));
    object.setProperty(QLatin1String("childType"), childType);
    object.setProperty(QLatin1String("length"), length);
    object.setProperty(toStringPropertyString, eng->newFunction(arrayToString));
    object.setProperty(setUpdatePropertyString, eng->newFunction(addUpdateFunc, 1));
    object.setProperty(setValidationPropertyString, eng->newFunction(addValidationFunc, 1));

    return object;
}

QScriptValue createEnumObject(QScriptContext* ctx, QScriptEngine* eng, bool flags)
{
    if (ctx->argumentCount() < 3)
        return ctx->throwError(
                QLatin1String("enumeration initializer takes at least three arguments."));
    QScriptValue name = ctx->argument(0);
    if (!name.isString())
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String("enumeration(): enum name (argument 1) is not a string"));

    QScriptValue enumType = ctx->argument(1);
    if (!enumType.isString() && enumType.isObject())
        enumType = enumType.property(typePropertyString);
    else
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String(
                        "enumeration(): enum type (argument 2) is not a valid string or object"));

    QString enumTypeString = enumType.toString();
    PrimitiveDataType pdt = PrimitiveFactory::typeStringToType(enumTypeString);
    if (pdt == Type_NotPrimitive || pdt == Type_Float || pdt == Type_Double)
        return ctx->throwError(
                QLatin1String("enumeration(): enum type (argument 2) '") + enumTypeString
                        + QLatin1String("' is not a valid type"));

    QScriptValue enumValues = ctx->argument(2);
    if (!enumValues.isObject())
        return ctx->throwError(QScriptContext::TypeError,
                QLatin1String("enumeration(): enum definition argument is not an object"));

    //all arguments are valid
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    if (flags)
        object.setProperty(typePropertyString, QLatin1String("flags"));
    else
        object.setProperty(typePropertyString, QLatin1String("enum"));

    object.setProperty(QLatin1String("enumType"), enumType);
    object.setProperty(QLatin1String("enumValues"), enumValues);
    object.setProperty(QLatin1String("enumName"), name);
    object.setProperty(toStringPropertyString, eng->newFunction(enumToString));
    object.setProperty(setUpdatePropertyString, eng->newFunction(addUpdateFunc, 1));
    object.setProperty(setValidationPropertyString, eng->newFunction(addValidationFunc, 1));

    return object;
}

QScriptValue scriptNewEnum(QScriptContext* ctx, QScriptEngine* eng)
{
    return createEnumObject(ctx, eng, false);
}

QScriptValue scriptNewFlags(QScriptContext* ctx, QScriptEngine* eng)
{
    return createEnumObject(ctx, eng, true);
}

QScriptValue scriptNewString(QScriptContext* ctx, QScriptEngine* eng)
{
    QString encoding;
    if (ctx->argumentCount() < 1)
        encoding = QLatin1String("ascii");
    else
        encoding = ctx->argument(0).toString();
    QScriptValue object;
    if (ctx->isCalledAsConstructor())
        object = ctx->thisObject();
    else
        object = eng->newObject();

    object.setProperty(typePropertyString, QLatin1String("string"));
    object.setProperty(QLatin1String("encoding"), encoding);
    object.setProperty(setUpdatePropertyString, eng->newFunction(addUpdateFunc, 1));
    object.setProperty(setValidationPropertyString, eng->newFunction(addValidationFunc, 1));

    return object;
}

//toString functions

QScriptValue primitiveToString(QScriptContext* ctx, QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QString type =
            ctx->thisObject().property(typePropertyString).toString().toLower();
    if (ctx->argumentCount() == 1)
    {
        //name passed as parameter -> C/C++ string
        QString name = ctx->argument(0).toString();
        return QString(type + QLatin1Char(' ') + name + QLatin1Char(';'));
    }
    else
        return type;
}

QScriptValue bitfieldToString(QScriptContext* ctx, QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QString type = ctx->thisObject().property(QLatin1String("bitfieldType")).toString();
    QString width = ctx->thisObject().property(QLatin1String("width")).toString();
    if (ctx->argumentCount() == 1)
    {
        //name passed as parameter -> C/C++ string
        QString name = ctx->argument(0).toString();
        return QString::fromLatin1("%1 %2 : %3;").arg(type, name, width);
    }
    else
        return QString(type + QLatin1String(" : ") + width + QLatin1Char(';'));
}

QScriptValue arrayToString(QScriptContext* ctx, QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QScriptValue thisObj = ctx->thisObject();
    QScriptValue childType = thisObj.property(QLatin1String("childType"));
    QString type = childType.toString();
    QString length = thisObj.property(QLatin1String("length")).toString();
    //arg1 is name, arg2 is indentation lvl (number of tabs needed, only needed when childType is struct/union)
    if (ctx->argumentCount() == 1 || ctx->argumentCount() == 2)
    {
        QString name = ctx->argument(0).toString();
        //name passed as parameter -> C/C++ string
        if (type == QLatin1String("struct") || type == QLatin1String("union"))
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
            args << QString() << ctx->argument(1); //empty name + current indentation lvl
            QScriptValue structStr = toString.call(childType, args);
            QString structString = structStr.toString();
            structString = structString.remove(structString.length() - 1, 1); //remove last char, the ';'
            structString = structString.remove(type.length(), 1); //remove 1 unneeded space
            structString += QLatin1Char(' ') + name + QLatin1Char('[') + length + QLatin1Char(']');
            return structString;
        }
        return QString::fromLatin1("%1 %2[%3];").arg(type, name, length);
    }
    else
        return QString::fromLatin1("%1[%2];").arg(type, length);
}

QScriptValue unionOrStructToCPPString(QScriptContext* ctx, QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QString type = ctx->thisObject().property(typePropertyString).toString(); // "union" or "struct"
    QScriptValue children = ctx->thisObject().property(QLatin1String("children"));
    QString name = ctx->argument(0).toString();
    int indentationLevel = 1;
    if (ctx->argumentCount() == 2)
    {
        //has second argument, which is indentation count
        QScriptValue idtLvl = ctx->argument(1);
        if (!idtLvl.isNumber())
            return ctx->throwError(QScriptContext::TypeError,
                    QLatin1String("second argument must be a number"));
        indentationLevel = idtLvl.toUInt32();
    }
    QString indent = QString(indentationLevel, QLatin1Char('\t'));
    QString baseIndent = QString(qMax(0, indentationLevel - 2), QLatin1Char('\t'));

    QString completeString = baseIndent + type + QLatin1Char(' ') + name + QLatin1String(" {\n");
    QScriptValueIterator iter(children);
    while (iter.hasNext())
    {
        iter.next();
        QString name = iter.name();
        QScriptValue val = iter.value();
        if (!val.isObject())
            continue;

        QScriptValue toString = val.property(toStringPropertyString);
        if (!toString.isFunction())
            continue;

        QScriptValueList arg;
        arg << name;

        //value is a union/struct  -> increase indentation level
        QString valType = val.property(typePropertyString).toString();
        if (valType == QLatin1String("union")
                || valType == QLatin1String("struct")
                || valType == QLatin1String("array"))
            arg << indentationLevel + 1;

        QString result = toString.call(val, arg).toString();
        completeString.append(indent + result + QLatin1Char('\n'));
    }

    completeString += QString(qMax(0, indentationLevel - 1), QLatin1Char('\t'))
            + QLatin1String("};");
    return completeString;
}

QScriptValue unionToString(QScriptContext* ctx, QScriptEngine* eng)
{
    //arg1 is name, arg2 is indentation lvl (number of tabs needed)
    if (ctx->argumentCount() == 1 || ctx->argumentCount() == 2)
        return unionOrStructToCPPString(ctx, eng);
    else
        return QLatin1String("union");
}

QScriptValue structToString(QScriptContext* ctx, QScriptEngine* eng)
{
    //arg1 is name, arg2 is indentation lvl (number of tabs needed)
    if (ctx->argumentCount() == 1 || ctx->argumentCount() == 2)
        return unionOrStructToCPPString(ctx, eng);
    else
        return QLatin1String("struct");
}

QScriptValue enumToString(QScriptContext* ctx, QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QString enumType = ctx->thisObject().property(QLatin1String("enumType")).toString();
    QString enumName = ctx->thisObject().property(QLatin1String("enumName")).toString();
    if (ctx->argumentCount() > 0)
    {
        //as c++
        QString name = ctx->argument(0).toString();
        return QString(enumName + QLatin1String(" /* ") + enumType + QLatin1String(" */ ")
                + name + QLatin1Char(';'));
    }
    //otherwise we also list all values
    QString result = QLatin1String("enum ") + enumName + QLatin1String(":\n");
    QScriptValue children = ctx->thisObject().property(QLatin1String("enumValues"));
    QScriptValueIterator iter(children);
    while (iter.hasNext())
    {
        iter.next();
        result.append(iter.name() + QLatin1String(" = ")
                + iter.value().toString() + QLatin1Char('\n'));
    }
    return result;
}

QScriptValue getChild(QScriptContext* ctx, QScriptEngine* eng)
{
    if (ctx->argumentCount() < 1)
        return ctx->throwError(
                QLatin1String("child(): name of child must be passed as first parameter"));
    QScriptValue name = ctx->argument(0);
    QScriptValue ret = ctx->thisObject().property(QLatin1String("children")).property(
            name.toString());
    if (ret.isValid())
        return ret;
    else
        return eng->undefinedValue();
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
