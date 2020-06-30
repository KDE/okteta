/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYCHARSSTREAMENCODER_HPP
#define KASTEN_BYTEARRAYCHARSSTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Qt
#include <QString>

namespace Kasten {

class CharsStreamEncoderSettings
{
public:
    CharsStreamEncoderSettings();

public:
    QString codecName;
    QChar undefinedChar = {QLatin1Char('?')};
    QChar substituteChar = {QLatin1Char('.')};
};

// TODO: this could rather be one of the default cop
class ByteArrayCharsStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    ByteArrayCharsStreamEncoder();
    ~ByteArrayCharsStreamEncoder() override;

protected: // AbstractByteArrayStreamEncoder API
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    CharsStreamEncoderSettings mSettings;
};

}

#endif
