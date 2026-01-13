/*
    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_COMPLEXARRAYDATA_HPP
#define KASTEN_COMPLEXARRAYDATA_HPP

// lib
#include "abstractarraydata.hpp"
// Std
#include <memory>
#include <vector>

class DataInformation;
class ScriptHandlerInfo;

class ComplexArrayData : public AbstractArrayData
{
public:
    /** Takes ownership of @p data !*/
    ComplexArrayData(unsigned int supportedLength, uint length,
                     std::unique_ptr<DataInformation>&& data,
                     ArrayDataInformation* parent);
    ~ComplexArrayData() override;

public: // AbstractArrayData API
    [[nodiscard]]
    unsigned int supportedLength() const override;
    [[nodiscard]]
    unsigned int length() const override;
    void setLength(uint supportedLength, uint length) override;

    [[nodiscard]]
    QString typeName() const override;
    [[nodiscard]]
    QString valueString() const override;

    [[nodiscard]]
    BitCount32 size() const override;

    [[nodiscard]]
    qint64 readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining) override;
    [[nodiscard]]
    QScriptValue toScriptValue(uint index, QScriptEngine* engine,
                               ScriptHandlerInfo* handlerInfo) override;
    [[nodiscard]]
    PrimitiveDataType primitiveType() const override;
    [[nodiscard]]
    bool isComplex() const override;

    [[nodiscard]]
    DataInformation* childAt(unsigned int idx) override;
    [[nodiscard]]
    int indexOf(const DataInformation* data) const override;
    [[nodiscard]]
    BitCount64 offset(const DataInformation* child) const override;
    [[nodiscard]]
    QVariant dataAt(uint index, int column, int role) override;
    [[nodiscard]]
    BitCount32 sizeAt(uint index) const override;
    [[nodiscard]]
    Qt::ItemFlags childFlags(int index, int column, bool fileLoaded) const override;
    [[nodiscard]]
    bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                      Okteta::Address address, BitCount64 bitsRemaining) override;

    [[nodiscard]]
    QWidget* createChildEditWidget(uint index, QWidget* parent) const override;
    [[nodiscard]]
    QVariant dataFromChildWidget(uint index, const QWidget* w) const override;
    void setChildWidgetData(uint index, QWidget* w) const override;

protected: // AbstractArrayData API
    void setNewParentForChildren() override;

private:
    void appendChildren(uint from, uint to);

private:
    std::vector<std::unique_ptr<DataInformation>> mChildren;
    uint m_length = 0;
};

#endif // KASTEN_COMPLEXARRAYDATA_HPP
