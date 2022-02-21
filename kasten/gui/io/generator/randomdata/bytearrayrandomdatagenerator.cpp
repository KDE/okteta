/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrandomdatagenerator.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QRandomGenerator>
#include <QMimeData>
#include <QByteArray>

namespace Kasten {

static constexpr int DefaultSize = 256;

static constexpr char ByteArrayRandomDataGeneratorConfigGroupId[] = "ByteArrayRandomDataGenerator";
static constexpr char SizeConfigKey[] = "Size";

ByteArrayRandomDataGeneratorSettings::ByteArrayRandomDataGeneratorSettings() = default;

bool ByteArrayRandomDataGeneratorSettings::operator==(const ByteArrayRandomDataGeneratorSettings& other) const
{
    return (size == other.size);
}

void ByteArrayRandomDataGeneratorSettings::loadConfig(const KConfigGroup& configGroup)
{
    size = configGroup.readEntry(SizeConfigKey, DefaultSize);
}

void ByteArrayRandomDataGeneratorSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(SizeConfigKey, size);
}

// TODO: support insert to selection, cmp. fill in painting program
// there are two kinds of generated datam fixed size (e.g. sequence) and endless size?
// perhaps by option fill selection? or a separate menu entry fill, which only works on selections?

ByteArrayRandomDataGenerator::ByteArrayRandomDataGenerator()
    : AbstractModelDataGenerator(
        i18nc("name of the generated data", "Random Data..."),
        QStringLiteral("application/octet-stream"),
        DynamicGeneration)
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ByteArrayRandomDataGeneratorConfigGroupId);

    mSettings.loadConfig(configGroup);
}

ByteArrayRandomDataGenerator::~ByteArrayRandomDataGenerator() = default;

void ByteArrayRandomDataGenerator::setSettings(const ByteArrayRandomDataGeneratorSettings& settings)
{
    if (mSettings == settings) {
        return;
    }

    mSettings = settings;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ByteArrayRandomDataGeneratorConfigGroupId);
    mSettings.saveConfig(configGroup);
//     Q_EMIT settingsChanged();
}

// TODO: use different RNG, with multiple characteristics and offer them in the config
QMimeData* ByteArrayRandomDataGenerator::generateData()
{
    const int insertDataSize = mSettings.size;
    QByteArray insertData(insertDataSize, '\0');

    auto* randomGenerator = QRandomGenerator::global();
    for (int i = 0; i < insertDataSize; ++i) {
        insertData[i] = static_cast<char>(randomGenerator->bounded(256));
    }

    auto* mimeData = new QMimeData;
    mimeData->setData(mimeType(), insertData);

// TODO: a method to get the description of the change, e.g.
#if 0
    Okteta::ChangesDescribable* changesDescribable =
        qobject_cast<Okteta::ChangesDescribable*>(mByteArrayModel);

    if (changesDescribable) {
        changesDescribable->openGroupedChange(i18n("RandomData inserted."));
    }
    mByteArrayView->insert(insertData);
//     mByteArrayModel->replace( filteredSection, filterResult );
    if (changesDescribable) {
        changesDescribable->closeGroupedChange();
    }
#endif

    return mimeData;
}

}
