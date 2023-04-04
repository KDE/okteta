/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYBASE32STREAMENCODER_HPP
#define KASTEN_BYTEARRAYBASE32STREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QString>

class KConfigGroup;

namespace Kasten {

class Base32StreamEncoderSettings
{
public:
    enum class EncodingType
    {
        Classic = 0,
        Hex = 1,
        ZHex = 2,
        _Count,
    };

public:
    Base32StreamEncoderSettings();
    Base32StreamEncoderSettings(const Base32StreamEncoderSettings&) = default;

    ~Base32StreamEncoderSettings() = default;

    Base32StreamEncoderSettings& operator=(const Base32StreamEncoderSettings&) = default;

    bool operator==(const Base32StreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    EncodingType encodingType = EncodingType::Classic;
};

class ByteArrayBase32StreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    static constexpr int inputGroupLength = 5;

    static constexpr int outputLineLength = 76;
    static constexpr int outputGroupLength = 8;
    static constexpr int maxOutputGroupsPerLine = outputLineLength / outputGroupLength;

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

private:
    Base32StreamEncoderSettings mSettings;
};

inline Base32StreamEncoderSettings ByteArrayBase32StreamEncoder::settings() const { return mSettings; }

}

#endif
