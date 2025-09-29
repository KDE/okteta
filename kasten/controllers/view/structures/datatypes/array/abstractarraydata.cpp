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
    Q_CHECK_PTR(mChildType);
    mChildType->setParent(parent);
}

AbstractArrayData::~AbstractArrayData() = default;

void AbstractArrayData::setParent(ArrayDataInformation* parent)
{
    mParent = parent;
    mChildType->setParent(parent);
    setNewParentForChildren();
}

std::unique_ptr<AbstractArrayData> AbstractArrayData::newArrayData(uint length,
                                                                   std::unique_ptr<DataInformation>&& type,
                                                                   ArrayDataInformation* parent)
{
    Q_CHECK_PTR(type);
    if (!type->isPrimitive()) {
        return std::make_unique<ComplexArrayData>(length, std::move(type), parent);
    }
    DataInformation* const rawTyoe = type.release();
    auto data = std::unique_ptr<PrimitiveDataInformation>(rawTyoe->asPrimitive());

    switch (data->type())
    {
    case PrimitiveDataType::Char:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Char>>(length, std::move(data), parent);
    case PrimitiveDataType::Int8:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Int8>>(length, std::move(data), parent);
    case PrimitiveDataType::Int16:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Int16>>(length, std::move(data), parent);
    case PrimitiveDataType::Int32:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Int32>>(length, std::move(data), parent);
    case PrimitiveDataType::Int64:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Int64>>(length, std::move(data), parent);
    case PrimitiveDataType::UInt8:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::UInt8>>(length, std::move(data), parent);
    case PrimitiveDataType::UInt16:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::UInt16>>(length, std::move(data), parent);
    case PrimitiveDataType::UInt32:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::UInt32>>(length, std::move(data), parent);
    case PrimitiveDataType::UInt64:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::UInt64>>(length, std::move(data), parent);
    case PrimitiveDataType::Bool8:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Bool8>>(length, std::move(data), parent);
    case PrimitiveDataType::Bool16:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Bool16>>(length, std::move(data), parent);
    case PrimitiveDataType::Bool32:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Bool32>>(length, std::move(data), parent);
    case PrimitiveDataType::Bool64:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Bool64>>(length, std::move(data), parent);
    case PrimitiveDataType::Float:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Float>>(length, std::move(data), parent);
    case PrimitiveDataType::Double:
        return std::make_unique<PrimitiveArrayData<PrimitiveDataType::Double>>(length, std::move(data), parent);
    default:
        // enum/bitfield/pointer need complex array data
        return std::make_unique<ComplexArrayData>(length, std::move(data), parent);
    }
}
