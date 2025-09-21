/*
    SPDX-FileCopyrightText: 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DATAINFORMATIONWITHCHILDREN_HPP
#define KASTEN_DATAINFORMATIONWITHCHILDREN_HPP

// lib
#include "datainformation.hpp"
// Std
#include <memory>
#include <vector>

class DataInformationWithChildren : public DataInformation
{
protected:
    explicit DataInformationWithChildren(const DataInformationWithChildren& d);

public:
    explicit DataInformationWithChildren(const QString& name,
                                         std::vector<std::unique_ptr<DataInformation>>&& children = {},
                                         DataInformation* parent = nullptr);
    ~DataInformationWithChildren() override;

public: // API to implement
    /** replaces child at @p index with @p newChild.
     * @param index the index of the child
     * @param newChild the new child (ownership is taken if replacing is successful,
     *  otherwise it must be deleted)
     * @return true if replacing was successful, false otherwise */
    [[nodiscard]]
    virtual bool replaceChildAt(unsigned int index, DataInformation* newChild);

public: // DataInformation API
    [[nodiscard]]
    unsigned int childCount() const override;
    [[nodiscard]]
    DataInformation* childAt(unsigned int index) const override;

    [[nodiscard]]
    QString tooltipString() const override;

    [[nodiscard]]
    QWidget* createEditWidget(QWidget* parent) const override;
    /** get the needed data from the widget */
    [[nodiscard]]
    QVariant dataFromWidget(const QWidget* w) const override;
    void setWidgetData(QWidget* w) const override;

    [[nodiscard]]
    BitCount32 size() const override;

    [[nodiscard]]
    bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;

    [[nodiscard]]
    int indexOf(const DataInformation* const data) const override;

    void resetValidationState() override;

public: // DataInformationBase API
    [[nodiscard]]
    bool isDataInformationWithChildren() const override;

public:
    void calculateValidationState();
    [[nodiscard]]
    QVariant childData(int row, int column, int role) const;
    /** Takes ownership! */
    void appendChild(std::unique_ptr<DataInformation>&& child, bool emitSignal = true);
    /** Takes ownership of all elements */
    void setChildren(std::vector<std::unique_ptr<DataInformation>>&& newChildren);
    void setChildren(const QScriptValue& newChildren);

public:
    [[nodiscard]]
    static bool readChildren(const std::vector<std::unique_ptr<DataInformation>>& children,
                             const Okteta::AbstractByteArrayModel* input,
                             Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset, qint64* readBitsPtr,
                             TopLevelDataInformation* top);

protected:
    [[nodiscard]]
    static std::vector<std::unique_ptr<DataInformation>> cloneList(const std::vector<std::unique_ptr<DataInformation>>& other, DataInformation* parent);

private: // DataInformation API
    [[nodiscard]]
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;

protected:
    std::vector<std::unique_ptr<DataInformation>> mChildren;
};

#endif /* KASTEN_DATAINFORMATIONWITHCHILDREN_HPP */
