/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTARRAYDATA_HPP
#define KASTEN_ABSTRACTARRAYDATA_HPP

// lib
#include <datainformationbase.hpp>
#include <primitivedatatype.hpp>
// Okteta core
#include <Okteta/Address>
// Qt
#include <Qt>
// Std
#include <memory>

class QWidget;
class QScriptEngine;
class QScriptValue;
class DataInformation;
class ScriptHandlerInfo;

namespace Okteta {
class AbstractByteArrayModel;
}

class QVariant;

class AbstractArrayData
{
public:
    explicit AbstractArrayData(std::unique_ptr<DataInformation>&& childType, ArrayDataInformation* parent);
    AbstractArrayData(const AbstractArrayData&) = delete;

    virtual ~AbstractArrayData();

    AbstractArrayData& operator=(const AbstractArrayData&) = delete;

public:
    void setParent(ArrayDataInformation* parent);
    /** @return The current child type. Ownership is NOT transferred */
    DataInformation* childType() const;

    virtual QVariant dataAt(uint index, int column, int role) = 0;
    virtual unsigned int length() const = 0;
    virtual void setLength(uint newLength) = 0;

    virtual QString typeName() const = 0;
    virtual QString valueString() const = 0;

    virtual BitCount32 size() const = 0;

    virtual DataInformation* childAt(unsigned int idx) = 0;

    virtual QScriptValue toScriptValue(uint index, QScriptEngine* engine,
                                       ScriptHandlerInfo* handlerInfo) = 0;
    /** the primitive type or PrimitiveDataType::Invalid for structs etc */
    virtual PrimitiveDataType primitiveType() const = 0;

    virtual int indexOf(const DataInformation* data) const = 0;
    virtual BitCount64 offset(const DataInformation* child) const = 0;
    virtual qint64 readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) = 0;
    virtual bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                              Okteta::Address address, BitCount64 bitsRemaining) = 0;
    virtual BitCount32 sizeAt(uint index) = 0;
    virtual Qt::ItemFlags childFlags(int row, int column, bool fileLoaded) = 0;
    virtual bool isComplex() const = 0;

    virtual QWidget* createChildEditWidget(uint index, QWidget* parent) const = 0;
    virtual QVariant dataFromChildWidget(uint index, const QWidget* w) const = 0;
    virtual void setChildWidgetData(uint index, QWidget* w) const = 0;

    /** Takes ownership over @p type ! */
    static AbstractArrayData* newArrayData(uint length,
                                           std::unique_ptr<DataInformation>&& type,
                                           ArrayDataInformation* parent);

protected:
    virtual void setNewParentForChildren() = 0;

protected:
    ArrayDataInformation* mParent;
    std::unique_ptr<DataInformation> mChildType;
};

inline DataInformation* AbstractArrayData::childType() const
{
    return mChildType.get();
}

#endif // KASTEN_ABSTRACTARRAYDATA_HPP
