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

#ifndef BYTEARRAYBASE32STREAMENCODER_H
#define BYTEARRAYBASE32STREAMENCODER_H

// lib
#include "abstractbytearraystreamencoder.h"
// Okteta core
#include <okteta/oktetacore.h>
// Qt
#include <QString>

namespace Kasten {

class Base32StreamEncoderSettings
{
public:
    enum class AlgorithmId
    {
        Classic = 0,
        Hex = 1,
        ZHex = 2
    };

public:
    Base32StreamEncoderSettings();

public:
    AlgorithmId algorithmId;
};

class ByteArrayBase32StreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    static const int inputGroupLength = 5;

    static const int outputLineLength = 76;
    static const int outputGroupLength = 8;
    static const int maxOutputGroupsPerLine = outputLineLength / outputGroupLength;

    enum class InputByteIndex
    {
        First = 0,
        Second,
        Third,
        Fourth,
        Fifth
    };

public:
    ByteArrayBase32StreamEncoder();
    ~ByteArrayBase32StreamEncoder() override;

public:
    Base32StreamEncoderSettings settings() const;
    void setSettings(const Base32StreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

protected:
    Base32StreamEncoderSettings mSettings;
};

inline Base32StreamEncoderSettings ByteArrayBase32StreamEncoder::settings() const { return mSettings; }
inline void ByteArrayBase32StreamEncoder::setSettings(const Base32StreamEncoderSettings& settings)
{
    mSettings = settings;
    emit settingsChanged();
}

}

#endif
