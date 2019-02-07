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

#ifndef KASTEN_BYTEARRAYUUENCODINGSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYUUENCODINGSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QString>

namespace Kasten {

class UuencodingStreamEncoderSettings
{
public:
    enum class AlgorithmId
    {
        Historical = 0,
        Base64 = 1
    };

public:
    UuencodingStreamEncoderSettings();

public:
    QString fileName;
    AlgorithmId algorithmId = AlgorithmId::Base64;
};

class ByteArrayUuencodingStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    enum class InputByteIndex
    {
        First = 0,
        Second,
        Third
    };

public:
    ByteArrayUuencodingStreamEncoder();
    ~ByteArrayUuencodingStreamEncoder() override;

public:
    UuencodingStreamEncoderSettings settings() const;
    void setSettings(const UuencodingStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

protected:
    UuencodingStreamEncoderSettings mSettings;
};

inline UuencodingStreamEncoderSettings ByteArrayUuencodingStreamEncoder::settings() const { return mSettings; }
inline void ByteArrayUuencodingStreamEncoder::setSettings(const UuencodingStreamEncoderSettings& settings)
{
    mSettings = settings;
    emit settingsChanged();
}

}

#endif
