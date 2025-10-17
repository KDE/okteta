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

class KConfigGroup;

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
        LongInteger,
        UnsignedLongInteger,
        Float,
        Double,
        _Count,
    };

private:
    static const QString DefaultVariableName;

    static constexpr PrimitiveDataType DefaultDataType = PrimitiveDataType::UnsignedChar;
    static constexpr int DefaultElementsPerLine = 4;
    static constexpr bool DefaultUnsignedAsHexadecimal = true;

    static constexpr char VariableNameConfigKey[] = "VariableName";
    static constexpr char DataTypeConfigKey[] = "DataType";
    static constexpr char ElementsPerLineConfigKey[] = "ElementsPerLine";
    static constexpr char UnsignedAsHexadecimalConfigKey[] = "UnsignedAsHexadecimal";

public:
    SourceCodeStreamEncoderSettings();
    SourceCodeStreamEncoderSettings(const SourceCodeStreamEncoderSettings&) = default;

    ~SourceCodeStreamEncoderSettings() = default;

    SourceCodeStreamEncoderSettings& operator=(const SourceCodeStreamEncoderSettings&) = default;

    bool operator==(const SourceCodeStreamEncoderSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    QString variableName = DefaultVariableName;
    PrimitiveDataType dataType = DefaultDataType;
    int elementsPerLine = DefaultElementsPerLine;
    bool unsignedAsHexadecimal = DefaultUnsignedAsHexadecimal;
};

// TODO: General synchronizer would load matching encoder and decoder
// manually defined by desktopfile
class ByteArraySourceCodeStreamEncoder : public AbstractByteArrayStreamEncoder
{
    Q_OBJECT

private:
    static constexpr char ConfigGroupId[] = "ByteArraySourceCodeStreamEncoder";

public:
    ByteArraySourceCodeStreamEncoder();
    ~ByteArraySourceCodeStreamEncoder() override;

public:
    SourceCodeStreamEncoderSettings settings() const;
    void setSettings(const SourceCodeStreamEncoderSettings& settings);

public:
    const QString* dataTypeNames() const;
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

}

#endif
