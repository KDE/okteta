/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

static const QString DefaultSeparator = QStringLiteral(" ");

static constexpr char ByteArrayValuesStreamEncoderConfigGroupId[] = "ByteArrayValuesStreamEncoder";

static constexpr char SeparatorConfigKey[] = "Separator";

ValuesStreamEncoderSettings::ValuesStreamEncoderSettings() = default;

bool ValuesStreamEncoderSettings::operator==(const ValuesStreamEncoderSettings& other) const
{
    // the only user-edited value
    return (separation == other.separation);
}

void ValuesStreamEncoderSettings::loadConfig(const KConfigGroup& configGroup)
{
    separation = configGroup.readEntry(SeparatorConfigKey, DefaultSeparator);
}

void ValuesStreamEncoderSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(SeparatorConfigKey, separation);
}


ByteArrayValuesStreamEncoder::ByteArrayValuesStreamEncoder()
    : AbstractByteArrayStreamEncoder(i18nc("name of the encoding target", "Values"), QStringLiteral("text/plain"))
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ByteArrayValuesStreamEncoderConfigGroupId);
    mSettings.loadConfig(configGroup);
}

ByteArrayValuesStreamEncoder::~ByteArrayValuesStreamEncoder() = default;

void ByteArrayValuesStreamEncoder::setSettings(const ValuesStreamEncoderSettings& settings)
{
    if (mSettings == settings) {
        return;
    }

    mSettings = settings;
    KConfigGroup configGroup(KSharedConfig::openConfig(), ByteArrayValuesStreamEncoderConfigGroupId);
    mSettings.saveConfig(configGroup);
    emit settingsChanged();
}

bool ByteArrayValuesStreamEncoder::encodeDataToStream(QIODevice* device,
                                                      const ByteArrayView* byteArrayView,
                                                      const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                      const Okteta::AddressRange& range)
{
    bool success = true;

    // settings
    mSettings.undefinedChar = byteArrayView->undefinedChar();
    mSettings.substituteChar = byteArrayView->substituteChar();
    mSettings.valueCoding = (Okteta::ValueCoding)byteArrayView->valueCoding();

    // encode
    QTextStream textStream(device);

    Okteta::ValueCodec* valueCodec = Okteta::ValueCodec::createCodec(mSettings.valueCoding);

    // prepare
    QString valueString;
    valueString.resize(valueCodec->encodingWidth());

    for (Okteta::Address i = range.start(); i <= range.end(); ++i) {
        if (i > range.start()) {
            textStream << mSettings.separation;
        }

        valueCodec->encode(&valueString, 0, byteArrayModel->byte(i));

        textStream << valueString;
    }

    // clean up
    delete valueCodec;

    return success;
}

}
