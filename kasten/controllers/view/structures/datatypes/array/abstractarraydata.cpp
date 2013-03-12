/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
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


#include "abstractarraydata.h"
#include "arraydatainformation.h"
#include "primitivearraydata.h"
#include "complexarraydata.h"
#include "../primitive/primitivedatainformation.h"

AbstractArrayData::AbstractArrayData(DataInformation* childType, ArrayDataInformation* parent)
        : mParent(parent), mChildType(childType)
{
    Q_CHECK_PTR(childType);
    mChildType->setParent(parent);
}

AbstractArrayData::~AbstractArrayData()
{
}

void AbstractArrayData::setParent(ArrayDataInformation* parent)
{
    mParent = parent;
    mChildType->setParent(parent);
    setNewParentForChildren();
}

AbstractArrayData* AbstractArrayData::newArrayData(uint length, DataInformation* type, ArrayDataInformation* parent)
{
    Q_CHECK_PTR(type);
    if (!type->isPrimitive())
        return new ComplexArrayData(length, type, parent);
    PrimitiveDataInformation* data = type->asPrimitive();

    switch (data->type().value)
    {
    case Type_Char:
        return new PrimitiveArrayData<Type_Char>(length, data, parent);
    case Type_Int8:
        return new PrimitiveArrayData<Type_Int8>(length, data, parent);
    case Type_Int16:
        return new PrimitiveArrayData<Type_Int16>(length, data, parent);
    case Type_Int32:
        return new PrimitiveArrayData<Type_Int32>(length, data, parent);
    case Type_Int64:
        return new PrimitiveArrayData<Type_Int64>(length, data, parent);
    case Type_UInt8:
        return new PrimitiveArrayData<Type_UInt8>(length, data, parent);
    case Type_UInt16:
        return new PrimitiveArrayData<Type_UInt16>(length, data, parent);
    case Type_UInt32:
        return new PrimitiveArrayData<Type_UInt32>(length, data, parent);
    case Type_UInt64:
        return new PrimitiveArrayData<Type_UInt64>(length, data, parent);
    case Type_Bool8:
        return new PrimitiveArrayData<Type_Bool8>(length, data, parent);
    case Type_Bool16:
        return new PrimitiveArrayData<Type_Bool16>(length, data, parent);
    case Type_Bool32:
        return new PrimitiveArrayData<Type_Bool32>(length, data, parent);
    case Type_Bool64:
        return new PrimitiveArrayData<Type_Bool64>(length, data, parent);
    case Type_Float:
        return new PrimitiveArrayData<Type_Float>(length, data, parent);
    case Type_Double:
        return new PrimitiveArrayData<Type_Double>(length, data, parent);
    default:
        // enum/bitfield/pointer need complex array data
        return new ComplexArrayData(length, data, parent);
    }
}
