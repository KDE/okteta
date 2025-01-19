/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraysourcecodestreamencoder.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QTextStream>
// Std
#include <array>
#include <algorithm>
#include <iterator>

static constexpr int primitiveDataTypeCount =
    static_cast<int>(Kasten::SourceCodeStreamEncoderSettings::PrimitiveDataType::_Count);
static const std::array<QString, primitiveDataTypeCount> primitiveDataTypeConfigValueList = {
    QStringLiteral("Char"),
    QStringLiteral("UnsignedChar"),
    QStringLiteral("Short"),
    QStringLiteral("UnsignedShort"),
    QStringLiteral("Integer"),
    QStringLiteral("UnsignedInteger"),
    QStringLiteral("Long"),
    QStringLiteral("UnsignedLong"),
    QStringLiteral("Float"),
    QStringLiteral("Double"),
};


template <>
inline Kasten::SourceCodeStreamEncoderSettings::PrimitiveDataType KConfigGroup::readEntry(const char *key,
                                                                                   const Kasten::SourceCodeStreamEncoderSettings::PrimitiveDataType &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    auto it = std::find(primitiveDataTypeConfigValueList.cbegin(), primitiveDataTypeConfigValueList.cend(), entry);
    if (it == primitiveDataTypeConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(primitiveDataTypeConfigValueList.cbegin(), it);
    return static_cast<Kasten::SourceCodeStreamEncoderSettings::PrimitiveDataType>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::SourceCodeStreamEncoderSettings::PrimitiveDataType &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const int listIndex = static_cast<int>(value);
    writeEntry(key, primitiveDataTypeConfigValueList[listIndex], flags);
}

namespace Kasten {

const QString SourceCodeStreamEncoderSettings::DefaultVariableName = QStringLiteral("array");

static constexpr int NoOfPrimitiveDataTypes = 10;

static constexpr  const char* PrimitiveDataTypeName[NoOfPrimitiveDataTypes] = {
    "char",
    "unsigned char",
    "int16_t",
    "uint16_t",
    "int32_t",
    "uint32_t",
    "int64_t",
    "uint64_t",
    "float",
    "double"
};

static constexpr int SizeOfPrimitiveDataType[NoOfPrimitiveDataTypes] =
{
    sizeof(char),
    sizeof(unsigned char),
    sizeof(int16_t),
    sizeof(uint16_t),
    sizeof(int32_t),
    sizeof(uint32_t),
    sizeof(int64_t),
    sizeof(uint64_t),
    sizeof(float),
    sizeof(double)
};

inline QString decimalFormattedNumberPlaceHolder() { return QStringLiteral("%1"); }
inline QString hexadecimalFormattedNumberPlaceHolder() { return QStringLiteral("0x%1"); }

SourceCodeStreamEncoderSettings::SourceCodeStreamEncoderSettings() = default;

bool SourceCodeStreamEncoderSettings::operator==(const SourceCodeStreamEncoderSettings& other) const
{
    return
        (variableName == other.variableName) &&
        (dataType == other.dataType) &&
        (elementsPerLine == other.elementsPerLine) &&
        (unsignedAsHexadecimal == other.unsignedAsHexadecimal);
}

void SourceCodeStreamEncoderSettings::loadConfig(const KConfigGroup& configGroup)
{
    variableName = configGroup.readEntry(VariableNameConfigKey, DefaultVariableName);
    dataType = configGroup.readEntry(DataTypeConfigKey, DefaultDataType);
    elementsPerLine = configGroup.readEntry(ElementsPerLineConfigKey, DefaultElementsPerLine);
    unsignedAsHexadecimal = configGroup.readEntry(UnsignedAsHexadecimalConfigKey, DefaultUnsignedAsHexadecimal);
}

void SourceCodeStreamEncoderSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(VariableNameConfigKey, variableName);
    configGroup.writeEntry(DataTypeConfigKey, dataType);
    configGroup.writeEntry(ElementsPerLineConfigKey, elementsPerLine);
    configGroup.writeEntry(UnsignedAsHexadecimalConfigKey, unsignedAsHexadecimal);
}

ByteArraySourceCodeStreamEncoder::ByteArraySourceCodeStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "C Array"), QStringLiteral("text/x-csrc"))
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    mSettings.loadConfig(configGroup);
}

ByteArraySourceCodeStreamEncoder::~ByteArraySourceCodeStreamEncoder() = default;

const char* const* ByteArraySourceCodeStreamEncoder::dataTypeNames() const { return PrimitiveDataTypeName; }
int ByteArraySourceCodeStreamEncoder::dataTypesCount() const { return NoOfPrimitiveDataTypes; }

