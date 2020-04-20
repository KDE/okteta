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

#ifndef KASTEN_BYTEARRAYSOURCECODESTREAMENCODER_HPP
#define KASTEN_BYTEARRAYSOURCECODESTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Qt
#include <QString>

namespace Kasten {

class SourceCodeStreamEncoderSettings
{
public:
    enum class PrimitiveDataType
    {
        Char = 0,
        UnsignedChar,
        Short,
        UnsignedShort,
        Integer,
        UnsignedInteger,
        Float,
        Double,
    };

public:
    SourceCodeStreamEncoderSettings();

public:
    QString variableName;
    PrimitiveDataType dataType = PrimitiveDataType::UnsignedChar;
    int elementsPerLine = 4;
    bool unsignedAsHexadecimal = true;
};

// TODO: General synchronizer would load matching encoder and decoder
// manually defined by desktopfile
class ByteArraySourceCodeStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    ByteArraySourceCodeStreamEncoder();
    ~ByteArraySourceCodeStreamEncoder() override;

public:
    SourceCodeStreamEncoderSettings settings() const;
    void setSettings(const SourceCodeStreamEncoderSettings& settings);

public:
    const char* const* dataTypeNames() const;
    int dataTypesCount() const;

protected: // AbstractByteArrayStreamEncoder API
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    QString printFormatted(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset, unsigned int dataSize) const;

private:
    SourceCodeStreamEncoderSettings mSettings;
};

inline SourceCodeStreamEncoderSettings ByteArraySourceCodeStreamEncoder::settings() const { return mSettings; }
inline void ByteArraySourceCodeStreamEncoder::setSettings(const SourceCodeStreamEncoderSettings& settings)
{
    mSettings = settings;
    Q_EMIT settingsChanged();
}

}

#endif
