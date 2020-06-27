/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVIEWTEXTSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYVIEWTEXTSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Okteta gui
// #include <Okteta/OffsetFormat>
// Okteta core
#include <Okteta/OktetaCore>
// Qt
#include <QString>

namespace Kasten {

class ByteArrayViewTextStreamEncoderSettings
{
public:
    ByteArrayViewTextStreamEncoderSettings();

public:
//     Okteta::OffsetFormat::Format offsetFormat;
    int codingWidth;
    int firstLineOffset;
    int startOffset;
    int delta;
    Okteta::ValueCoding valueCoding = Okteta::HexadecimalCoding;
    QString codecName;
    QChar undefinedChar = {QLatin1Char('?')};
    QChar substituteChar = {QLatin1Char('.')};
    QString separation;
//     Okteta::CharCodec *CharCodec;
//     Okteta::OffsetFormat::print printFunction;
};

// TODO: this could rather be one of the default cop
class ByteArrayViewTextStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    ByteArrayViewTextStreamEncoder();
    ~ByteArrayViewTextStreamEncoder() override;

protected: // AbstractByteArrayStreamEncoder API
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    ByteArrayViewTextStreamEncoderSettings mSettings;
};

}

#endif
