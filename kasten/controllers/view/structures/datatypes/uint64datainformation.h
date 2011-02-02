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
#ifndef UINT64DATAINFORMATION_H_
#define UINT64DATAINFORMATION_H_

#include "unsignedprimitivedatainformation.h"
#include "../poddecoder/typeeditors/uint64editor.h"

class UInt64DataInformation: public UnsignedPrimitiveDataInformation
{
Q_OBJECT
PRIMITIVEDATAINFORMATION_SUBCLASS_CONSTRUCTORS(UInt64,UnsignedPrimitive)
public:
    DATAINFORMATION_CLONE(UInt64)

    virtual int size() const;
    virtual QString typeName() const;
    virtual QString valueString() const;
    virtual PrimitiveDataType type() const;
    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newVal);
    
    virtual QWidget* createEditWidget(QWidget* parent) const;
private:
    quint64 mValue;
};

inline PrimitiveDataType UInt64DataInformation::type() const
{
    return Type_UInt64;
}

inline int UInt64DataInformation::size() const
{
    return 64;
}

inline QString UInt64DataInformation::typeName() const
{
    return i18nc("Data type", "unsigned long");
}

#endif /* UINT64DATAINFORMATION_H_ */
