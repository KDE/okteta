/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractarraydata.hpp"
#include "arraydatainformation.hpp"
#include "primitivearraydata.hpp"
#include "complexarraydata.hpp"
#include "../primitive/primitivedatainformation.hpp"

AbstractArrayData::AbstractArrayData(DataInformation* childType, ArrayDataInformation* parent)
    : mParent(parent)
    , mChildType(childType)
{
    Q_CHECK_PTR(childType);
    mChildType->setParent(parent);
}

AbstractArrayData::~AbstractArrayData() = default;

void AbstractArrayData::setParent(ArrayDataInformation* parent)
{
    mParent = parent;
    mChildType->setParent(parent);
    setNewParentForChildren();
}

AbstractArrayData* AbstractArrayData::newArrayData(uint length, DataInformation* type, ArrayDataInformation* parent)
{
    Q_CHECK_PTR(type);
    if (!type->isPrimitive()) {
        return new ComplexArrayData(length, type, parent);
    }
    PrimitiveDataInformation* data = type->asPrimitive();

    switch (data->type())
    {
    case PrimitiveDataType::Char:
        return new PrimitiveArrayData<PrimitiveDataType::Char>(length, data, parent);
    case PrimitiveDataType::Int8:
        return new PrimitiveArrayData<PrimitiveDataType::Int8>(length, data, parent);
    case PrimitiveDataType::Int16:
        return new PrimitiveArrayData<PrimitiveDataType::Int16>(length, data, parent);
    case PrimitiveDataType::Int32:
        return new PrimitiveArrayData<PrimitiveDataType::Int32>(length, data, parent);
    case PrimitiveDataType::Int64:
        return new PrimitiveArrayData<PrimitiveDataType::Int64>(length, data, parent);
    case PrimitiveDataType::UInt8:
        return new PrimitiveArrayData<PrimitiveDataType::UInt8>(length, data, parent);
    case PrimitiveDataType::UInt16:
        return new PrimitiveArrayData<PrimitiveDataType::UInt16>(length, data, parent);
    case PrimitiveDataType::UInt32:
        return new PrimitiveArrayData<PrimitiveDataType::UInt32>(length, data, parent);
    case PrimitiveDataType::UInt64:
        return new PrimitiveArrayData<PrimitiveDataType::UInt64>(length, data, parent);
    case PrimitiveDataType::Bool8:
        return new PrimitiveArrayData<PrimitiveDataType::Bool8>(length, data, parent);
    case PrimitiveDataType::Bool16:
        return new PrimitiveArrayData<PrimitiveDataType::Bool16>(length, data, parent);
    case PrimitiveDataType::Bool32:
        return new PrimitiveArrayData<PrimitiveDataType::Bool32>(length, data, parent);
    case PrimitiveDataType::Bool64:
        return new PrimitiveArrayData<PrimitiveDataType::Bool64>(length, data, parent);
    case PrimitiveDataType::Float:
        return new PrimitiveArrayData<PrimitiveDataType::Float>(length, data, parent);
    case PrimitiveDataType::Double:
        return new PrimitiveArrayData<PrimitiveDataType::Double>(length, data, parent);
    default:
        // enum/bitfield/pointer need complex array data
        return new ComplexArrayData(length, data, parent);
    }
}
