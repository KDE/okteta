/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PRIMITIVETEMPLATEINFO_HPP
#define KASTEN_PRIMITIVETEMPLATEINFO_HPP

#include "../primitivedatatype.hpp"
#include "booldatainformation.hpp"
#include "floatdatainformation.hpp"
#include "doubledatainformation.hpp"
#include "uintdatainformation.hpp"
#include "sintdatainformation.hpp"
#include "chardatainformation.hpp"
#include "basicprimitivedatainformation.hpp"

using UInt8DataInformation =  BasicPrimitiveDataInformation<quint8,  UIntDataInformationMethods<quint8>>;
using UInt16DataInformation = BasicPrimitiveDataInformation<quint16, UIntDataInformationMethods<quint16>>;
using UInt32DataInformation = BasicPrimitiveDataInformation<quint32, UIntDataInformationMethods<quint32>>;
using UInt64DataInformation = BasicPrimitiveDataInformation<quint64, UIntDataInformationMethods<quint64>>;

using SInt8DataInformation =  BasicPrimitiveDataInformation<qint8,  SIntDataInformationMethods<qint8>>;
using SInt16DataInformation = BasicPrimitiveDataInformation<qint16, SIntDataInformationMethods<qint16>>;
using SInt32DataInformation = BasicPrimitiveDataInformation<qint32, SIntDataInformationMethods<qint32>>;
using SInt64DataInformation = BasicPrimitiveDataInformation<qint64, SIntDataInformationMethods<qint64>>;

using Bool8DataInformation =  BasicPrimitiveDataInformation<quint8,  BoolDataInformationMethods<quint8>>;
using Bool16DataInformation = BasicPrimitiveDataInformation<quint16, BoolDataInformationMethods<quint16>>;
using Bool32DataInformation = BasicPrimitiveDataInformation<quint32, BoolDataInformationMethods<quint32>>;
using Bool64DataInformation = BasicPrimitiveDataInformation<quint64, BoolDataInformationMethods<quint64>>;

using CharDataInformation =   BasicPrimitiveDataInformation<quint8, CharDataInformationMethods>;
using FloatDataInformation =  BasicPrimitiveDataInformation<float,  FloatDataInformationMethods>;
using DoubleDataInformation = BasicPrimitiveDataInformation<double, DoubleDataInformationMethods>;

// just to simplify usage of those classes (no need to specify two template parameters)
template <PrimitiveDataType T> struct PrimitiveInfo
{
    using valueType = void;
    using Class =     void; // to cause compiler errors with e.g. PrimitiveDataType::NotPrimitive
    using Methods =   void; // to cause compiler errors with e.g. PrimitiveDataType::NotPrimitive
};

template <> struct PrimitiveInfo<PrimitiveDataType::Bool8>
{
    using valueType = quint8;
    using Class =     Bool8DataInformation;
    using Methods =   BoolDataInformationMethods<valueType>;
};
template <> struct PrimitiveInfo<PrimitiveDataType::Bool16>
{
    using valueType = quint16;
    using Class =     Bool16DataInformation;
    using Methods =   BoolDataInformationMethods<valueType>;
};
template <> struct PrimitiveInfo<PrimitiveDataType::Bool32>
{
    using valueType = quint32;
    using Class =     Bool32DataInformation;
    using Methods =   BoolDataInformationMethods<valueType>;
};
template <> struct PrimitiveInfo<PrimitiveDataType::Bool64>
{
    using valueType = quint64;
    using Class =     Bool64DataInformation;
    using Methods =   BoolDataInformationMethods<valueType>;
};

template <> struct PrimitiveInfo<PrimitiveDataType::UInt8>
{
    using valueType = quint8;
    using Class =     UInt8DataInformation;
    using Methods =   UIntDataInformationMethods<valueType>;
};
template <> struct PrimitiveInfo<PrimitiveDataType::UInt16>
{
    using valueType = quint16;
    using Class =     UInt16DataInformation;
    using Methods =   UIntDataInformationMethods<valueType>;
};
template <> struct PrimitiveInfo<PrimitiveDataType::UInt32>
{
    using valueType = quint32;
    using Class =     UInt32DataInformation;
    using Methods =   UIntDataInformationMethods<valueType>;
};
template <> struct PrimitiveInfo<PrimitiveDataType::UInt64>
{
    using valueType = quint64;
    using Class =     UInt64DataInformation;
    using Methods =   UIntDataInformationMethods<valueType>;
};

template <> struct PrimitiveInfo<PrimitiveDataType::Int8>
{
    using valueType = qint8;
    using Class =     SInt8DataInformation;
    using Methods =   SIntDataInformationMethods<valueType>;
};
template <> struct PrimitiveInfo<PrimitiveDataType::Int16>
{
    using valueType = qint16;
    using Class =     SInt16DataInformation;
    using Methods =   SIntDataInformationMethods<valueType>;
};
template <> struct PrimitiveInfo<PrimitiveDataType::Int32>
{
    using valueType = qint32;
    using Class =     SInt32DataInformation;
    using Methods =   SIntDataInformationMethods<valueType>;
};
template <> struct PrimitiveInfo<PrimitiveDataType::Int64>
{
    using valueType = qint64;
    using Class =     SInt64DataInformation;
    using Methods =   SIntDataInformationMethods<valueType>;
};

template <> struct PrimitiveInfo<PrimitiveDataType::Char>
{
    using valueType = quint8;
    using Class =     CharDataInformation;
    using Methods =   CharDataInformationMethods;
};
template <> struct PrimitiveInfo<PrimitiveDataType::Float>
{
    using valueType = float;
    using Class =     FloatDataInformation;
    using Methods =   FloatDataInformationMethods;
};
template <> struct PrimitiveInfo<PrimitiveDataType::Double>
{
    using valueType = double;
    using Class =     DoubleDataInformation;
    using Methods =   DoubleDataInformationMethods;
};

#endif // KASTEN_PRIMITIVETEMPLATEINFO_HPP
