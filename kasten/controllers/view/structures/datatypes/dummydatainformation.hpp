/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DUMMYDATAINFORMATION_HPP
#define KASTEN_DUMMYDATAINFORMATION_HPP

// lib
#include "datainformation.hpp"

class DataInformationWithDummyChildren;

class DummyDataInformation : public DataInformation
{
    DATAINFORMATION_CLONE(DummyDataInformation, DataInformation), mIndex(d.mIndex) {}

public:
    explicit DummyDataInformation(DataInformationBase* parent, const QString& name = QString());
    ~DummyDataInformation() override = default;

public: // DataInformation API
    [[nodiscard]]
    unsigned int childCount() const override { return 0; }
    [[nodiscard]]
    DataInformation* childAt(unsigned int) const override { Q_ASSERT(false); return nullptr; }
    [[nodiscard]]
    BitCount64 childPosition(const DataInformation*, Okteta::Address) const override { Q_ASSERT(false); return 0; }

    [[nodiscard]]
    Qt::ItemFlags flags(int column, bool fileLoaded = true) const override;
    [[nodiscard]]
    QVariant data(int column, int role) const override;

    void setWidgetData(QWidget* w) const override;
    [[nodiscard]]
    QVariant dataFromWidget(const QWidget* w) const override;
    [[nodiscard]]
    QWidget* createEditWidget(QWidget* parent) const override;

    [[nodiscard]]
    qint64 readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;
    [[nodiscard]]
    bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;

    [[nodiscard]]
    BitCount32 size() const override;

    [[nodiscard]]
    QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) override;

    [[nodiscard]]
    int indexOf(const DataInformation* const) const override { Q_ASSERT(false); return -1; }

public: // DataInformationBase API
    [[nodiscard]]
    bool isDummy() const override;

public:
    inline void setDummyIndex(uint newIndex) { mIndex = newIndex; }
    [[nodiscard]]
    inline uint dummyIndex() const { return mIndex; }

    inline void setWasAbleToRead(bool wasAbleToRead) { mWasAbleToRead = wasAbleToRead; }

private: // DataInformation API
    [[nodiscard]]
    QString valueStringImpl() const override;
    [[nodiscard]]
    QString typeNameImpl() const override;
    [[nodiscard]]
    QScriptClass* scriptClass(ScriptHandlerInfo*) const override { Q_ASSERT(false); return nullptr; }

private:
    uint mIndex = 0;
    DataInformationWithDummyChildren* parentHelper() const;
};

/**
 * This class declares all methods that are needed if there can be dummy children
 */
class DataInformationWithDummyChildren : public DataInformation
{
protected:
    DataInformationWithDummyChildren(const DataInformationWithDummyChildren& d) = default;

public:
    explicit DataInformationWithDummyChildren(const QString& name, DataInformationBase* parent = nullptr)
        : DataInformation(name, parent)
    {}
    ~DataInformationWithDummyChildren() override = default;

public: // API to implement
    /** the data of child at index @p row. Useful for arrays, or DataInformations with fake children*/
    [[nodiscard]]
    virtual QVariant childData(int row, int column, int role) const = 0;
    [[nodiscard]]
    virtual Qt::ItemFlags childFlags(int row, int column, bool fileLoaded = true) const = 0;
    [[nodiscard]]
    virtual BitCount32 childSize(uint index) const = 0;

    /** create a QWidget for the QItemDelegate */
    [[nodiscard]]
    virtual QWidget* createChildEditWidget(uint index, QWidget* parent) const = 0;
    /** get the needed data from the widget */
    [[nodiscard]]
    virtual QVariant dataFromChildWidget(uint index, const QWidget* w) const = 0;
    /** initialize the delegate widget with the correct data */
    virtual void setChildWidgetData(uint index, QWidget* w) const = 0;

    [[nodiscard]]
    virtual QString childTypeName(uint index) const = 0;
    [[nodiscard]]
    virtual QString childString(uint index) const = 0;
    [[nodiscard]]
    virtual bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                              Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) = 0;
    [[nodiscard]]
    virtual QScriptValue childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const = 0;

public: // DataInformation API
    [[nodiscard]]
    int indexOf(const DataInformation* const data) const override;

public: // DataInformationBase API
    [[nodiscard]]
    bool isDataInformationWithDummyChildren() const override;
};

#endif // KASTEN_DUMMYDATAINFORMATION_HPP
