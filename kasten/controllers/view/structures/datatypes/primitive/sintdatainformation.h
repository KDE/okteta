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
#ifndef SINTDATAINFORMATION_H
#define SINTDATAINFORMATION_H

#include "basicprimitivedatainformation.h"
#include "../poddecoder/typeeditors/sintspinbox.h"

template<typename T>
class SIntDataInformation : public BasicPrimitiveDataInformation<T, SIntDataInformation<T> >
{
public:
    explicit SIntDataInformation(const QString& name, DataInformation* parent = 0);
    virtual ~SIntDataInformation();
    virtual SIntDataInformation<T>* clone() const;

    static PrimitiveDataType staticType();
    static T fromVariant(const QVariant& value, bool* ok);
    static QScriptValue asScriptValue(T value, QScriptEngine* engine, ScriptHandlerInfo* handler);
    static QString staticValueString(T val);
    static QString staticValueString(T val, int base);

    static QWidget* staticCreateEditWidget(QWidget* parent);
    static QVariant staticDataFromWidget(const QWidget* w);
    static void staticSetWidgetData(T value, QWidget* w);

protected:
    explicit SIntDataInformation(const SIntDataInformation& d);
};

template<>
inline PrimitiveDataType SIntDataInformation<qint8>::staticType()
{
    return Type_Int8;
}
template<>
inline PrimitiveDataType SIntDataInformation<qint16>::staticType()
{
    return Type_Int16;
}
template<>
inline PrimitiveDataType SIntDataInformation<qint32>::staticType()
{
    return Type_Int32;
}
template<>
inline PrimitiveDataType SIntDataInformation<qint64>::staticType()
{
    return Type_Int64;
}

template<typename T>
inline QWidget* SIntDataInformation<T>::staticCreateEditWidget(QWidget* parent)
{
    SIntSpinBox* ret = new SIntSpinBox(parent, PrimitiveDataInformation::signedDisplayBase());
    ret->setRange(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    return ret;
}

template<typename T>
inline QVariant SIntDataInformation<T>::staticDataFromWidget(const QWidget* w)
{
    const SIntSpinBox* spin = dynamic_cast<const SIntSpinBox*>(w);
    Q_CHECK_PTR(spin);
    if (spin)
        return spin->value();

    kWarning() << "could not cast widget";
    return QVariant();
}

template<typename T>
inline void SIntDataInformation<T>::staticSetWidgetData(T value, QWidget* w)
{
    SIntSpinBox* spin = dynamic_cast<SIntSpinBox*>(w);
    Q_CHECK_PTR(spin);
    if (spin)
        spin->setValue(value);
}

template<typename T>
inline SIntDataInformation<T>::SIntDataInformation(const QString& name, DataInformation* parent)
        : BasicPrimitiveDataInformation<T, SIntDataInformation<T> >(name, parent)
{
}

template<typename T>
inline SIntDataInformation<T>::~SIntDataInformation()
{
}

template<typename T>
inline SIntDataInformation<T>::SIntDataInformation(const SIntDataInformation& d)
        : BasicPrimitiveDataInformation<T, SIntDataInformation<T> >(d)
{
}

template<typename T>
inline SIntDataInformation<T>* SIntDataInformation<T>::clone() const
{
    return new SIntDataInformation<T>(*this);
}

template<typename T>
inline QString SIntDataInformation<T>::staticValueString(T value)
{
    return SIntDataInformation<T>::staticValueString(value, PrimitiveDataInformation::signedDisplayBase());
}

template<typename T>
inline T SIntDataInformation<T>::fromVariant(const QVariant& value, bool* ok)
{
    Q_CHECK_PTR(ok);
    qint64 val = value.toLongLong(ok);
    T result = T(val);
    if (val != qint64(result))
        *ok = false;
    return result;
}

#endif // SINTDATAINFORMATION_H
