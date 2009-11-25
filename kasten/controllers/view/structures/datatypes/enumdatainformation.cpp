/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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
        PrimitiveDataInformation* type, QSharedDataPointer<EnumDefinition> enumDef,
        int index, DataInformation* parent) :
    PrimitiveDataInformation(name, type->type(), index, parent), mEnum(enumDef),
            mValue(type)
{
    if (mType != mEnum->type())
        kWarning() << "incompatible types in definition and value: "
                << enumDef->type() << "and " << mType;
}
EnumDataInformation::EnumDataInformation(const EnumDataInformation& e) :
    PrimitiveDataInformation(e), mEnum(e.mEnum)
{
    mValue = static_cast<PrimitiveDataInformation*> (e.mValue->clone());

}

EnumDataInformation::~EnumDataInformation()
{
    delete mValue;
}

QVariant EnumDataInformation::primitiveValue() const
{
    return mValue->primitiveValue();
}

QString EnumDataInformation::getValueString() const
{
    QString enumVal = mEnum->value(mValue->value());
    if (!enumVal.isEmpty())
    {
        return i18n("%1 (%2)", enumVal, mValue->getValueString());
    }
    else
        return i18n("%1 (value not in enum)", mValue->getValueString());
}
QString EnumDataInformation::getTypeName() const
{
    return "enum";
}
QString EnumDataInformation::getTypeString() const
{
    return i18n("enum (%1)", mValue->getTypeName());
}

bool EnumDataInformation::setData(const QVariant& value, DataInformation* inf,
        Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining)
{
    return mValue->setData(value, inf, out, byteOrder, address, remaining);
}
Okteta::Size EnumDataInformation::readData(Okteta::AbstractByteArrayModel* input,
        ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining)
{
    return mValue->readData(input, byteOrder, address, remaining);
}

