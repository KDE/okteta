/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYUUENCODINGSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYUUENCODINGSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Qt
#include <QString>

class KConfigGroup;

namespace Kasten {

class UuencodingStreamEncoderSettings
{
public:
    enum class EncodingType
    {
        Historical = 0,
        Base64 = 1
    };

private:
    static constexpr char FileNameConfigKey[] = "FileName";
    static constexpr char EncodingTypeConfigKey[] = "EncodingType";
    static constexpr EncodingType DefaultEncodingType = EncodingType::Base64;
    static const QString DefaultFileName;

public:
    UuencodingStreamEncoderSettings();
    UuencodingStreamEncoderSettings(const UuencodingStreamEncoderSettings&) = default;

    ~UuencodingStreamEncoderSettings() = default;

    UuencodingStreamEncoderSettings& operator=(const UuencodingStreamEncoderSettings&) = default;

    bool operator==(const UuencodingStreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    QString fileName;
    EncodingType encodingType = EncodingType::Base64;
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

private:
    static constexpr int defaultUuInputLineLength = 45;
    static constexpr int uuInputLineLength = defaultUuInputLineLength;
    static constexpr int uuInputGroupLength = 3;
    static constexpr int maxInputGroupsPerLine = uuInputLineLength / uuInputGroupLength;

    static constexpr char ConfigGroupId[] = "ByteArrayUuencodingStreamEncoder";

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

private:
    UuencodingStreamEncoderSettings mSettings;
};

inline UuencodingStreamEncoderSettings ByteArrayUuencodingStreamEncoder::settings() const { return mSettings; }

}

#endif
