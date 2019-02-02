/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PODDATA_HPP
#define PODDATA_HPP

// Okteta core
#include <okteta/byte.hpp>
#include <okteta/oktetacore.hpp>
#include <QSysInfo>

namespace Okteta {

class PODData
{
public:
    static const int Size = sizeof(double);

public:
    PODData();

public:
    void setByteOrder(QSysInfo::Endian byteOrder);
    bool updateRawData(int size);
    Byte* rawData();

public:
    const Byte* originalData() const;
    const Byte* byteOrderSetData() const;
    QSysInfo::Endian byteOrder() const;

    unsigned long bitValue(int noOfBitsToRead) const;
    void getPointers(const void** P8Bit, const void** P16Bit, const void** P32Bit, const void** P64Bit) const;
    const void* pointer(int byteCount) const;
    int size() const;

protected:
    // ensure strict alignment for double as needed on some architectures (e.g. PA-RISC)
    using Aligned64Bit = union
    {
        double mDummy;
        Byte mBytes[Size];
    };

protected:
    Byte* mCurrentOriginalData = nullptr;
    Byte* mCurrentEndiannessSetData = nullptr;

    Aligned64Bit mOriginalAligned64Bit;
    Aligned64Bit mByteOrderSetAligned64Bit;

    int mCurrentSize = 0;
    QSysInfo::Endian mByteOrder = QSysInfo::ByteOrder;
};

}

#endif
