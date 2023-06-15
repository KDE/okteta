/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/
#include "scriptutils.hpp"

// Qt
#include <QScriptValueIterator>
#include <QScriptValue>

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
