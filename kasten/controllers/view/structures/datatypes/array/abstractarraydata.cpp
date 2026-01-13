/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractarraydata.hpp"

// lib
#include "arraydatainformation.hpp"
#include "primitivearraydata.hpp"
#include "complexarraydata.hpp"
#include <primitivedatainformation.hpp>
// Std
#include <utility>

AbstractArrayData::AbstractArrayData(std::unique_ptr<DataInformation>&& childType, ArrayDataInformation* parent)
    : mParent(parent)
    , mChildType(std::move(childType))
{
    mChildType->setParent(parent);
}

AbstractArrayData::~AbstractArrayData() = default;

void AbstractArrayData::setParent(ArrayDataInformation* parent)
{
    mParent = parent;
    mChildType->setParent(parent);
    setNewParentForChildren();
}

std::unique_ptr<AbstractArrayData> AbstractArrayData::newArrayData(uint supportedLength, uint length,
                                                                   std::unique_ptr<DataInformation>&& type,
                                                                   ArrayDataInformation* parent)
{
    if (!type->isPrimitive()) {
        return std::make_unique<ComplexArrayData>(supportedLength, length, std::move(type), parent);
    }
    DataInformation* const rawTyoe = type.release();
    auto data = std::unique_ptr<PrimitiveDataInformation>(rawTyoe->asPrimitive());

    // TODO: support also structs composed of just simple types with-in PrimitiveArrayData, being const sizes
    switch (data->type())
    {
    case PrimitiveDataType::Char:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Char>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::Int8:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Int8>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::Int16:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Int16>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::Int32:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Int32>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::Int64:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Int64>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::UInt8:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::UInt8>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::UInt16:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::UInt16>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::UInt32:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::UInt32>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::UInt64:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::UInt64>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::Bool8:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Bool8>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::Bool16:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Bool16>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::Bool32:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Bool32>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::Bool64:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Bool64>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::Float:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Float>>(supportedLength, length, std::move(data), parent);
    case PrimitiveDataType::Double:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Double>>(supportedLength, length, std::move(data), parent);
    default:
        // enum/bitfield/pointer need complex array data
        return std::make_unique<ComplexArrayData>(supportedLength, length, std::move(data), parent);
    }
}
