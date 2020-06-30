/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PODDATA_HPP
#define KASTEN_PODDATA_HPP

// Okteta core
#include <Okteta/Byte>
#include <Okteta/OktetaCore>
// Qt
#include <QSysInfo>

namespace Okteta {

class PODData
{
public:
    static constexpr int Size = sizeof(double);

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

private:
    // ensure strict alignment for double as needed on some architectures (e.g. PA-RISC)
    using Aligned64Bit = union
    {
        double mDummy;
        Byte mBytes[Size];
    };

private:
    Byte* mCurrentOriginalData = nullptr;
    Byte* mCurrentEndiannessSetData = nullptr;

    Aligned64Bit mOriginalAligned64Bit;
    Aligned64Bit mByteOrderSetAligned64Bit;

    int mCurrentSize = 0;
    QSysInfo::Endian mByteOrder = QSysInfo::ByteOrder;
};

}

#endif
