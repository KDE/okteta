/*
    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycharsstreamencoder.hpp"

// lib
#include <bytearrayview.hpp>
// liboktetawidgets
// TODO: private class, can only use due to being in oktetakastengui, make public in core
#include <bytearraychar8stringencoder.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/Character>
#include <Okteta/CharCodec>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QTextStream>
// Std
#include <algorithm>
#include <iterator>

const std::array<QString, Kasten::CharsStreamEncoderSettings::EncodeModeCount>
Kasten::CharsStreamEncoderSettings::encodeModeConfigValueList = {
    QStringLiteral("Substitute"),
    QStringLiteral("EscapeCStyle"),
};

template <>
inline Kasten::CharsStreamEncoderSettings::EncodeMode KConfigGroup::readEntry(const char *key,
                                                                              const Kasten::CharsStreamEncoderSettings::EncodeMode &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    const auto it = std::find(Kasten::CharsStreamEncoderSettings::encodeModeConfigValueList.cbegin(),
                              Kasten::CharsStreamEncoderSettings::encodeModeConfigValueList.cend(),
                              entry);
    if (it == Kasten::CharsStreamEncoderSettings::encodeModeConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(Kasten::CharsStreamEncoderSettings::encodeModeConfigValueList.cbegin(), it);
    return static_cast<Kasten::CharsStreamEncoderSettings::EncodeMode>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::CharsStreamEncoderSettings::EncodeMode &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const int listIndex = static_cast<int>(value);
    writeEntry(key, Kasten::CharsStreamEncoderSettings::encodeModeConfigValueList[listIndex], flags);
}

const std::array<QString, Kasten::CharsStreamEncoderSettings::EscapedValueCodingCount>
Kasten::CharsStreamEncoderSettings::escapedValueCodingConfigValueList = {
    QStringLiteral("Hexadecimal"),
    QStringLiteral("Octal"),
};

template <>
inline Kasten::CharsStreamEncoderSettings::EscapedValueCoding KConfigGroup::readEntry(const char *key,
                                                                                      const Kasten::CharsStreamEncoderSettings::EscapedValueCoding &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    const auto it = std::find(Kasten::CharsStreamEncoderSettings::escapedValueCodingConfigValueList.cbegin(),
                              Kasten::CharsStreamEncoderSettings::escapedValueCodingConfigValueList.cend(),
                              entry);
    if (it == Kasten::CharsStreamEncoderSettings::escapedValueCodingConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(Kasten::CharsStreamEncoderSettings::escapedValueCodingConfigValueList.cbegin(), it);
    return static_cast<Kasten::CharsStreamEncoderSettings::EscapedValueCoding>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::CharsStreamEncoderSettings::EscapedValueCoding &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const int listIndex = static_cast<int>(value);
    writeEntry(key, Kasten::CharsStreamEncoderSettings::escapedValueCodingConfigValueList[listIndex], flags);
}

namespace Kasten {

CharsStreamEncoderSettings::CharsStreamEncoderSettings() = default;

bool CharsStreamEncoderSettings::operator==(const CharsStreamEncoderSettings& other) const
{
    return
        (escapedValueCoding == other.escapedValueCoding) &&
        (encodeMode == other.encodeMode);
}

void CharsStreamEncoderSettings::loadConfig(const KConfigGroup& configGroup)
{
    escapedValueCoding = configGroup.readEntry(EscapedValueCodingConfigKey, DefaultEscapedValueCoding);
    encodeMode = configGroup.readEntry(EncodeModeConfigKey, DefaultEncodeMode);
}

void CharsStreamEncoderSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(EscapedValueCodingConfigKey, escapedValueCoding);
    configGroup.writeEntry(EncodeModeConfigKey, encodeMode);
}

ByteArrayCharsStreamEncoder::ByteArrayCharsStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Characters"), QStringLiteral("text/plain"))
{}

ByteArrayCharsStreamEncoder::~ByteArrayCharsStreamEncoder() = default;

void ByteArrayCharsStreamEncoder::setSettings(const CharsStreamEncoderSettings& settings)
{
    if (mSettings == settings) {
        return;
    }

    mSettings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    mSettings.saveConfig(configGroup);
    Q_EMIT settingsChanged();
}

inline
Okteta::ByteArrayChar8StringEncoder::ValueCoding toStringEncoderValueCoding(CharsStreamEncoderSettings::EscapedValueCoding settingsCoding)
{
    return static_cast<Okteta::ByteArrayChar8StringEncoder::ValueCoding>(settingsCoding);
}

bool ByteArrayCharsStreamEncoder::encodeDataToStream(QIODevice* device,
                                                     const ByteArrayView* byteArrayView,
                                                     const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                     const Okteta::AddressRange& range)
{
    bool success = true;

    // settings
    mSettings.codecName = byteArrayView->charCodingName();
    mSettings.undefinedChar = byteArrayView->undefinedChar();
    mSettings.substituteChar = byteArrayView->substituteChar();

    // encode
    QTextStream textStream(device);

    const auto charCodec = Okteta::CharCodec::createCodec(mSettings.codecName);

    if (mSettings.encodeMode == CharsStreamEncoderSettings::EncodeMode::Substitute) {
        const QChar tabChar = QLatin1Char('\t');
        const QChar returnChar = QLatin1Char('\n');

        for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
            const Okteta::Byte byte = byteArrayModel->byte(i);
            const Okteta::Character byteChar = charCodec->decode(byte);

            const QChar streamChar = byteChar.isUndefined() ?      mSettings.undefinedChar :
                                    (!byteChar.isPrint()
                                    || byteChar == tabChar
                                    || byteChar == returnChar) ? mSettings.substituteChar :
                                                                static_cast<QChar>(byteChar);
            textStream << streamChar;
        }
    } else  {
        const Okteta::ByteArrayChar8StringEncoder stringEncoder;
        const Okteta::ByteArrayChar8StringEncoder::ValueCoding escapeValueCoding = toStringEncoderValueCoding(mSettings.escapedValueCoding);
        for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
            const Okteta::Byte byte = byteArrayModel->byte(i);

            const QString string = stringEncoder.encodeAsString(byte, charCodec.get(), escapeValueCoding);

            textStream << string;
        }
    }
    return success;
}

}

#include "moc_bytearraycharsstreamencoder.cpp"
