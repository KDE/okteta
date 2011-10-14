/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011  Alex Richardson <alex.richardson@gmx.de>
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
#ifndef UINTDATAINFORMATION_H
#define UINTDATAINFORMATION_H

#include "unsigneddatainformation.h"

template<typename T, PrimitiveDataType typeValue>
class UIntDataInformation : public UnsignedDataInformation<T>
{
public:
    explicit UIntDataInformation(QString name, DataInformation* parent = 0);
    virtual ~UIntDataInformation() {}
    virtual UIntDataInformation<T, typeValue>* clone() const;

    virtual QScriptValue valueAsQScriptValue() const;
    static QScriptValue asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    virtual QString valueString() const;
    static QString valueString(T val);
    virtual PrimitiveDataType type() const;
    virtual QString typeName() const;
protected:
    explicit UIntDataInformation(const UIntDataInformation& d);
};

template<typename T, PrimitiveDataType typeValue>
inline PrimitiveDataType UIntDataInformation<T, typeValue>::type() const
{
    return typeValue;
}

template<typename T, PrimitiveDataType typeValue>
inline QString UIntDataInformation<T, typeValue>::typeName() const
{
    return PrimitiveDataInformation::typeName(typeValue);
}

template<typename T, PrimitiveDataType typeValue>
inline UIntDataInformation<T, typeValue>::UIntDataInformation(QString name, DataInformation* parent)
        : UnsignedDataInformation<T>(name, parent)
{
}

template<typename T, PrimitiveDataType typeValue>
inline UIntDataInformation<T, typeValue>::UIntDataInformation(const UIntDataInformation& d)
        : UnsignedDataInformation<T>(d)
{
}

template<typename T, PrimitiveDataType typeValue>
inline UIntDataInformation<T, typeValue>* UIntDataInformation<T, typeValue>::clone() const
{
    return new UIntDataInformation<T, typeValue>(*this);
}

#endif // UINTDATAINFORMATION_H