void ByteArraySourceCodeStreamEncoder::setSettings(const SourceCodeStreamEncoderSettings& settings)
{
    if (mSettings == settings) {
        return;
    }

    mSettings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    mSettings.saveConfig(configGroup);
    Q_EMIT settingsChanged();
}

bool ByteArraySourceCodeStreamEncoder::encodeDataToStream(QIODevice* device,
                                                          const ByteArrayView* byteArrayView,
                                                          const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                          const Okteta::AddressRange& range)
{
    Q_UNUSED(byteArrayView)

    bool success = true;

    // encode
    QTextStream textStream(device);

//     textStream << "// from File , selection \n";

    const int size = range.width();
    const int dataTypeSize = SizeOfPrimitiveDataType[static_cast<int>(mSettings.dataType)];
    const int sizeOfArray = (size + dataTypeSize - 1) / dataTypeSize;

    textStream << "const " << QLatin1String(PrimitiveDataTypeName[static_cast<int>(mSettings.dataType)]) << ' '
               << mSettings.variableName << '[' << sizeOfArray << "] =" << Qt::endl
               << '{' << Qt::endl;

    int elementAddedOnLine = 0;
    for (Okteta::Address i = range.start(); i <= range.end(); i += dataTypeSize) {
        if (elementAddedOnLine == 0) {
            textStream << "   "; // just 3, one space before every datum
        }
        textStream << ' ' << printFormatted(byteArrayModel, i, range.end() - i + 1);
        if (i + dataTypeSize <= range.end()) {
            textStream << ',';
        }

        if (++elementAddedOnLine >= mSettings.elementsPerLine) {
            textStream << Qt::endl;
            elementAddedOnLine = 0;
        }
    }

    if (elementAddedOnLine > 0) {
        textStream << Qt::endl;
    }

    textStream << "};" << Qt::endl;

    return success;
}

QString ByteArraySourceCodeStreamEncoder::printFormatted(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                                         unsigned int dataSize) const
{
    QString result;
    switch (mSettings.dataType)
    {
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Char:
    {
        char e = 0;
        std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin<size_t>(sizeof(e), dataSize));
        constexpr int fieldWidth = 4;
        result = decimalFormattedNumberPlaceHolder().arg((int)e, fieldWidth);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::UnsignedChar:
    {
        unsigned char e = 0;
        std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 2 : 3;
        const QString formattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
                                                   hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
        const QChar stuffChar = QLatin1Char(mSettings.unsignedAsHexadecimal ? '0' : ' ');
        result = formattedNumberPlaceHolder.arg(e, fieldWidth, base, stuffChar);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Short:
    {
        int16_t e = 0;
        std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        constexpr int fieldWidth = 6;
        result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::UnsignedShort:
    {
        uint16_t e = 0;
        std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 4 : 5;
        const QString formattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
                                                   hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
        const QChar stuffChar = QLatin1Char(mSettings.unsignedAsHexadecimal ? '0' : ' ');
        result = formattedNumberPlaceHolder.arg(e, fieldWidth, base, stuffChar);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Integer:
    {
        int32_t e = 0;
        std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        constexpr int fieldWidth = 11;
        result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::UnsignedInteger:
    {
        uint32_t e = 0;
        std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 8 : 10;
        const QString formattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
                                                   hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
        const QChar stuffChar = QLatin1Char(mSettings.unsignedAsHexadecimal ? '0' : ' ');
        result = formattedNumberPlaceHolder.arg(e, fieldWidth, base, stuffChar);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::LongInteger:
    {
        int64_t e = 0;
        std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        constexpr int fieldWidth = 20;
        result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::UnsignedLongInteger:
    {
        uint64_t e = 0;
        std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 16 : 20;
        const QString formattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
                                                   hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
        const QChar stuffChar = QLatin1Char(mSettings.unsignedAsHexadecimal ? '0' : ' ');
        result = formattedNumberPlaceHolder.arg(e, fieldWidth, base, stuffChar);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Float:
    {
        float e = 0;
        std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        constexpr int fieldWidth = 13;
        result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Double:
    {
        double e = 0;
        std::ignore = byteArrayModel->copyTo(reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)), dataSize));
        constexpr int fieldWidth = 13;
        result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::_Count:
        // dummy entry to avoid compiler warning -Wswitch, can this be avoided?
        break;
    }

    return result;
}

}

#include "moc_bytearraysourcecodestreamencoder.cpp"
