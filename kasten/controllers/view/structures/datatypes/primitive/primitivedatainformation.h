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
#ifndef PRIMITIVEDATAINFORMATION_H_
#define PRIMITIVEDATAINFORMATION_H_

#include "../datainformation.h"
#include "../primitivedatatype.h"
#include "structviewpreferences.h" //TODO remove once (un)signeddisplaybase in different file

union AllPrimitiveTypes;

class PrimitiveDataInformation : public DataInformation
{
    friend class PrimitiveDataInformationTest; //that unit test needs to change mWasAbleToRead
public:
    explicit PrimitiveDataInformation(const QString& name, DataInformation* parent = 0);
    virtual ~PrimitiveDataInformation();
    virtual PrimitiveDataInformation* clone() const = 0;
    virtual BitCount32 size() const = 0;

    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;

    virtual bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);

    virtual PrimitiveDataType type() const = 0;
    virtual BitCount32 childSize(uint index) const;
    virtual QScriptValue valueAsQScriptValue() const = 0;

    virtual bool isPrimitive() const;
    virtual AllPrimitiveTypes value() const = 0;
    virtual void setValue(AllPrimitiveTypes newValue) = 0;

    static int unsignedDisplayBase();
    static int signedDisplayBase();

protected:
    virtual BitCount32 offset(unsigned int index) const;
    PrimitiveDataInformation(const PrimitiveDataInformation& d);
};

inline BitCount32 PrimitiveDataInformation::childSize(uint index) const
{
    Q_UNUSED(index);
    Q_ASSERT_X(false, "PrimitiveDataInformation::childSize", "This should never be called");
    return 0;
}

inline BitCount32 PrimitiveDataInformation::offset(unsigned int index) const
{
    Q_UNUSED(index)
    Q_ASSERT_X(false, "PrimitiveDataInformation::offset", "This should never be called");
    return 0;
}

inline bool PrimitiveDataInformation::isPrimitive() const
{
    return true;
}

inline int PrimitiveDataInformation::unsignedDisplayBase()
{
    Kasten2::StructViewPreferences::EnumUnsignedDisplayBase::type base =
            Kasten2::StructViewPreferences::unsignedDisplayBase();
    if (base == Kasten2::StructViewPreferences::EnumUnsignedDisplayBase::Binary)
        return 2;
    else if (base == Kasten2::StructViewPreferences::EnumUnsignedDisplayBase::Hexadecimal)
        return 16;
    else
        return 10; //safe default value
}

inline int PrimitiveDataInformation::signedDisplayBase()
{
    Kasten2::StructViewPreferences::EnumSignedDisplayBase::type base =
            Kasten2::StructViewPreferences::signedDisplayBase();
    if (base == Kasten2::StructViewPreferences::EnumSignedDisplayBase::Binary)
        return 2;
    else if (base == Kasten2::StructViewPreferences::EnumSignedDisplayBase::Hexadecimal)
        return 16;
    else
        return 10; //safe default value
}

inline PrimitiveDataInformation::PrimitiveDataInformation(const QString& name,
        DataInformation* parent)
        : DataInformation(name, parent)
{
}

inline PrimitiveDataInformation::PrimitiveDataInformation(const PrimitiveDataInformation& d)
        : DataInformation(d)
{
}

#endif /* PRIMITIVEDATAINFORMATION_H_ */
