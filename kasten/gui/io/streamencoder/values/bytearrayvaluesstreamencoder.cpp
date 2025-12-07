/*
    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayvaluesstreamencoder.hpp"

// lib
#include <bytearrayview.hpp>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ValueCodec>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QTextStream>

namespace Kasten {

const QString ValuesStreamEncoderSettings::DefaultPrefix = QString();
const QString ValuesStreamEncoderSettings::DefaultPostfix = QString();
const QString ValuesStreamEncoderSettings::DefaultSeparator = QStringLiteral(" ");

ValuesStreamEncoderSettings::ValuesStreamEncoderSettings() = default;

bool ValuesStreamEncoderSettings::operator==(const ValuesStreamEncoderSettings& other) const
{
    // the only user-edited value
    return
        (prefix == other.prefix) &&
        (postfix == other.postfix) &&
        (separation == other.separation);
}

void ValuesStreamEncoderSettings::loadConfig(const KConfigGroup& configGroup)
{
    prefix = configGroup.readEntry(PrefixConfigKey, DefaultPrefix);
    postfix = configGroup.readEntry(PostfixConfigKey, DefaultPostfix);
    separation = configGroup.readEntry(SeparatorConfigKey, DefaultSeparator);
}

void ValuesStreamEncoderSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(PrefixConfigKey, prefix);
    configGroup.writeEntry(PostfixConfigKey, postfix);
    configGroup.writeEntry(SeparatorConfigKey, separation);
}

const QString ByteArrayValuesStreamEncoder::ConfigGroupId = QStringLiteral("ByteArrayValuesStreamEncoder");

ByteArrayValuesStreamEncoder::ByteArrayValuesStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Values"), QStringLiteral("text/plain"))
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    mSettings.loadConfig(configGroup);
}

ByteArrayValuesStreamEncoder::~ByteArrayValuesStreamEncoder() = default;

void ByteArrayValuesStreamEncoder::setSettings(const ValuesStreamEncoderSettings& settings)
{
    if (mSettings == settings) {
        return;
    }

    mSettings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    mSettings.saveConfig(configGroup);
    Q_EMIT settingsChanged();
}

bool ByteArrayValuesStreamEncoder::encodeDataToStream(QIODevice* device,
                                                      const ByteArrayView* byteArrayView,
                                                      const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                      const Okteta::AddressRange& range)
{
    bool success = true;

    // settings
    mSettings.valueCoding = (Okteta::ValueCoding)byteArrayView->valueCoding();

    // encode
    QTextStream textStream(device);

    const std::unique_ptr<const Okteta::ValueCodec> valueCodec(Okteta::ValueCodec::createCodec(mSettings.valueCoding));

    // prepare
    QString valueString;
    valueString.resize(valueCodec->encodingWidth());

    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        if (i > range.start()) {
            textStream << mSettings.separation;
        }

        valueCodec->encode(&valueString, 0, byteArrayModel->byte(i));

        textStream << mSettings.prefix << valueString << mSettings.postfix;
    }

    return success;
}

}

#include "moc_bytearrayvaluesstreamencoder.cpp"
