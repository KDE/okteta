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

namespace Kasten {

class ValuesStreamEncoderSettings
{
public:
    ValuesStreamEncoderSettings();

public:
    Okteta::ValueCoding valueCoding = Okteta::HexadecimalCoding;
    QString separation;
    QChar undefinedChar;
    QChar substituteChar;
};

class ByteArrayValuesStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    ByteArrayValuesStreamEncoder();
    ~ByteArrayValuesStreamEncoder() override;

public:
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
inline void ByteArrayValuesStreamEncoder::setSettings(const ValuesStreamEncoderSettings& settings)
{
    mSettings = settings;
    emit settingsChanged();
}

}

#endif
