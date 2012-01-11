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

#ifndef BYTEARRAYSRECSTREAMENCODER_H
#define BYTEARRAYSRECSTREAMENCODER_H

// lib
#include "abstractbytearraystreamencoder.h"
// Okteta core
#include <oktetacore.h>
// Qt
#include <QtCore/QString>

class QTextStream;


namespace Kasten2
{

class SRecStreamEncoderSettings
{
  public:
    enum AddressSizeId { FourBytesId = 0, ThreeBytesId = 1, TwoBytesId = 2 };

  public:
    SRecStreamEncoderSettings();
  public:
    AddressSizeId addressSizeId;
};


class ByteArraySRecStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

  private:
    enum RecordType
    {
        BlockHeader=0,
        DataSequence2B=1,
        DataSequence3B=2,
        DataSequence4B=3,
        RecordCount=5,
        EndOfBlock4B=7,
        EndOfBlock3B=8,
        EndOfBlock2B=9
    };

    static const char startCode = 'S';

    static const int byteCountLineOffset = 0;
    static const int byteCountLineSize = 1;
    static const int addressLineOffset = byteCountLineOffset + byteCountLineSize;

    static const char hexDigits[16];

  private:
    static char charOfRecordType( RecordType type );
    static char hexValueOfNibble( int nibble );
    static void writeBigEndian( unsigned char* line, quint32 value, int byteSize );
    static void streamLine( QTextStream& textStream, RecordType recordType,
                            const unsigned char* line );
    static void streamBlockHeader( QTextStream& textStream, unsigned char* line );
    static void streamRecordCount( QTextStream& textStream, unsigned char* line,
                                    quint16 recordCount );
    static void streamBlockEnd( QTextStream& textStream, unsigned char* line,
                                RecordType recordType, quint32 startAddress = 0 ); // TODO: make address

  public:
    ByteArraySRecStreamEncoder();
    virtual ~ByteArraySRecStreamEncoder();

  public:
    SRecStreamEncoderSettings settings() const;
    void setSettings( const SRecStreamEncoderSettings& settings );

  protected: // AbstractByteArrayStreamEncoder API
    virtual bool encodeDataToStream( QIODevice* device,
                                     const ByteArrayView* byteArrayView,
                                     const Okteta::AbstractByteArrayModel* byteArrayModel,
                                     const Okteta::AddressRange& range );

  protected:
    SRecStreamEncoderSettings mSettings;
};


inline SRecStreamEncoderSettings ByteArraySRecStreamEncoder::settings() const { return mSettings; }
inline void ByteArraySRecStreamEncoder::setSettings( const SRecStreamEncoderSettings& settings )
{
    mSettings = settings;
    emit settingsChanged();
}

inline char ByteArraySRecStreamEncoder::charOfRecordType( RecordType type )
{ return (char)('0'+type); }
inline char ByteArraySRecStreamEncoder::hexValueOfNibble( int nibble )
{ return hexDigits[nibble & 0xF]; }

inline void ByteArraySRecStreamEncoder::writeBigEndian( unsigned char* line,
                                                        quint32 value, int byteSize )
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
