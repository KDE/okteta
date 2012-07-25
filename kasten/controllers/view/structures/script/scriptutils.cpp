/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#include <QScriptValueIterator>
#include <QScriptValue>
#include <QScriptString>
#include <QScriptContext>
#include <QScriptEngine>

#include <KDebug>

#include "../allprimitivetypes.h"

QString ScriptUtils::qScriptValueToString(const QScriptValue& val)
{
    if (!val.isValid())
        return QLatin1String("invalid");
    if (val.isUndefined())
        return QLatin1String("undefined");
    if (val.isNull())
        return QLatin1String("null");


    QString ret = val.toString();
    if (!val.isQObject()) {
        return ret;
    }
    ret += QLatin1String(" [");
    QScriptValueIterator it(val);
    bool first = true;
    while (it.hasNext())
    {
        if (it.name().startsWith(QLatin1String("_")))
            continue; // skip all names starting with _ like e.g. __proto__

        if (!first)
            ret += QLatin1String(", ");
        else
            first = false;

        it.next();
        QScriptValue loopValue = it.value();
        if (!loopValue.isObject()) {
            ret += it.name() + QLatin1String("=") + loopValue.toString();
        }
        else {
            ret += it.name() + QLatin1String("=") + qScriptValueToString(loopValue);
        }
    }
    return ret;
}

void ScriptUtils::wrapAllPrimitiveTypes(QScriptValue& out,
        AllPrimitiveTypes allPrim, PrimitiveDataType actualType)
{
    out.setProperty(QLatin1String("type"), PrimitiveType::standardTypeName(actualType));
    out.setProperty(QLatin1String("char"), QString(QLatin1Char(allPrim.ubyteValue)));
    out.setProperty(QLatin1String("int8"), allPrim.byteValue);
    out.setProperty(QLatin1String("uint8"), allPrim.ubyteValue);
    out.setProperty(QLatin1String("int16"), allPrim.shortValue);
    out.setProperty(QLatin1String("uint16"), allPrim.ushortValue);
    out.setProperty(QLatin1String("int32"), allPrim.intValue);
    out.setProperty(QLatin1String("uint32"), allPrim.uintValue);
    //QtScript has no support for 64 bit ints, add another value which contains the higher 32 bits
    //XXX any better solution for this?
    out.setProperty(QLatin1String("int64high32bits"), qint32(allPrim.ulongValue >> 32));
    out.setProperty(QLatin1String("uint64high32bits"), quint32(allPrim.ulongValue >> 32));

    out.setProperty(QLatin1String("float"), allPrim.floatValue);
    out.setProperty(QLatin1String("double"), allPrim.doubleValue);
}
