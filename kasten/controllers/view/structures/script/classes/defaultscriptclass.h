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

#include <QVector>
#include <QScriptClass>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptString>
#include <QScriptClassPropertyIterator>

class DataInformation;
class ScriptHandlerInfo;

class DefaultScriptClass : public QScriptClass
{
    friend class DefaultscriptClassIterator;
    Q_DISABLE_COPY(DefaultScriptClass)
public:
    typedef DataInformation* DataInfPtr;
    typedef QVector<QPair<QScriptString, QScriptValue::PropertyFlags> > PropertyInfoList;
    DefaultScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    virtual ~DefaultScriptClass();

    virtual QueryFlags queryProperty(const QScriptValue& object, const QScriptString& name, QueryFlags flags, uint* id);
    virtual QScriptValue::PropertyFlags propertyFlags(const QScriptValue& object, const QScriptString& name, uint id);
    virtual QScriptValue property(const QScriptValue& object, const QScriptString& name, uint id);
    virtual void setProperty(QScriptValue& object, const QScriptString& name, uint id, const QScriptValue& value);
    virtual QScriptValue prototype() const;

    virtual QScriptClassPropertyIterator* newIterator(const QScriptValue& object);

    /** Convert a QScriptValue to DataInformation than qscriptvalue_cast, since we know exactly what to expect
     * @param val the value to convert. Do not call .data() on it.
     * @return @p val converted to a Datainformation*
     */
    static DataInformation* toDataInformation(const QScriptValue& val);

protected:
    virtual bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QueryFlags* flags, uint* id) = 0;
    virtual bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint id, QScriptValue::PropertyFlags* flags)  = 0;
    virtual QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id)  = 0;
    virtual bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) = 0;
    static QScriptValue Default_proto_toString(QScriptContext* ctx, QScriptEngine* eng);

private:
    void setDataType(const QScriptValue& value, DataInformation* data);
protected:
    QScriptString s_valid;
    QScriptString s_wasAbleToRead;
    QScriptString s_validationError;
    QScriptString s_parent;
    QScriptString s_byteOrder;
    QScriptString s_name;
    QScriptString s_datatype;
    QScriptString s_updateFunc;
    QScriptString s_validationFunc;
    /** Contains all properties of this class, classes inheriting should add their own properties to this list */
    PropertyInfoList mIterableProperties;
    QScriptValue mDefaultPrototype;
    ScriptHandlerInfo* mHandlerInfo;
};

/** Provide a default iterator for all properties. This should suffice for all classes that don't have children */
class DefaultscriptClassIterator : public QScriptClassPropertyIterator {
public:
    DefaultscriptClassIterator(const QScriptValue& object, DefaultScriptClass* cls);
    virtual ~DefaultscriptClassIterator();
    virtual bool hasNext() const;
    virtual bool hasPrevious() const;
    virtual QScriptString name() const;
    virtual QScriptValue::PropertyFlags flags() const;
    virtual uint id() const;
    virtual void next();
    virtual void previous();
    virtual void toBack();
    virtual void toFront();
private:
    int mCurrent;
    DefaultScriptClass* mClass;
    DataInformation* mData;
};

#endif // DEFAULTSCRIPTCLASS_H
