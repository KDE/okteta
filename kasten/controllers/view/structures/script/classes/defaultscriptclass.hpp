/*
    SPDX-FileCopyrightText: 2011, 2012, 2013 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DEFAULTSCRIPTCLASS_HPP
#define KASTEN_DEFAULTSCRIPTCLASS_HPP

// Qt
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

public:
    using DataInfPtr = DataInformation*;
    using PropertyInfoList = QVector<QPair<QScriptString, QScriptValue::PropertyFlags>>;

public:
    DefaultScriptClass(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    ~DefaultScriptClass() override;

public: // QScriptClass API
    [[nodiscard]]
    QueryFlags queryProperty(const QScriptValue& object, const QScriptString& name, QueryFlags flags, uint* id) override;
    [[nodiscard]]
    QScriptValue::PropertyFlags propertyFlags(const QScriptValue& object, const QScriptString& name, uint id) override;
    [[nodiscard]]
    QScriptValue property(const QScriptValue& object, const QScriptString& name, uint id) override;
    void setProperty(QScriptValue& object, const QScriptString& name, uint id, const QScriptValue& value) override;
    [[nodiscard]]
    QScriptValue prototype() const override;

    [[nodiscard]]
    QScriptClassPropertyIterator* newIterator(const QScriptValue& object) override;

public:
    /** Convert a QScriptValue to DataInformation than qscriptvalue_cast, since we know exactly what to expect
     * @param val the value to convert. Do not call .data() on it.
     * @return @p val converted to a Datainformation*
     */
    [[nodiscard]]
    static DataInformation* toDataInformation(const QScriptValue& obj);

protected: // API to implement
    [[nodiscard]]
    virtual bool queryAdditionalProperty(const DataInformation* data, const QScriptString& name, QueryFlags* flags, uint* id) = 0;
    [[nodiscard]]
    virtual bool additionalPropertyFlags(const DataInformation* data, const QScriptString& name, uint id, QScriptValue::PropertyFlags* flags)  = 0;
    [[nodiscard]]
    virtual QScriptValue additionalProperty(const DataInformation* data, const QScriptString& name, uint id)  = 0;
    [[nodiscard]]
    virtual bool setAdditionalProperty(DataInformation* data, const QScriptString& name, uint id, const QScriptValue& value) = 0;

protected:
    [[nodiscard]]
    ScriptHandlerInfo* handlerInfo() const;

private:
    [[nodiscard]]
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

private:
    QScriptValue mDefaultPrototype;
    ScriptHandlerInfo* const mHandlerInfo;
};

inline
ScriptHandlerInfo* DefaultScriptClass::handlerInfo() const
{
    return mHandlerInfo;
}

/** Provide a default iterator for all properties. This should suffice for all classes that don't have children */
class DefaultscriptClassIterator : public QScriptClassPropertyIterator
{
public:
    DefaultscriptClassIterator(const QScriptValue& object, DefaultScriptClass* cls);
    ~DefaultscriptClassIterator() override;

public: // QScriptClassPropertyIterator API
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
    DefaultScriptClass* const mClass;
    DataInformation* mData;
};

#endif // KASTEN_DEFAULTSCRIPTCLASS_HPP
