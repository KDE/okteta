/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef PRIMITIVETEMPLATEINFO_H
#define PRIMITIVETEMPLATEINFO_H

#include "../primitivedatatype.h"
#include "booldatainformation.h"
#include "floatdatainformation.h"
#include "doubledatainformation.h"
#include "uintdatainformation.h"
#include "sintdatainformation.h"
#include "chardatainformation.h"
#include "basicprimitivedatainformation.h"

typedef BasicPrimitiveDataInformation<quint8, UIntDataInformationMethods<quint8> > UInt8DataInformation;
typedef BasicPrimitiveDataInformation<quint16, UIntDataInformationMethods<quint16> > UInt16DataInformation;
typedef BasicPrimitiveDataInformation<quint32, UIntDataInformationMethods<quint32> > UInt32DataInformation;
typedef BasicPrimitiveDataInformation<quint64, UIntDataInformationMethods<quint64> > UInt64DataInformation;

typedef BasicPrimitiveDataInformation<qint8, SIntDataInformationMethods<qint8> > SInt8DataInformation;
typedef BasicPrimitiveDataInformation<qint16, SIntDataInformationMethods<qint16> > SInt16DataInformation;
typedef BasicPrimitiveDataInformation<qint32, SIntDataInformationMethods<qint32> > SInt32DataInformation;
typedef BasicPrimitiveDataInformation<qint64, SIntDataInformationMethods<qint64> > SInt64DataInformation;

typedef BasicPrimitiveDataInformation<quint8, BoolDataInformationMethods<quint8> > Bool8DataInformation;
typedef BasicPrimitiveDataInformation<quint16, BoolDataInformationMethods<quint16> > Bool16DataInformation;
typedef BasicPrimitiveDataInformation<quint32, BoolDataInformationMethods<quint32> > Bool32DataInformation;
typedef BasicPrimitiveDataInformation<quint64, BoolDataInformationMethods<quint64> > Bool64DataInformation;

typedef BasicPrimitiveDataInformation<quint8, CharDataInformationMethods> CharDataInformation;
typedef BasicPrimitiveDataInformation<float, FloatDataInformationMethods> FloatDataInformation;
typedef BasicPrimitiveDataInformation<double, DoubleDataInformationMethods> DoubleDataInformation;


//just to simplify usage of those classes (no need to specify two template parameters)
template<PrimitiveDataTypeEnum T> struct PrimitiveInfo {
    typedef void valueType;
    typedef void Class; //to cause compiler errors with e.g. Type_NotPrimitive
    typedef void Methods; //to cause compiler errors with e.g. Type_NotPrimitive
};

template<> struct PrimitiveInfo<Type_Bool8> {
    typedef quint8 valueType;
    typedef Bool8DataInformation Class;
    typedef BoolDataInformationMethods<valueType> Methods;
};
template<> struct PrimitiveInfo<Type_Bool16> {
    typedef quint16 valueType;
    typedef Bool16DataInformation Class;
    typedef BoolDataInformationMethods<valueType> Methods;
};
template<> struct PrimitiveInfo<Type_Bool32> {
    typedef quint32 valueType;
    typedef Bool32DataInformation Class;
    typedef BoolDataInformationMethods<valueType> Methods;
};
template<> struct PrimitiveInfo<Type_Bool64> {
    typedef quint64 valueType;
    typedef Bool64DataInformation Class;
    typedef BoolDataInformationMethods<valueType> Methods;
};


template<> struct PrimitiveInfo<Type_UInt8> {
    typedef quint8 valueType;
    typedef UInt8DataInformation Class;
    typedef UIntDataInformationMethods<valueType> Methods;
};
template<> struct PrimitiveInfo<Type_UInt16> {
    typedef quint16 valueType;
    typedef UInt16DataInformation Class;
    typedef UIntDataInformationMethods<valueType> Methods;
};
template<> struct PrimitiveInfo<Type_UInt32> {
    typedef quint32 valueType;
    typedef UInt32DataInformation Class;
    typedef UIntDataInformationMethods<valueType> Methods;
};
template<> struct PrimitiveInfo<Type_UInt64> {
    typedef quint64 valueType;
    typedef UInt64DataInformation Class;
    typedef UIntDataInformationMethods<valueType> Methods;
};


template<> struct PrimitiveInfo<Type_Int8> {
    typedef qint8 valueType;
    typedef SInt8DataInformation Class;
    typedef SIntDataInformationMethods<valueType> Methods;
};
template<> struct PrimitiveInfo<Type_Int16> {
    typedef qint16 valueType;
    typedef SInt16DataInformation Class;
    typedef SIntDataInformationMethods<valueType> Methods;
};
template<> struct PrimitiveInfo<Type_Int32> {
    typedef qint32 valueType;
    typedef SInt32DataInformation Class;
    typedef SIntDataInformationMethods<valueType> Methods;
};
template<> struct PrimitiveInfo<Type_Int64> {
    typedef qint64 valueType;
    typedef SInt64DataInformation Class;
    typedef SIntDataInformationMethods<valueType> Methods;
};


template<> struct PrimitiveInfo<Type_Char> {
    typedef quint8 valueType;
    typedef CharDataInformation Class;
    typedef CharDataInformationMethods Methods;
};
template<> struct PrimitiveInfo<Type_Float> {
    typedef float valueType;
    typedef FloatDataInformation Class;
    typedef FloatDataInformationMethods Methods;
};
template<> struct PrimitiveInfo<Type_Double> {
    typedef double valueType;
    typedef DoubleDataInformation Class;
    typedef DoubleDataInformationMethods Methods;
};

#endif //PRIMITIVETEMPLATEINFO_H
