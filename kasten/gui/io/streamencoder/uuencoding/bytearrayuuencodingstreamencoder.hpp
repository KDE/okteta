/*
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
    static inline constexpr char FileNameConfigKey[] = "FileName";
    static inline constexpr char EncodingTypeConfigKey[] = "EncodingType";
    static inline constexpr EncodingType DefaultEncodingType = EncodingType::Base64;
    static const QString DefaultFileName;

public:
    UuencodingStreamEncoderSettings();
    UuencodingStreamEncoderSettings(const UuencodingStreamEncoderSettings&) = default;
    UuencodingStreamEncoderSettings(UuencodingStreamEncoderSettings&&) = default;

    ~UuencodingStreamEncoderSettings() = default;

    UuencodingStreamEncoderSettings& operator=(const UuencodingStreamEncoderSettings&) = default;
    UuencodingStreamEncoderSettings& operator=(UuencodingStreamEncoderSettings&&) = default;

    [[nodiscard]]
    bool operator==(const UuencodingStreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    QString fileName = DefaultFileName;
    EncodingType encodingType = DefaultEncodingType;
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
    static inline constexpr int defaultInputLineLength = 45;
    static inline constexpr int inputLineLength = defaultInputLineLength;
    static inline constexpr int inputGroupLength = 3;
    static inline constexpr int maxInputGroupsPerLine = inputLineLength / inputGroupLength;

    static inline constexpr char ConfigGroupId[] = "ByteArrayUuencodingStreamEncoder";

public:
    ByteArrayUuencodingStreamEncoder();
    ~ByteArrayUuencodingStreamEncoder() override;

public:
    [[nodiscard]]
    UuencodingStreamEncoderSettings settings() const;
    void setSettings(const UuencodingStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
    [[nodiscard]]
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
