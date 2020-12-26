/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2011, 2013 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ADDITIONALDATA_HPP
#define KASTEN_ADDITIONALDATA_HPP

#include <QScriptValue>
#include <QVariant>

/** Additional data which is not needed frequently. Store this in a QHash to save memory */
class AdditionalData
{
public:
    enum class AdditionalDataType
    {
        UpdateFunction,
        ValidationFunction,
        ValidationError,
        ArrayLengthFunction,
        CustomTypeName,
        ToStringFunction,
        PointerInterpreterFunction,
    };
    explicit AdditionalData();
    AdditionalData(const AdditionalData& data);
    ~AdditionalData();
    AdditionalData& operator=(const AdditionalData& data);
    QVariant get(AdditionalDataType entry) const;
    void set(AdditionalDataType entry, const QVariant& value);
    void remove(AdditionalDataType entry);

private:
    QHash<int, QVariant> mData;
};
Q_STATIC_ASSERT(sizeof(AdditionalData) == sizeof(void*));

Q_DECLARE_METATYPE(QScriptValue)

inline AdditionalData::AdditionalData() = default;

inline AdditionalData::AdditionalData(const AdditionalData& data) = default;

inline AdditionalData::~AdditionalData() = default;

inline AdditionalData& AdditionalData::operator=(const AdditionalData& data) = default;

inline QVariant AdditionalData::get(AdditionalData::AdditionalDataType entry) const
{
    return mData.value((int)entry);
}

inline void AdditionalData::set(AdditionalData::AdditionalDataType entry, const QVariant& value)
{
    mData.insert((int)entry, value);
}

inline void AdditionalData::remove(AdditionalData::AdditionalDataType entry)
{
    mData.remove((int)entry);
}

#endif /* KASTEN_ADDITIONALDATA_HPP */
