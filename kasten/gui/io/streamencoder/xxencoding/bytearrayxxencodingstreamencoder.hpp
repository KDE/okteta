/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYXXENCODINGSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYXXENCODINGSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta core
#include <Okteta/OktetaCore>
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

    ~XxencodingStreamEncoderSettings() = default;

    XxencodingStreamEncoderSettings& operator=(const XxencodingStreamEncoderSettings&) = default;

    bool operator==(const XxencodingStreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    QString fileName;
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
    static constexpr int defaultxxInputLineLength = 45;
    static constexpr int xxInputLineLength = defaultxxInputLineLength;
    static constexpr int xxInputGroupLength = 3;
    static constexpr int maxXxInputGroupsPerLine = xxInputLineLength / xxInputGroupLength;

    static constexpr char ConfigGroupId[] = "ByteArrayXxencodingStreamEncoder";

public:
    ByteArrayXxencodingStreamEncoder();
    ~ByteArrayXxencodingStreamEncoder() override;

public:
    XxencodingStreamEncoderSettings settings() const;
    void setSettings(const XxencodingStreamEncoderSettings& settings);

protected: // AbstractByteArrayStreamEncoder API
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
