/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011, 2013 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ADDITIONALDATA_H_
#define ADDITIONALDATA_H_

#include <QScriptValue>
#include <QVariant>

/** Additional data which is not needed frequently. Store this in a QHash to save memory */
class AdditionalData
{
public:
    enum AdditionalDataType {
        UpdateFunction, ValidationFunction, ValidationError, ArrayLengthFunction, CustomTypeName, ToStringFunction
    };
    explicit AdditionalData();
    AdditionalData(const AdditionalData& data);
    ~AdditionalData();
    QVariant get(AdditionalDataType entry) const;
    void set(AdditionalDataType entry, const QVariant& value);
    void remove(AdditionalDataType entry);
private:
    QHash<int, QVariant> mData;
};
Q_STATIC_ASSERT(sizeof(AdditionalData) == sizeof(void*));

Q_DECLARE_METATYPE(QScriptValue)

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

inline AdditionalData::AdditionalData()
{
}

inline AdditionalData::AdditionalData(const AdditionalData& data)
    : mData(data.mData)
{
}

inline AdditionalData::~AdditionalData()
{
}

#endif /* ADDITIONALDATA_H_ */
