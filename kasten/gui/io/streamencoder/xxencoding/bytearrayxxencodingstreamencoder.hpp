/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYXXENCODINGSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYXXENCODINGSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Qt
#include <QString>

class KConfigGroup;

namespace Kasten {

class XxencodingStreamEncoderSettings
{
private:
    static const QString DefaultFileName;

    static constexpr char FileNameConfigKey[] = "FileName";

public:
    XxencodingStreamEncoderSettings();
    XxencodingStreamEncoderSettings(const XxencodingStreamEncoderSettings&) = default;
    XxencodingStreamEncoderSettings(XxencodingStreamEncoderSettings&&) = default;

    ~XxencodingStreamEncoderSettings() = default;

    XxencodingStreamEncoderSettings& operator=(const XxencodingStreamEncoderSettings&) = default;
    XxencodingStreamEncoderSettings& operator=(XxencodingStreamEncoderSettings&&) = default;

    [[nodiscard]]
    bool operator==(const XxencodingStreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    QString fileName = DefaultFileName;
};

class ByteArrayXxencodingStreamEncoder : public AbstractByteArrayStreamEncoder
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
    static constexpr int defaultInputLineLength = 45;
    static constexpr int inputLineLength = defaultInputLineLength;
    static constexpr int inputGroupLength = 3;
    static constexpr int maxInputGroupsPerLine = inputLineLength / inputGroupLength;

    static const QString ConfigGroupId;

public:
    ByteArrayXxencodingStreamEncoder();
    ~ByteArrayXxencodingStreamEncoder() override;

public:
    [[nodiscard]]
    XxencodingStreamEncoderSettings settings() const;
    void setSettings(const XxencodingStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
    [[nodiscard]]
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    XxencodingStreamEncoderSettings mSettings;
};

inline XxencodingStreamEncoderSettings ByteArrayXxencodingStreamEncoder::settings() const { return mSettings; }

}

#endif
