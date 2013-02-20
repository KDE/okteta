/*
 *    This file is part of the Okteta Kasten module, made within the KDE community.
 *
 *    Copyright 2012  Alex Richardson <alex.richardson@gmx.de>
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <QString>
#include <QtTest/QTest>
#include "view/structures/datatypes/primitivedatatype.h"
#include "view/structures/datatypes/datainformation.h"
#include "view/structures/datatypes/primitive/primitivedatainformation.h"
#include "view/structures/datatypes/primitive/bitfield/signedbitfielddatainformation.h"
#include "view/structures/datatypes/primitive/bitfield/unsignedbitfielddatainformation.h"
#include "view/structures/datatypes/primitive/bitfield/boolbitfielddatainformation.h"

namespace Utils
{

/** Converts a string to a binary number (spaces are ignored)
 * @param val the string representing a binary number
 * @return
 */
template<typename T>
T binary(const char* val)
{
    QString value = QLatin1String(val);
    value = value.remove(QLatin1Char(' '));
    Q_ASSERT(unsigned(value.length()) <= sizeof(T) * 8); //otherwise we overflow
    bool ok = false;
    quint64 result = value.toULongLong(&ok, 2);
    Q_ASSERT(ok);
    return static_cast<T>(result);
}

struct DataInformationCheck
{
    virtual ~DataInformationCheck() {}
    virtual void check(DataInformation* data) = 0;
};

struct PrimitiveTypeCheck : public DataInformationCheck
{
    virtual ~PrimitiveTypeCheck() {}
    PrimitiveTypeCheck(PrimitiveDataType type) : mType(type) {};
    virtual void check(DataInformation* data)
    {
        QVERIFY(data->isPrimitive());
        QCOMPARE(data->asPrimitive()->type().value, mType.value);
    }
private:
    PrimitiveDataType mType;
};

struct SignedBitfieldCheck : public DataInformationCheck
{
    virtual ~SignedBitfieldCheck() {}
    virtual void check(DataInformation* data)
    {
        QVERIFY(data->isBitfield());
        QVERIFY(dynamic_cast<SignedBitfieldDataInformation*>(data));
    }
};

struct UnsignedBitfieldCheck : public DataInformationCheck
{
    virtual ~UnsignedBitfieldCheck() {}
    virtual void check(DataInformation* data)
    {
        QVERIFY(data->isBitfield());
        QVERIFY(dynamic_cast<UnsignedBitfieldDataInformation*>(data));
    }
};

struct BoolBitfieldCheck : public DataInformationCheck
{
    virtual ~BoolBitfieldCheck() {}
    virtual void check(DataInformation* data)
    {
        QVERIFY(data->isBitfield());
        QVERIFY(dynamic_cast<BoolBitfieldDataInformation*>(data));
    }
};

}
Q_DECLARE_METATYPE(Utils::DataInformationCheck*)

#endif /* TESTUTILS_H_ */
