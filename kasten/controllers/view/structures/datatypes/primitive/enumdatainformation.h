/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef ENUMDATAINFORMATION_H_
#define ENUMDATAINFORMATION_H_

#include "primitivedatainformation.h"
#include "enumdefinition.h"
#include "abstractenumdatainformation.h"

class EnumDataInformation: public AbstractEnumDataInformation
{
public:
    /** this object takes ownership of @p type */
    EnumDataInformation(QString name, PrimitiveDataInformation* type,
            EnumDefinition::Ptr enumDef, DataInformation* parent = 0);
    EnumDataInformation(const EnumDataInformation& e);
    DATAINFORMATION_CLONE(Enum)
    virtual ~EnumDataInformation();

    virtual QString valueString() const;
    virtual QString getTypeString() const;
    virtual QString typeName() const;

    virtual BitCount32 size() const;
    virtual PrimitiveDataType type() const;
    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);

    virtual qint64 readData(Okteta::AbstractByteArrayModel* input,
            Okteta::Address address, BitCount64 remaining, quint8* bitOffset);
    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);

    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
    virtual AllPrimitiveTypes qVariantToAllPrimitiveTypes(const QVariant& value) const;
    virtual QScriptValue valueAsQScriptValue() const;

protected:
    PrimitiveDataInformation* mValue; //to allow different enum sizes
};

inline PrimitiveDataType EnumDataInformation::type() const
{
    return mValue->type();
}

inline BitCount32 EnumDataInformation::size() const
{
    return mValue->size();
}

#endif /* ENUMDATAINFORMATION_H_ */
