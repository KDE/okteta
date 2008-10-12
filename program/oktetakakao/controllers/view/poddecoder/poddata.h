/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef PODDATA_H
#define PODDATA_H

// Qt
#include <QtCore/Qt>

class PODData
{
  public:
    // TODO: add PDP endianess
    enum ByteOrder
    {
        LittleEndianOrder = 0, // Intel, Alpha, ...
        BigEndianOrder =    1 // Sun, Motorola, ...
    };
    static const ByteOrder ThisMachineEndianOrder =
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
    LittleEndianOrder;
#else
    BigEndianOrder;
#endif
    static const int Size = sizeof(double);

  public:
    PODData();

  public:
    void setByteOrder( int byteOrder );
    bool updateRawData( int size );
    unsigned char *rawData();

  public:
    const unsigned char *data() const;
    int byteOrder() const;
    unsigned long bitValue( int noOfBitsToRead ) const;
    void pointers( const void **P8Bit, const void **P16Bit, const void **P32Bit, const void **P64Bit ) const;
    void pointer( const void** P, int byteCount ) const;
    int size() const;

  protected:
    // ensure strict alignment for double as needed on some architectures (e.g. PA-RISC)
    typedef union { unsigned char Data[Size]; double Dummy; } Aligned64Bit;

  protected:
    unsigned char *mCurrentData;
    Aligned64Bit mAligned64Bit;
    int mCurrentSize;
    int mByteOrder;
};

#endif
