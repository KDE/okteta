/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
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

#ifndef SCRIPTUTILS_H_
#define SCRIPTUTILS_H_

#include <QtCore/QObject>
#include <QtCore/QAtomicPointer>

#include <QtScript/QScriptValue>
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include "../allprimitivetypes.h"

#include <KGlobal>

class ScriptUtils: public QObject
{
Q_OBJECT
public:
    ScriptUtils(QObject* parent = NULL) :
        QObject(parent)
    {
    }
    ~ScriptUtils()
    {
    }
    static ScriptUtils* object();

    static void dumpQScriptValue(const QScriptValue& val, const char* file, int line);
    static QString qScriptValueToString(const QScriptValue& val);

    static void wrapAllPrimitiveTypes(QScriptValue& out, AllPrimitiveTypes allPrim,
            PrimitiveDataType actualType);

    static QScriptValue allPrimitivesToString(QScriptContext* ctx, QScriptEngine* eng);

    void logScriptError(const QString& message, QScriptValue errorObject);
    void logScriptError(const QString& message)
    {
        logScriptError(message, QScriptValue());
    }
    void logScriptError(QStringList errorMessages)
    {
        foreach(const QString& msg, errorMessages)
            {
                logScriptError(msg);
            }
    }
signals:
    void scriptError(QString msg, QString errToString = QString());
public:
};

#endif /* SCRIPTUTILS_H_ */
