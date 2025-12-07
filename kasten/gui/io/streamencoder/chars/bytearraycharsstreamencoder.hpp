/*
    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCHARSSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYCHARSSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QString>

class KConfigGroup;

namespace Kasten {

class CharsStreamEncoderSettings
{
public:
    enum class EncodeMode
    {
        Substitute = 0,
        EscapeCStyle = 1,
        _Count,
    };

    static constexpr auto EncodeModeCount = static_cast<std::size_t>(EncodeMode::_Count);
    static const std::array<QString, EncodeModeCount> encodeModeConfigValueList;

    /// Beware, not compatible with other enums
    enum class EscapedValueCoding
    {
        Hexadecimal = 0,
        Octal = 1,
        _Count,
    };

    static constexpr auto EscapedValueCodingCount = static_cast<std::size_t>(EscapedValueCoding::_Count);
    static const std::array<QString, EscapedValueCodingCount> escapedValueCodingConfigValueList;

private:
    static constexpr char EncodeModeConfigKey[] = "EncodeMode";
    static constexpr char EscapedValueCodingConfigKey[] = "EscapedValueCoding";

    static constexpr EncodeMode DefaultEncodeMode = EncodeMode::Substitute;
    static constexpr EscapedValueCoding DefaultEscapedValueCoding = EscapedValueCoding::Hexadecimal;

public:
    CharsStreamEncoderSettings();
    CharsStreamEncoderSettings(const CharsStreamEncoderSettings&) = default;
    CharsStreamEncoderSettings(CharsStreamEncoderSettings&&) = default;

    ~CharsStreamEncoderSettings() = default;

    CharsStreamEncoderSettings& operator=(const CharsStreamEncoderSettings&) = default;
    CharsStreamEncoderSettings& operator=(CharsStreamEncoderSettings&&) = default;

    [[nodiscard]]
    bool operator==(const CharsStreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    QString codecName;
    EncodeMode encodeMode = DefaultEncodeMode;
    EscapedValueCoding escapedValueCoding = DefaultEscapedValueCoding;
    QChar undefinedChar = {QLatin1Char('?')};
    QChar substituteChar = {QLatin1Char('.')};
};

// TODO: this could rather be one of the default cop
class ByteArrayCharsStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

private:
    static const QString ConfigGroupId;

public:
    ByteArrayCharsStreamEncoder();
    ~ByteArrayCharsStreamEncoder() override;

public:
    [[nodiscard]]
    CharsStreamEncoderSettings settings() const;
    void setSettings(const CharsStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
    [[nodiscard]]
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    CharsStreamEncoderSettings mSettings;
};

inline CharsStreamEncoderSettings ByteArrayCharsStreamEncoder::settings() const { return mSettings; }

}

#endif
