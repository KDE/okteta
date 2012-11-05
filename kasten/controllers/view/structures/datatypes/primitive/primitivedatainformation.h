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
#include "../../allprimitivetypes.h"
#include "structviewpreferences.h"

union AllPrimitiveTypes;

/**
 * A base class for all primitive data elements (e.g. unsigned integers or floating-point numbers)
 */
class PrimitiveDataInformation : public DataInformation
{
    friend class PrimitiveDataInformationTest; //that unit test needs to change mWasAbleToRead
public:
    explicit PrimitiveDataInformation(const QString& name, DataInformation* parent = 0);
    virtual ~PrimitiveDataInformation();
    virtual PrimitiveDataInformation* clone() const = 0;

    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;


    virtual bool isPrimitive() const;
    virtual AllPrimitiveTypes value() const = 0;
    virtual void setValue(AllPrimitiveTypes newValue) = 0;
    virtual QScriptValue valueAsQScriptValue() const = 0;
    virtual PrimitiveDataType type() const = 0;

    virtual unsigned int childCount() const { return 0; }
    virtual DataInformation* childAt(unsigned int) const { Q_ASSERT(false); return 0; }
    virtual bool canHaveChildren() const { return false; }
    virtual BitCount64 childPosition(const DataInformation*, Okteta::Address) const { Q_ASSERT(false); return 0; }
    virtual int indexOf(const DataInformation* const) const {Q_ASSERT(false); return -1; }

    static int unsignedDisplayBase();
    static int signedDisplayBase();

protected:
    virtual BitCount32 offset(unsigned int index) const;
    PrimitiveDataInformation(const PrimitiveDataInformation& d);
};

/**
 * A base class for data types which just wrap an underlying primitive data type.
 * The basic operations (reading/writing/size/value) are simply delegated to the wrapped element.
 *
 * This is used for pointers (wrapping differently sized integers) and enumerations/bitflags
 * Due to the use of a PrimitiveDataInformation pointer as a member any primitive type can be wrapped.
 * For example pointers using a bitfield value are possible and theoretically floating point enums
 * (although these were not added due to float comparison issues - use the binary representation of an
 * equally sized integer instead).
 */
class PrimitiveDataInformationWrapper : public PrimitiveDataInformation
{
protected:
    PrimitiveDataInformationWrapper(const PrimitiveDataInformationWrapper& d);
public:
    /** takes ownership of @p valueType */
    PrimitiveDataInformationWrapper(const QString& name, PrimitiveDataInformation* valueType,
            DataInformation* parent = 0);
    virtual ~PrimitiveDataInformationWrapper() {}

    //delegate all these to the underlying object:

    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset)
    { return mValue->setData(value, out, address, bitsRemaining, bitOffset); }



    virtual BitCount32 size() const { return mValue->size(); }

    virtual void setWidgetData(QWidget* w) const {mValue->setWidgetData(w); }

    virtual QVariant dataFromWidget(const QWidget* w) const { return mValue->dataFromWidget(w); }

    virtual QWidget* createEditWidget(QWidget* parent) const { return mValue->createEditWidget(parent); }

    virtual AllPrimitiveTypes value() const { return mValue->value(); }

    virtual void setValue(AllPrimitiveTypes newValue) { mValue->setValue(newValue); }

    //classes derived from this are not true primitive types (they provide additional information)
    virtual PrimitiveDataType type() const { return Type_Invalid; }


    virtual QScriptValue valueAsQScriptValue() const;

    //we have to do slightly more than just forward with this method
    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            BitCount64 bitsRemaining, quint8* bitOffset);
protected:
    QScopedPointer<PrimitiveDataInformation> mValue;
};


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
