/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#include "scriptutils.hpp"

#include <QScriptValueIterator>
#include <QScriptValue>
#include <QScriptString>
#include <QScriptContext>
#include <QScriptEngine>

#include "../allprimitivetypes.hpp"

QString ScriptUtils::qScriptValueToString(const QScriptValue& val)
{
    if (!val.isValid()) {
        return QStringLiteral("invalid");
    }
    if (val.isUndefined()) {
        return QStringLiteral("undefined");
    }
    if (val.isNull()) {
        return QStringLiteral("null");
    }

    QString ret = val.toString();
    if (!val.isQObject()) {
        return ret;
    }
    ret += QLatin1String(" [");
    QScriptValueIterator it(val);
    bool first = true;
    while (it.hasNext()) {
        if (it.name().startsWith(QLatin1Char('_'))) {
            continue; // skip all names starting with _ like e.g. __proto__

        }
        if (!first) {
            ret += QLatin1String(", ");
        } else {
            first = false;
        }

        it.next();
        QScriptValue loopValue = it.value();
        if (!loopValue.isObject()) {
            ret += it.name() + QLatin1Char('=') + loopValue.toString();
        } else {
            ret += it.name() + QLatin1Char('=') + qScriptValueToString(loopValue);
        }
    }
    return ret;
}

void ScriptUtils::wrapAllPrimitiveTypes(QScriptValue& out,
                                        AllPrimitiveTypes allPrim, PrimitiveDataType actualType)
{
    out.setProperty(QStringLiteral("type"), PrimitiveType::standardTypeName(actualType));
    out.setProperty(QStringLiteral("char"), QString(allPrim.value<quint8>() > 127
                                                    ? QChar::ReplacementCharacter : QChar(allPrim.value<qint8>(), 0)));
    out.setProperty(QStringLiteral("int8"), allPrim.value<qint8>());
    out.setProperty(QStringLiteral("uint8"), allPrim.value<quint8>());
    out.setProperty(QStringLiteral("int16"), allPrim.value<qint16>());
    out.setProperty(QStringLiteral("uint16"), allPrim.value<quint16>());
    out.setProperty(QStringLiteral("int32"), allPrim.value<qint32>());
    out.setProperty(QStringLiteral("uint32"), allPrim.value<quint32>());
    out.setProperty(QStringLiteral("int64"), QString::number(allPrim.value<qint64>()));
    out.setProperty(QStringLiteral("uint64"), QString::number(allPrim.value<quint64>()));
    // QtScript has no support for 64 bit ints, add another value which contains the higher 32 bits
    // XXX any better solution for this?
    out.setProperty(QStringLiteral("int64high32bits"), qint32(allPrim.value<qint64>() >> 32));
    out.setProperty(QStringLiteral("uint64high32bits"), quint32(allPrim.value<quint64>() >> 32));

    out.setProperty(QStringLiteral("float"), allPrim.value<float>());
    out.setProperty(QStringLiteral("double"), allPrim.value<double>());
}
