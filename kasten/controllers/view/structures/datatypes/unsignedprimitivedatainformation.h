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
#ifndef UNSIGNEDPRIMITIVEDATAINFORMATION_H_
#define UNSIGNEDPRIMITIVEDATAINFORMATION_H_

#include "primitivedatainformation.h"

class UnsignedPrimitiveDataInformation: public PrimitiveDataInformation
{
Q_OBJECT
PRIMITIVEDATAINFORMATION_SUBCLASS_CONSTRUCTORS(UnsignedPrimitive,Primitive)
public:
    virtual inline int displayBase() const
    {
        int base = Kasten::StructViewPreferences::unsignedDisplayBase();
        if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Binary)
        {
            return 2;
        }
        if (base == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Decimal)
        {
            return 10;
        }
        if (base
                == Kasten::StructViewPreferences::EnumUnsignedDisplayBase::Hexadecimal)
        {
            return 16;
        }
        return 10; //safe default value
    }
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual void setWidgetData(QWidget* w) const;
};

#endif /* UNSIGNEDPRIMITIVEDATAINFORMATION_H_ */
