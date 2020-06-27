/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSOURCECODESTREAMENCODER_HPP
#define KASTEN_BYTEARRAYSOURCECODESTREAMENCODER_HPP

// lib
#include "abstractbytearraystreamencoder.hpp"
// Qt
#include <QString>

namespace Kasten {

class SourceCodeStreamEncoderSettings
{
public:
    enum class PrimitiveDataType
    {
        Char = 0,
        UnsignedChar,
        Short,
        UnsignedShort,
        Integer,
        UnsignedInteger,
        Float,
        Double,
    };

public:
    SourceCodeStreamEncoderSettings();

public:
    QString variableName;
    PrimitiveDataType dataType = PrimitiveDataType::UnsignedChar;
    int elementsPerLine = 4;
    bool unsignedAsHexadecimal = true;
};

// TODO: General synchronizer would load matching encoder and decoder
// manually defined by desktopfile
class ByteArraySourceCodeStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

public:
    ByteArraySourceCodeStreamEncoder();
    ~ByteArraySourceCodeStreamEncoder() override;

public:
    SourceCodeStreamEncoderSettings settings() const;
    void setSettings(const SourceCodeStreamEncoderSettings& settings);

public:
    const char* const* dataTypeNames() const;
    int dataTypesCount() const;

protected: // AbstractByteArrayStreamEncoder API
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    QString printFormatted(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset, unsigned int dataSize) const;

private:
    SourceCodeStreamEncoderSettings mSettings;
};

inline SourceCodeStreamEncoderSettings ByteArraySourceCodeStreamEncoder::settings() const { return mSettings; }
inline void ByteArraySourceCodeStreamEncoder::setSettings(const SourceCodeStreamEncoderSettings& settings)
{
    mSettings = settings;
    emit settingsChanged();
}

}

#endif
