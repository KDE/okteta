/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012, 2013 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DEFAULTSCRIPTCLASS_HPP
#define KASTEN_DEFAULTSCRIPTCLASS_HPP

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
    using DataInfPtr = DataInformation*;
    using PropertyInfoList = QVector<QPair<QScriptString, QScriptValue::PropertyFlags>>;

    DefaultScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~DefaultScriptClass() override;

    QueryFlags queryProperty(const QScriptValue& object, const QScriptString& name, QueryFlags flags, uint* id) override;
    QScriptValue::PropertyFlags propertyFlags(const QScriptValue& object, const QScriptString& name, uint id) override;
    QScriptValue property(const QScriptValue& object, const QScriptString& name, uint id) override;
    void setProperty(QScriptValue& object, const QScriptString& name, uint id, const QScriptValue& value) override;
    QScriptValue prototype() const override;

    QScriptClassPropertyIterator* newIterator(const QScriptValue& object) override;

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
    QScriptString s_customTypeName;
    QScriptString s_asStringFunc;
    /** Contains all properties of this class, classes inheriting should add their own properties to this list */
    PropertyInfoList mIterableProperties;
    QScriptValue mDefaultPrototype;
    ScriptHandlerInfo* mHandlerInfo;
};

/** Provide a default iterator for all properties. This should suffice for all classes that don't have children */
class DefaultscriptClassIterator : public QScriptClassPropertyIterator
{
public:
    DefaultscriptClassIterator(const QScriptValue& object, DefaultScriptClass* cls);
    ~DefaultscriptClassIterator() override;
    bool hasNext() const override;
    bool hasPrevious() const override;
    QScriptString name() const override;
    QScriptValue::PropertyFlags flags() const override;
    uint id() const override;
    void next() override;
    void previous() override;
    void toBack() override;
    void toFront() override;

private:
    int mCurrent = -1;
    DefaultScriptClass* mClass;
    DataInformation* mData;
};

#endif // KASTEN_DEFAULTSCRIPTCLASS_HPP
