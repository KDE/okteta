/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>
    SPDX-FileCopyrightText: 2016 Aaron Bishop <erroneous@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGDATAINFORMATION_HPP
#define KASTEN_STRINGDATAINFORMATION_HPP

#include "../datainformation.hpp"
#include "../dummydatainformation.hpp"
#include "stringdata.hpp"

class DummyDataInformation;

class StringDataInformation : public DataInformationWithDummyChildren
{
    DATAINFORMATION_CLONE_DECL(StringDataInformation, DataInformationWithDummyChildren);

public:
    enum class StringType
    {
        InvalidEncoding = -1,
        ASCII = 0,
        Latin1,
        UTF8,
        UTF16_LE,
        UTF16_BE,
        UTF32_LE,
        UTF32_BE,
        EBCDIC
    };

    StringDataInformation(const QString& name, StringType encoding, DataInformationBase* parent = nullptr);
    ~StringDataInformation() override;

    bool canHaveChildren() const override;
    DataInformation* childAt(unsigned int) const override;

    bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* input,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;
    bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                      Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;
    qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;

    BitCount32 size() const override;
    void setWidgetData(QWidget* w) const override;
    QVariant dataFromWidget(const QWidget* w) const override;
    QWidget* createEditWidget(QWidget* parent) const override;
    unsigned int childCount() const override;
    Qt::ItemFlags flags(int column, bool fileLoaded = true) const override;
    bool isString() const override;

    QVariant data(int column, int role) const override;
    QVariant childData(int row, int column, int role) const override;
    Qt::ItemFlags childFlags(int row, int column, bool fileLoaded = true) const override;
    BitCount32 childSize(uint index) const override;
    QString childTypeName(uint index) const override;
    void setChildWidgetData(uint index, QWidget* w) const override;
    QVariant dataFromChildWidget(uint index, const QWidget* w) const override;
    QWidget* createChildEditWidget(uint index, QWidget* parent) const override;
    QScriptValue childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const override;
    BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const override;

    StringType encoding() const;
    QString encodingName() const;
    void setEncoding(StringType encoding);
    uint terminationCodePoint() const;
    void setTerminationCodePoint(uint term);
    uint maxCharCount() const;
    void setMaxCharCount(uint count);
    uint maxByteCount() const;
    void setMaxByteCount(uint count);
    int stringLength() const;
    int stringByteLength() const;
    uint terminationMode() const;
    QString valueAt(int index) const;
    /** Removes this mode from the termination modes. If none is left, changes string to null terminated
     * @param mode The mode to remove
     */
    void unsetTerminationMode(StringData::TerminationMode mode);

private:
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;
    QString typeNameImpl() const override;
    QString valueStringImpl() const override;

private:
    QScopedPointer<DummyDataInformation> mDummy;
    QScopedPointer<StringData> mData;
    StringType mEncoding = StringType::InvalidEncoding;

    static const QString encodingNames[static_cast<int>(StringDataInformation::StringType::EBCDIC)+2];
};

inline StringDataInformation::StringType StringDataInformation::encoding() const
{
    return mEncoding;
}
inline QString StringDataInformation::encodingName() const
{
    return encodingNames[static_cast<int>(mEncoding)+1];
}

inline uint StringDataInformation::maxByteCount() const
{
    return mData->maxByteCount();
}

inline void StringDataInformation::setMaxByteCount(uint count)
{
    mData->setMaxByteCount(count);
}

inline uint StringDataInformation::maxCharCount() const
{
    return mData->terminationCodePoint();
}

inline void StringDataInformation::setMaxCharCount(uint count)
{
    mData->setMaxCharCount(count);
}

inline uint StringDataInformation::terminationCodePoint() const
{
    return mData->terminationCodePoint();
}

inline void StringDataInformation::setTerminationCodePoint(uint term)
{
    mData->setTerminationCodePoint(term);
}

inline int StringDataInformation::stringLength() const
{
    return mData->count();
}

inline int StringDataInformation::stringByteLength() const
{
    return mData->size() / 8;
}

inline uint StringDataInformation::terminationMode() const
{
    return mData->terminationMode();
}

inline QString StringDataInformation::valueAt(int index) const
{
    Q_ASSERT((uint)index < mData->count());
    return mData->stringValue(index);
}

#endif // KASTEN_STRINGDATAINFORMATION_HPP
