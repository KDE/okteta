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
#include "scriptutils.h"

#include <QtScript/QScriptValueIterator>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptString>
#include <KDebug>

void ScriptUtils::dumpQScriptValue(QScriptValue& val, const char* file, int line)
{
    kDebug()
        << "dumping called from: " << file << ":" << line;
    if (!val.isValid())
    {
        kDebug()
            << "val is invalid";
        return;
    }

    if (val.isUndefined())
    {
        kDebug()
            << "val is undefined";
        return;
    }
    kDebug()
        << "val=" << val.toString();

    QScriptValueIterator it(val);
    while (it.hasNext())
    {
        it.next();
        kDebug()
            << "name=" << it.name() << "scriptName=" << it.scriptName() << "val="
                    << it.value().toString();
    }
}

void ScriptUtils::wrapAllPrimitiveTypes(QScriptValue& val,
        AllPrimitiveTypes allPrim, PrimitiveDataType actualType)
{
    QString type;
    switch (actualType)
    {
    case Type_Bool64:
    case Type_UInt64:
    case Type_Bitfield:
        type = "uint64";
        break;
    case Type_Bool32:
    case Type_UInt32:
        type = "uint32";
        break;
    case Type_Bool16:
    case Type_UInt16:
        type = "uint16";
        break;
    case Type_Bool8:
    case Type_UInt8:
        type = "uint8";
        break;
    case Type_Char:
        type = "char";
        break;
    case Type_Float:
        type = "float";
        break;
    case Type_Double:
        type = "double";
        break;
    case Type_Int8:
        type = "int8";
        break;
    case Type_Int16:
        type = "int16";
        break;
    case Type_Int32:
        type = "int32";
        break;
    case Type_Int64:
        type = "int64";
        break;
    default:
        break;
    }
    val.setProperty("__type__", type);
    val.setProperty("char", QString(QLatin1Char(allPrim.ubyteValue)));
    val.setProperty("int8", allPrim.byteValue);
    val.setProperty("uint8", allPrim.ubyteValue);
    val.setProperty("int16", allPrim.shortValue);
    val.setProperty("uint16", allPrim.ushortValue);
    val.setProperty("int32", allPrim.intValue);
    val.setProperty("uint32", allPrim.uintValue);
    //XXX QtScript has no support for 64 bit ints, cast to double or to 32 bit??
    val.setProperty("int64", (qsreal) allPrim.longValue);
    val.setProperty("uint64", (qsreal) allPrim.ulongValue);

    val.setProperty("float", allPrim.floatValue);
    val.setProperty("double", allPrim.doubleValue);
    QScriptValue toStringFunc = val.engine()->newFunction(allPrimitivesToString);
    val.setProperty("toString", toStringFunc);
}

QScriptValue ScriptUtils::allPrimitivesToString(QScriptContext* ctx,
        QScriptEngine* eng)
{
    Q_UNUSED(eng)
    QString type = ctx->thisObject().property("__type__").toString();
    return ctx->thisObject().property(type).toString();
}

void ScriptUtils::logScriptError(const QString& message, QScriptValue errorObject)
{
    if (!errorObject.isError())
    {
        if (!message.isEmpty())
            emit scriptError(message);
    }
    else
        emit scriptError(message, errorObject.toString());
}
K_GLOBAL_STATIC(ScriptUtils, scrObj)

ScriptUtils* ScriptUtils::object() {
    return scrObj;
}
