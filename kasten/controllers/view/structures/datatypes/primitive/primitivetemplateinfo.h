/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011  Alex Richardson <alex.richardson@gmx.de>
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

#include "../../allprimitivetypes.h"
#include "booldatainformation.h"
#include "floatdatainformation.h"
#include "doubledatainformation.h"
#include "uintdatainformation.h"
#include "sintdatainformation.h"
#include "chardatainformation.h"

//just to simplify usage of those classes (no need to specify two template parameters)
template<PrimitiveDataType T> struct PrimitiveInfo {
    typedef void Class; //to cause compiler errors with e.g. Type_NotPrimitive
};

template<> struct PrimitiveInfo<Type_Bool8> {
    typedef BoolDataInformation<quint8, Type_Bool8> Class;
};
template<> struct PrimitiveInfo<Type_Bool16> {
    typedef BoolDataInformation<quint16, Type_Bool16> Class;
};
template<> struct PrimitiveInfo<Type_Bool32> {
    typedef BoolDataInformation<quint32, Type_Bool32> Class;
};
template<> struct PrimitiveInfo<Type_Bool64> {
    typedef BoolDataInformation<quint64, Type_Bool64> Class;
};


template<> struct PrimitiveInfo<Type_UInt8> {
    typedef UIntDataInformation<quint8, Type_UInt8> Class;
};
template<> struct PrimitiveInfo<Type_UInt16> {
    typedef UIntDataInformation<quint16, Type_UInt16> Class;
};
template<> struct PrimitiveInfo<Type_UInt32> {
    typedef UIntDataInformation<quint32, Type_UInt32> Class;
};
template<> struct PrimitiveInfo<Type_UInt64> {
    typedef UIntDataInformation<quint64, Type_UInt64> Class;
};


template<> struct PrimitiveInfo<Type_Int8> {
    typedef SIntDataInformation<qint8, Type_Int8> Class;
};
template<> struct PrimitiveInfo<Type_Int16> {
    typedef SIntDataInformation<qint16, Type_Int16> Class;
};
template<> struct PrimitiveInfo<Type_Int32> {
    typedef SIntDataInformation<qint32, Type_Int32> Class;
};
template<> struct PrimitiveInfo<Type_Int64> {
    typedef SIntDataInformation<qint64, Type_Int64> Class;
};


template<> struct PrimitiveInfo<Type_Char> {
    typedef CharDataInformation Class;
};
template<> struct PrimitiveInfo<Type_Float> {
    typedef FloatDataInformation Class;
};
template<> struct PrimitiveInfo<Type_Double> {
    typedef DoubleDataInformation Class;
};


#endif //PRIMITIVETEMPLATEINFO_H
