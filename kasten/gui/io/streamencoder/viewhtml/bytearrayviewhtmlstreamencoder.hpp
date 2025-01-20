/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVIEWHTMLSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYVIEWHTMLSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QString>

namespace Kasten {

class ByteArrayViewHtmlStreamEncoderSettings
{
public:
    ByteArrayViewHtmlStreamEncoderSettings();
    ByteArrayViewHtmlStreamEncoderSettings(const ByteArrayViewHtmlStreamEncoderSettings&) = default;

    ~ByteArrayViewHtmlStreamEncoderSettings() = default;

    ByteArrayViewHtmlStreamEncoderSettings& operator=(const ByteArrayViewHtmlStreamEncoderSettings&) = default;

public:
    int codingWidth;
    int firstLineOffset;
    int startOffset;
    int delta;
    Okteta::ValueCoding valueCoding = Okteta::HexadecimalCoding;
    QString codecName;
    QChar undefinedChar = {QLatin1Char('?')};
    QChar substituteChar = {QLatin1Char('.')};
};

// TODO: this could rather be one of the default cop
class ByteArrayViewHtmlStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    ByteArrayViewHtmlStreamEncoder();
    ~ByteArrayViewHtmlStreamEncoder() override;

protected: // AbstractByteArrayStreamEncoder API
    [[nodiscard]]
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    ByteArrayViewHtmlStreamEncoderSettings mSettings;
};

}

#endif
