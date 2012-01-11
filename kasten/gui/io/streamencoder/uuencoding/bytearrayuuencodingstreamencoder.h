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

#ifndef BYTEARRAYUUENCODINGSTREAMENCODER_H
#define BYTEARRAYUUENCODINGSTREAMENCODER_H

// lib
#include "abstractbytearraystreamencoder.h"
// Okteta core
#include <oktetacore.h>
// Qt
#include <QtCore/QString>


namespace Kasten2
{

class UuencodingStreamEncoderSettings
{
  public:
    enum AlgorithmId { HistoricalId = 0, Base64Id = 1 };

  public:
    UuencodingStreamEncoderSettings();
  public:
    QString fileName;
    AlgorithmId algorithmId;
};


class ByteArrayUuencodingStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

  public:
    enum InputByteIndex { FirstByte, SecondByte, ThirdByte };

  public:
    ByteArrayUuencodingStreamEncoder();
    virtual ~ByteArrayUuencodingStreamEncoder();

  public:
    UuencodingStreamEncoderSettings settings() const;
    void setSettings( const UuencodingStreamEncoderSettings& settings );

  protected: // AbstractByteArrayStreamEncoder API
    virtual bool encodeDataToStream( QIODevice* device,
                                     const ByteArrayView* byteArrayView,
                                     const Okteta::AbstractByteArrayModel* byteArrayModel,
                                     const Okteta::AddressRange& range );

  protected:
    UuencodingStreamEncoderSettings mSettings;
};


inline UuencodingStreamEncoderSettings ByteArrayUuencodingStreamEncoder::settings() const { return mSettings; }
inline void ByteArrayUuencodingStreamEncoder::setSettings( const UuencodingStreamEncoderSettings& settings )
{
    mSettings = settings;
    emit settingsChanged();
}

}

#endif
