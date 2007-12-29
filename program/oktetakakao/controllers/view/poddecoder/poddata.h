/***************************************************************************
                          poddata.h  -  description
                             -------------------
    begin                : Fri Dec 28 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef PODDATA_H
#define PODDATA_H

// Qt
#include <Qt>

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
