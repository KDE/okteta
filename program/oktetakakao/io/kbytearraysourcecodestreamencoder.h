/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#ifndef KBYTEARRAYSOURCECODESTREAMENCODER_H
#define KBYTEARRAYSOURCECODESTREAMENCODER_H

// lib
#include "kabstractbytearraystreamencoder.h"
// Qt
#include <QtCore/QString>

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
class KByteArraySourceCodeStreamEncoder : public KAbstractByteArrayStreamEncoder
{
    Q_OBJECT

  public:
    KByteArraySourceCodeStreamEncoder();
    virtual ~KByteArraySourceCodeStreamEncoder();

  public:
    SourceCodeStreamEncoderSettings settings() const;
    void setSettings( const SourceCodeStreamEncoderSettings& settings );

  public:
    const char** dataTypeNames() const;
    int dataTypesCount() const;

  Q_SIGNALS:
    void settingsChanged();

  protected: // KAbstractByteArrayStreamEncoder API
    virtual bool encodeDataToStream( QIODevice *device,
                                     const KByteArrayDisplay* byteArrayView,
                                     const KHECore::AbstractByteArrayModel *byteArrayModel,
                                     const KHE::Section &section );

  protected:
    void printFormatted( char *buffer, const KHECore::AbstractByteArrayModel *byteArrayModel, int pos, unsigned int dataSize ) const;

  protected:
    SourceCodeStreamEncoderSettings mSettings;
};


inline SourceCodeStreamEncoderSettings KByteArraySourceCodeStreamEncoder::settings() const { return mSettings; }
inline void KByteArraySourceCodeStreamEncoder::setSettings( const SourceCodeStreamEncoderSettings& settings )
{
    mSettings = settings;
    emit settingsChanged();
}

#endif
