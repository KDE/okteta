/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>
    SPDX-FileCopyrightText: 2016 Aaron Bishop <erroneous@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGDATAINFORMATION_HPP
#define KASTEN_STRINGDATAINFORMATION_HPP

// lib
#include "stringdata.hpp"
#include <datainformation.hpp>
#include <dummydatainformation.hpp>
// Std
#include <array>
#include <memory>

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

    static constexpr auto EncodingTypeCount = static_cast<std::size_t>(StringDataInformation::StringType::EBCDIC) + 2;

public:
    StringDataInformation(const QString& name, StringType encoding, DataInformationBase* parent = nullptr);
    ~StringDataInformation() override;

public: // DataInformationWithDummyChildren API
    [[nodiscard]]
    QVariant childData(int row, int column, int role) const override;
    [[nodiscard]]
    Qt::ItemFlags childFlags(int row, int column, bool fileLoaded = true) const override;
    [[nodiscard]]
    BitCount32 childSize(uint index) const override;

    [[nodiscard]]
    QWidget* createChildEditWidget(uint index, QWidget* parent) const override;
    [[nodiscard]]
    QVariant dataFromChildWidget(uint index, const QWidget* w) const override;
    void setChildWidgetData(uint index, QWidget* w) const override;

    [[nodiscard]]
    QString childTypeName(uint index) const override;
    [[nodiscard]]
    QString childString(uint index) const override;
    [[nodiscard]]
    bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                      Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;
    [[nodiscard]]
    QScriptValue childToScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const override;

public: // DataInformation API
    [[nodiscard]]
    unsigned int childCount() const override;
    [[nodiscard]]
    DataInformation* childAt(unsigned int) const override;
    [[nodiscard]]
    BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const override;

    [[nodiscard]]
    Qt::ItemFlags flags(int column, bool fileLoaded = true) const override;
    [[nodiscard]]
    QVariant data(int column, int role) const override;

    [[nodiscard]]
    QWidget* createEditWidget(QWidget* parent) const override;
    [[nodiscard]]
    QVariant dataFromWidget(const QWidget* w) const override;
    void setWidgetData(QWidget* w) const override;

    [[nodiscard]]
    BitCount32 size() const override;

    [[nodiscard]]
    qint64 readData(const Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                    BitCount64 bitsRemaining, quint8* bitOffset) override;
    [[nodiscard]]
    bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* input,
                 Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset) override;

public: // DataInformationBase API
    [[nodiscard]]
    bool isString() const override;

public:
    [[nodiscard]]
    StringType encoding() const;
    [[nodiscard]]
    QString encodingName() const;
    void setEncoding(StringType encoding);
    [[nodiscard]]
    uint terminationCodePoint() const;
    void setTerminationCodePoint(uint term);
    [[nodiscard]]
    uint maxCharCount() const;
    void setMaxCharCount(uint count);
    [[nodiscard]]
    uint maxByteCount() const;
    void setMaxByteCount(uint count);
    [[nodiscard]]
    int stringLength() const;
    [[nodiscard]]
    int stringByteLength() const;
    [[nodiscard]]
    uint terminationMode() const;
    [[nodiscard]]
    QString childNameAt(int index) const;
    [[nodiscard]]
    QString valueAt(int index) const;
    /** Removes this mode from the termination modes. If none is left, changes string to null terminated
     * @param mode The mode to remove
     */
    void unsetTerminationMode(StringData::TerminationMode mode);

private: // DataInformation API
    [[nodiscard]]
    QString typeNameImpl() const override;
    [[nodiscard]]
    QString valueStringImpl() const override;
    [[nodiscard]]
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;

private:
    mutable DummyDataInformation mDummy;
    std::unique_ptr<StringData> mData;
    StringType mEncoding = StringType::InvalidEncoding;

    static const std::array<QString, EncodingTypeCount> encodingNames;
};

inline StringDataInformation::StringType StringDataInformation::encoding() const
{
    return mEncoding;
}
inline QString StringDataInformation::encodingName() const
{
    const auto index = static_cast<std::size_t>(mEncoding) + 1;
    return encodingNames[index];
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

inline QString StringDataInformation::childNameAt(int index) const
{
    Q_ASSERT((uint)index < mData->count());
    // TODO termination char
    return QString(QLatin1Char('[') + QString::number(index) + QLatin1Char(']'));
}

inline QString StringDataInformation::valueAt(int index) const
{
    Q_ASSERT((uint)index < mData->count());
    return mData->stringValue(index);
}

#endif // KASTEN_STRINGDATAINFORMATION_HPP
