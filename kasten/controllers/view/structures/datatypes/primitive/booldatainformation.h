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

#ifndef BOOLDATAINFORMATION_H
#define BOOLDATAINFORMATION_H

#include "basicprimitivedatainformation.h"
#include "uintdatainformation.h"
#include <QScriptValue>

template<typename T>
class BoolDataInformation : public BasicPrimitiveDataInformation<T, BoolDataInformation<T> >
{
public:
    explicit BoolDataInformation(const QString& name, DataInformation* parent = 0);
    virtual ~BoolDataInformation();
    virtual BoolDataInformation<T>* clone() const;

    virtual QScriptValue valueAsQScriptValue() const;
    static QScriptValue asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    virtual QString valueString() const;
    static QString valueString(T val);
    static QString valueString(T val, int base);
    static PrimitiveDataType staticType();
    static T fromVariant(const QVariant& value, bool* ok);

    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(T value, QWidget* w);

protected:
    explicit BoolDataInformation(const BoolDataInformation& d);
};

template<>
inline PrimitiveDataType BoolDataInformation<quint8>::staticType()
{
    return Type_Bool8;
}
template<>
inline PrimitiveDataType BoolDataInformation<quint16>::staticType()
{
    return Type_Bool16;
}
template<>
inline PrimitiveDataType BoolDataInformation<quint32>::staticType()
{
    return Type_Bool32;
}
template<>
inline PrimitiveDataType BoolDataInformation<quint64>::staticType()
{
    return Type_Bool64;
}

template<typename T>
inline QScriptValue BoolDataInformation<T>::valueAsQScriptValue() const
{
    return BoolDataInformation<T>::asScriptValue(this->mValue, 0, 0);
}

template<typename T>
inline QScriptValue BoolDataInformation<T>::asScriptValue(T value, QScriptEngine* engine,
        ScriptHandlerInfo* handler)
{
    Q_UNUSED(engine);
    Q_UNUSED(handler);
    return QScriptValue(value != 0);
}

template<typename T>
inline BoolDataInformation<T>::BoolDataInformation(const QString& name, DataInformation* parent)
        : BasicPrimitiveDataInformation<T, BoolDataInformation<T> >(name, parent)
{
}

template<typename T>
inline BoolDataInformation<T>::~BoolDataInformation()
{
}

template<typename T>
inline BoolDataInformation<T>::BoolDataInformation(const BoolDataInformation& d)
        : BasicPrimitiveDataInformation<T, BoolDataInformation<T> >(d)
{
}

template<typename T>
inline BoolDataInformation<T>* BoolDataInformation<T>::clone() const
{
    return new BoolDataInformation<T>(*this);
}

template<typename T>
inline QString BoolDataInformation<T>::valueString(T value)
{
    return BoolDataInformation<T>::valueString(value,
            PrimitiveDataInformation::unsignedDisplayBase());
}

template<typename T>
inline T BoolDataInformation<T>::fromVariant(const QVariant& value, bool* ok)
{
    return UIntDataInformation<T>::fromVariant(value, ok);
}

#endif // BOOLDATAINFORMATION_H
