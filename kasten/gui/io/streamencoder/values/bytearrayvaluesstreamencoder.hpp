/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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
    static inline constexpr char SeparatorConfigKey[] = "Separator";

    static const QString DefaultSeparator;

public:
    ValuesStreamEncoderSettings();
    ValuesStreamEncoderSettings(const ValuesStreamEncoderSettings&) = default;

    ~ValuesStreamEncoderSettings() = default;

    ValuesStreamEncoderSettings& operator=(const ValuesStreamEncoderSettings&) = default;

    [[nodiscard]]
    bool operator==(const ValuesStreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    Okteta::ValueCoding valueCoding = Okteta::HexadecimalCoding;
    QString separation;
    QChar undefinedChar;
    QChar substituteChar;
};

class ByteArrayValuesStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "ByteArrayValuesStreamEncoder";

public:
    ByteArrayValuesStreamEncoder();
    ~ByteArrayValuesStreamEncoder() override;

public:
    [[nodiscard]]
    ValuesStreamEncoderSettings settings() const;
    void setSettings(const ValuesStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
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
