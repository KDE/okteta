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

#include "basicprimitivedatainformation.h"

template<typename T>
class UIntDataInformation : public BasicPrimitiveDataInformation<T, UIntDataInformation<T> >
{
public:
    explicit UIntDataInformation(const QString& name, DataInformation* parent = 0);
    virtual ~UIntDataInformation() {}
    virtual UIntDataInformation<T>* clone() const;

    static QScriptValue asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    static QString staticValueString(T val);
    static QString staticValueString(T val, int base);
    static PrimitiveDataType staticType();
    static T fromVariant(const QVariant& value, bool* ok);

    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(T value, QWidget* w);
protected:
    explicit UIntDataInformation(const UIntDataInformation& d);
};

template<>
inline PrimitiveDataType UIntDataInformation<quint8>::staticType() { return Type_UInt8; }
template<>
inline PrimitiveDataType UIntDataInformation<quint16>::staticType() { return Type_UInt16; }
template<>
inline PrimitiveDataType UIntDataInformation<quint32>::staticType() { return Type_UInt32; }
template<>
inline PrimitiveDataType UIntDataInformation<quint64>::staticType() { return Type_UInt64; }

template<typename T>
inline UIntDataInformation<T>::UIntDataInformation(const QString& name, DataInformation* parent)
        : BasicPrimitiveDataInformation<T, UIntDataInformation<T> >(name, parent)
{
}

template<typename T>
inline T UIntDataInformation<T>::fromVariant(const QVariant& value, bool* ok)
{
    Q_CHECK_PTR(ok);
    quint64 val = value.toULongLong(ok);
    T result = T(val);
    if (val != quint64(result))
        *ok = false;
    return result;
}

template<typename T>
inline UIntDataInformation<T>::UIntDataInformation(const UIntDataInformation& d)
        : BasicPrimitiveDataInformation<T, UIntDataInformation<T> >(d)
{
}

template<typename T>
inline UIntDataInformation<T>* UIntDataInformation<T>::clone() const
{
    return new UIntDataInformation<T>(*this);
}

template<typename T>
inline QString UIntDataInformation<T>::staticValueString(T value)
{
    return UIntDataInformation<T>::staticValueString(value, PrimitiveDataInformation::unsignedDisplayBase());
}



#endif // UINTDATAINFORMATION_H
