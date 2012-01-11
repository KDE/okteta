/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef BYTEARRAYSOURCECODESTREAMENCODER_H
#define BYTEARRAYSOURCECODESTREAMENCODER_H

// lib
#include "abstractbytearraystreamencoder.h"
// Qt
#include <QtCore/QString>


namespace Kasten2
{

class SourceCodeStreamEncoderSettings
{
  public:
    enum PrimitiveDataType
    {
        CharType = 0,
        UnsignedCharType,
        ShortType,
        UnsignedShortType,
        IntegerType,
        UnsignedIntegerType,
        FloatType,
        DoubleType
    };

  public:
    SourceCodeStreamEncoderSettings();

  public:
    QString variableName;
    int  dataType;
    int elementsPerLine;
    bool unsignedAsHexadecimal;
};


// TODO: General synchronizer would load matching encoder and decoder
// manually defined by desktopfile
class ByteArraySourceCodeStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

  public:
    ByteArraySourceCodeStreamEncoder();
    virtual ~ByteArraySourceCodeStreamEncoder();

  public:
    SourceCodeStreamEncoderSettings settings() const;
    void setSettings( const SourceCodeStreamEncoderSettings& settings );

  public:
    const char* const* dataTypeNames() const;
    int dataTypesCount() const;

  protected: // AbstractByteArrayStreamEncoder API
    virtual bool encodeDataToStream( QIODevice* device,
                                     const ByteArrayView* byteArrayView,
                                     const Okteta::AbstractByteArrayModel* byteArrayModel,
                                     const Okteta::AddressRange& range );

  protected:
    QString printFormatted( const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset, unsigned int dataSize ) const;

  protected:
    SourceCodeStreamEncoderSettings mSettings;
};


inline SourceCodeStreamEncoderSettings ByteArraySourceCodeStreamEncoder::settings() const { return mSettings; }
inline void ByteArraySourceCodeStreamEncoder::setSettings( const SourceCodeStreamEncoderSettings& settings )
{
    mSettings = settings;
    emit settingsChanged();
}

}

#endif
