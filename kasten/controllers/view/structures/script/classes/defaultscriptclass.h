/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef DEFAULTSCRIPTCLASS_H
#define DEFAULTSCRIPTCLASS_H

#include <QtScript/QScriptClass>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptEngine>
#include <QtScript/QScriptString>
#include <KDebug>

class DataInformation;
class ScriptHandlerInfo;

class DefaultScriptClass : public QScriptClass
{
    Q_DISABLE_COPY(DefaultScriptClass)
public:
    typedef DataInformation* DataInfPtr;
    DefaultScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    virtual ~DefaultScriptClass();
    //TODO
    //virtual QScriptClassPropertyIterator* newIterator(const QScriptValue& object);
    virtual QueryFlags queryProperty(const QScriptValue& object, const QScriptString& name, QueryFlags flags, uint* id);
    virtual QScriptValue::PropertyFlags propertyFlags(const QScriptValue& object, const QScriptString& name, uint id);
    virtual QScriptValue property(const QScriptValue& object, const QScriptString& name, uint id);
    virtual void setProperty(QScriptValue& object, const QScriptString& name, uint id, const QScriptValue& value);
    virtual QScriptValue prototype() const;
    static QScriptValue toScriptValue(QScriptEngine* eng, const DataInfPtr& data);
    static void fromScriptValue(const QScriptValue& obj, DataInfPtr& data);
protected:
    virtual bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QueryFlags* flags, uint* id) = 0;
    virtual bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint id, QScriptValue::PropertyFlags* flags)  = 0;
    virtual QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id)  = 0;
    virtual bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) = 0;
    static QScriptValue Default_proto_toString(QScriptContext* ctx, QScriptEngine* eng);

protected:
    QScriptString valid;
    QScriptString wasAbleToRead;
    QScriptString validationError;
    QScriptString parent;
    QScriptString byteOrder;
    QScriptValue mDefaultPrototype;
    ScriptHandlerInfo* mHandlerInfo;
};

#endif // DEFAULTSCRIPTCLASS_H
