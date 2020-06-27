/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_COMPLEXARRAYDATA_HPP
#define KASTEN_COMPLEXARRAYDATA_HPP

#include "abstractarraydata.hpp"

#include <QVector>

class DataInformation;
class ScriptHandlerInfo;

class ComplexArrayData : public AbstractArrayData
{
    Q_DISABLE_COPY(ComplexArrayData)

public:
    /** Takes ownership of @p data !*/
    ComplexArrayData(unsigned int initialLength, DataInformation* data, ArrayDataInformation* parent);
    ~ComplexArrayData() override;

    QVariant dataAt(uint index, int column, int role) override;

    unsigned int length() const override;
    void setLength(uint newLength) override;
    BitCount32 size() const override;

    QString typeName() const override;

    DataInformation* childAt(unsigned int idx) override;
    int indexOf(const DataInformation* data) const override;
    BitCount64 offset(const DataInformation* child) const override;
    BitCount32 sizeAt(uint index) override;
    Qt::ItemFlags childFlags(int row, int column, bool fileLoaded) override;

    PrimitiveDataType primitiveType() const override;
    bool isComplex() const override;

    QScriptValue toScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) override;

    qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) override;
    bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                      Okteta::Address address, BitCount64 bitsRemaining) override;

    QWidget* createChildEditWidget(uint index, QWidget* parent) const override;
    QVariant dataFromChildWidget(uint index, const QWidget* w) const override;
    void setChildWidgetData(uint index, QWidget* w) const override;

protected:
    void setNewParentForChildren() override;

private:
    void appendChildren(uint from, uint to);

private:
    QVector<DataInformation*> mChildren;
};

#endif // KASTEN_COMPLEXARRAYDATA_HPP
