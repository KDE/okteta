/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011  Alex Richardson <alex.richardson@gmx.de>
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
#ifndef DOUBLEDATAINFORMATION_H_
#define DOUBLEDATAINFORMATION_H_

#include "primitivedatainformation.h"

class DoubleDataInformation: public PrimitiveDataInformation
{
PRIMITIVEDATAINFORMATION_SUBCLASS_CONSTRUCTORS(Double,Primitive)
public:
    DATAINFORMATION_CLONE(Double)

    virtual int size() const;
    virtual QString typeName() const;

    virtual QString valueString() const;
    static QString valueString(double value);
    virtual PrimitiveDataType type() const;
    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);
    virtual QScriptValue valueAsQScriptValue() const;
    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    virtual AllPrimitiveTypes qVariantToAllPrimitiveTypes(const QVariant& value) const;
private:
    double mValue;
};

inline PrimitiveDataType DoubleDataInformation::type() const
{
    return Type_Double;
}

inline int DoubleDataInformation::size() const
{
    return 64;
}

inline QString DoubleDataInformation::typeName() const
{
    return i18nc("Data type", "double");
}

inline QString DoubleDataInformation::valueString(double value)
{
    if (Kasten::StructViewPreferences::localeAwareFloatFormatting())
        return KGlobal::locale()->formatNumber(value, Kasten::StructViewPreferences::floatPrecision());
    else
        return QString::number(value, 'g', Kasten::StructViewPreferences::floatPrecision());
}


#endif /* DOUBLEDATAINFORMATION_H_ */
