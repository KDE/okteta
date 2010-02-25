/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "enumdatainformation.h"

EnumDataInformation::EnumDataInformation(QString name,
        PrimitiveDataInformation* type, EnumDefinition::Ptr enumDef, int index,
        DataInformation* parent) :
    PrimitiveDataInformation(name, type->type(), index, parent), mEnum(enumDef),
            mValue(type)
{
    if (mType != mEnum->type())
        kWarning() << "incompatible types in definition and value: "
                << enumDef->type() << "and " << mType;
    connect(mValue, SIGNAL(dataChanged()), SLOT(onChildDataChanged()));
}

EnumDataInformation::EnumDataInformation(const EnumDataInformation& e) :
    PrimitiveDataInformation(e), mEnum(e.mEnum)
{
    mValue = static_cast<PrimitiveDataInformation*> (e.mValue->clone());
    connect(mValue, SIGNAL(dataChanged()), SLOT(onChildDataChanged()));
}

EnumDataInformation::~EnumDataInformation()
{
    delete mValue;
}

QVariant EnumDataInformation::primitiveValue() const
{
    return mValue->primitiveValue();
}

QString EnumDataInformation::valueString() const
{
    QString enumVal = mEnum->value(mValue->value());
    if (!enumVal.isEmpty())
    {
        return i18n("%1 (%2)", enumVal, mValue->valueString());
    }
    else
        return i18n("%1 (value not in enum)", mValue->valueString());
}
QString EnumDataInformation::typeName() const
{
    return "enum";
}
QString EnumDataInformation::getTypeString() const
{
    return i18n("enum (%1)", mValue->typeName());
}

bool EnumDataInformation::setData(const QVariant& value, DataInformation* inf,
        Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining, quint8* bitOffset)
{
    if (this != inf)
        return false;
    //correct object -> use mValue so PrimitiveDataInformation::setData() returns true
    bool ret = mValue->setData(value, mValue, out, byteOrder, address, remaining,
            bitOffset);
    return ret;
}
Okteta::Size EnumDataInformation::readData(Okteta::AbstractByteArrayModel* input,
        ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining,
        quint8* bitOffset)
{
    return mValue->readData(input, byteOrder, address, remaining, bitOffset);
}

QWidget* EnumDataInformation::createEditWidget(QWidget* parent) const
{
    return mValue->createEditWidget(parent);
}

QVariant EnumDataInformation::dataFromWidget(const QWidget* w) const
{
    return mValue->dataFromWidget(w);
}

void EnumDataInformation::setWidgetData(QWidget* w) const
{
    mValue->setWidgetData(w);
}
