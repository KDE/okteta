/*
    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVALUESSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYVALUESSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QString>

class KConfigGroup;

namespace Kasten {

class ValuesStreamEncoderSettings
{
private:
    static constexpr char PrefixConfigKey[] = "Prefix";
    static constexpr char PostfixConfigKey[] = "Postfix";
    static constexpr char SeparatorConfigKey[] = "Separator";

    static const QString DefaultPrefix;
    static const QString DefaultPostfix;
    static const QString DefaultSeparator;
    static constexpr Okteta::ValueCoding DefaultValueCoding = Okteta::HexadecimalCoding;

public:
    ValuesStreamEncoderSettings();
    ValuesStreamEncoderSettings(const ValuesStreamEncoderSettings&) = default;
    ValuesStreamEncoderSettings(ValuesStreamEncoderSettings&&) = default;

    ~ValuesStreamEncoderSettings() = default;

    ValuesStreamEncoderSettings& operator=(const ValuesStreamEncoderSettings&) = default;
    ValuesStreamEncoderSettings& operator=(ValuesStreamEncoderSettings&&) = default;

    [[nodiscard]]
    bool operator==(const ValuesStreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    Okteta::ValueCoding valueCoding = DefaultValueCoding;
    QString prefix = DefaultPrefix;
    QString postfix = DefaultPrefix;
    QString separation = DefaultSeparator;
};

class ByteArrayValuesStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

private:
    static constexpr char ConfigGroupId[] = "ByteArrayValuesStreamEncoder";

public:
    ByteArrayValuesStreamEncoder();
    ~ByteArrayValuesStreamEncoder() override;

public:
    [[nodiscard]]
    ValuesStreamEncoderSettings settings() const;
    void setSettings(const ValuesStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
    [[nodiscard]]
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    ValuesStreamEncoderSettings mSettings;
};

inline ValuesStreamEncoderSettings ByteArrayValuesStreamEncoder::settings() const { return mSettings; }

}

#endif
