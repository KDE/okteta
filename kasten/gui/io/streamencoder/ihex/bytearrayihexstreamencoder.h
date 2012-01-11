/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYIHEXSTREAMENCODER_H
#define BYTEARRAYIHEXSTREAMENCODER_H

// lib
#include "abstractbytearraystreamencoder.h"
// Okteta core
#include <oktetacore.h>
// Qt
#include <QtCore/QString>

class QTextStream;


namespace Kasten2
{

class IHexStreamEncoderSettings
{
  public:
    enum AddressSizeId { Bits32Id = 0, Bits16Id = 1, Bits8Id = 2 };

  public:
    IHexStreamEncoderSettings();
  public:
    AddressSizeId addressSizeId;
};


class ByteArrayIHexStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

  private:
    enum IHexRecordType
    {
        DataRecord=0,
        EndOfFileRecord=1,
        ExtendedSegmentAddressRecord=2,
        StartSegmentAddressRecord=3,
        ExtendedLinearAddressRecord=4,
        StartLinearAddressRecord=5
    };

    static const char startCode = ':';

    static const int byteCountLineOffset = 0;
    static const int byteCountLineSize = 1;
    static const int addressLineOffset = byteCountLineOffset + byteCountLineSize;
    static const int addressLineSize = 2;
    static const int recordTypeLineOffset = addressLineOffset + addressLineSize;
    static const int recordTypeLineSize = 1;
    static const int dataLineOffset = recordTypeLineOffset + recordTypeLineSize;
    static const char hexDigits[16];
  private:
    static char hexValueOfNibble( int nibble );
    static void writeBigEndian( unsigned char* line, quint32 value, int byteSize );

    static void streamLine( QTextStream& textStream, const unsigned char* line );
    static void streamExtendedSegmentAddress( QTextStream& textStream, unsigned char* line,
                                              quint16 upperSegmentBaseAddress );
    static void streamExtendedLinearAddress( QTextStream& textStream, unsigned char* line,
                                             quint16 upperLinearBaseAddress );
    static void streamEndOfFile( QTextStream& textStream, unsigned char* line,
                                 quint16 startAddress = 0 );

  public:
    ByteArrayIHexStreamEncoder();
    virtual ~ByteArrayIHexStreamEncoder();

  public:
    IHexStreamEncoderSettings settings() const;
    void setSettings( const IHexStreamEncoderSettings& settings );

  protected: // AbstractByteArrayStreamEncoder API
    virtual bool encodeDataToStream( QIODevice* device,
                                     const ByteArrayView* byteArrayView,
                                     const Okteta::AbstractByteArrayModel* byteArrayModel,
                                     const Okteta::AddressRange& range );

  protected:
    IHexStreamEncoderSettings mSettings;
};


inline IHexStreamEncoderSettings ByteArrayIHexStreamEncoder::settings() const { return mSettings; }
inline void ByteArrayIHexStreamEncoder::setSettings( const IHexStreamEncoderSettings& settings )
{
    mSettings = settings;
    emit settingsChanged();
}

inline char ByteArrayIHexStreamEncoder::hexValueOfNibble( int nibble )
{ return hexDigits[nibble & 0xF]; }

inline void ByteArrayIHexStreamEncoder::writeBigEndian( unsigned char* line, quint32 value, int byteSize )
{
    while( byteSize > 0 )
    {
        --byteSize;
        line[byteSize] = value;
        value >>= 8;
    }
}

}

#endif
