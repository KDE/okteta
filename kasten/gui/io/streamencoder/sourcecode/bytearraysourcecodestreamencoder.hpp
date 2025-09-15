/*
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

    static inline constexpr PrimitiveDataType DefaultDataType = PrimitiveDataType::UnsignedChar;
    static inline constexpr int DefaultElementsPerLine = 4;
    static inline constexpr bool DefaultUnsignedAsHexadecimal = true;

    static inline constexpr char VariableNameConfigKey[] = "VariableName";
    static inline constexpr char DataTypeConfigKey[] = "DataType";
    static inline constexpr char ElementsPerLineConfigKey[] = "ElementsPerLine";
    static inline constexpr char UnsignedAsHexadecimalConfigKey[] = "UnsignedAsHexadecimal";

public:
    SourceCodeStreamEncoderSettings();
    SourceCodeStreamEncoderSettings(const SourceCodeStreamEncoderSettings&) = default;
    SourceCodeStreamEncoderSettings(SourceCodeStreamEncoderSettings&&) = default;

    ~SourceCodeStreamEncoderSettings() = default;

    SourceCodeStreamEncoderSettings& operator=(const SourceCodeStreamEncoderSettings&) = default;
    SourceCodeStreamEncoderSettings& operator=(SourceCodeStreamEncoderSettings&&) = default;

    [[nodiscard]]
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
    static inline constexpr char ConfigGroupId[] = "ByteArraySourceCodeStreamEncoder";

public:
    ByteArraySourceCodeStreamEncoder();
    ~ByteArraySourceCodeStreamEncoder() override;

public:
    [[nodiscard]]
    SourceCodeStreamEncoderSettings settings() const;
    void setSettings(const SourceCodeStreamEncoderSettings& settings);

public:
    [[nodiscard]]
    const char* const* dataTypeNames() const;
    [[nodiscard]]
    int dataTypesCount() const;

protected: // AbstractByteArrayStreamEncoder API
    [[nodiscard]]
    bool encodeDataToStream(QIODevice* device,
                            const ByteArrayView* byteArrayView,
                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                            const Okteta::AddressRange& range) override;

private:
    [[nodiscard]]
    QString printFormatted(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset, unsigned int dataSize) const;

private:
    SourceCodeStreamEncoderSettings mSettings;
};

inline SourceCodeStreamEncoderSettings ByteArraySourceCodeStreamEncoder::settings() const { return mSettings; }

}

#endif
