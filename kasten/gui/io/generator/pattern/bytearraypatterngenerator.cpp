/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraypatterngenerator.hpp"

// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QMimeData>
#include <QByteArray>
// Std
#include <algorithm>
#include <array>
#include <iterator>

static constexpr int codingCount =
    static_cast<int>(Kasten::ByteArrayPatternGeneratorSettings::_CodingCount);
static const std::array<QString, codingCount> codingConfigValueList = {
    QStringLiteral("Hexadecimal"),
    QStringLiteral("Decimal"),
    QStringLiteral("Octal"),
    QStringLiteral("Binary"),
    QStringLiteral("Char"),
    QStringLiteral("UTF-8"),
};


template <>
inline Kasten::ByteArrayPatternGeneratorSettings::Coding
KConfigGroup::readEntry(const char *key,
                        const Kasten::ByteArrayPatternGeneratorSettings::Coding &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    auto it = std::find(codingConfigValueList.cbegin(), codingConfigValueList.cend(), entry);
    if (it == codingConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(codingConfigValueList.cbegin(), it);
    return static_cast<Kasten::ByteArrayPatternGeneratorSettings::Coding>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Kasten::ByteArrayPatternGeneratorSettings::Coding &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    QString configValue;
    if (value == Kasten::ByteArrayPatternGeneratorSettings::InvalidCoding) {
        configValue = QStringLiteral("Invalid");
    } else {
        const int listIndex = static_cast<int>(value);
        configValue = codingConfigValueList[listIndex];
    }
    writeEntry(key, configValue, flags);
}

namespace Kasten {

static const QByteArray DefaultPattern = QByteArray(1, 0);
static constexpr ByteArrayPatternGeneratorSettings::Coding DefaultPatternCoding = ByteArrayPatternGeneratorSettings::HexadecimalCoding;
static constexpr int DefaultCount = 1;

static constexpr char ByteArrayPatternGeneratorConfigGroupId[] = "ByteArrayPatternGenerator";
static constexpr char PatternConfigKey[] = "Pattern";
static constexpr char PatternCodingConfigKey[] = "PatternCoding";
static constexpr char CountConfigKey[] = "Count";

ByteArrayPatternGeneratorSettings::ByteArrayPatternGeneratorSettings() = default;

bool ByteArrayPatternGeneratorSettings::operator==(const ByteArrayPatternGeneratorSettings& other) const
{
    return (pattern == other.pattern) && (patternCoding == other.patternCoding) && (count == other.count);
}

void ByteArrayPatternGeneratorSettings::loadConfig(const KConfigGroup& configGroup)
{
    pattern = configGroup.readEntry(PatternConfigKey, DefaultPattern);
    count = configGroup.readEntry(CountConfigKey, DefaultCount);
    patternCoding = configGroup.readEntry(PatternCodingConfigKey, DefaultPatternCoding);
}

void ByteArrayPatternGeneratorSettings::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(PatternConfigKey, pattern);
    configGroup.writeEntry(PatternCodingConfigKey, patternCoding);
    configGroup.writeEntry(CountConfigKey, count);
}

// TODO: support insert to selection, cmp. fill in painting program
// there are two kinds of generated datam fixed size (e.g. sequence) and endless size?
// perhaps by option fill selection? or a separate menu entry fill, which only works on selections?

ByteArrayPatternGenerator::ByteArrayPatternGenerator()
    : AbstractModelDataGenerator(
        i18nc("name of the generated data", "Pattern..."),
        QStringLiteral("application/octet-stream"),
        DynamicGeneration)
{
    const KConfigGroup configGroup(KSharedConfig::openConfig(), ByteArrayPatternGeneratorConfigGroupId);

    mSettings.loadConfig(configGroup);
}

ByteArrayPatternGenerator::~ByteArrayPatternGenerator() = default;

void ByteArrayPatternGenerator::setSettings(const ByteArrayPatternGeneratorSettings& settings)
{
    if (mSettings == settings) {
        return;
    }

    mSettings = settings;

    KConfigGroup configGroup(KSharedConfig::openConfig(), ByteArrayPatternGeneratorConfigGroupId);
    mSettings.saveConfig(configGroup);
//     emit settingsChanged();
}

// TODO: optimize and check if pattern is just one byte, so memset can be used
// TODO: see if copying larger chunks with memcpy is faster, so
QMimeData* ByteArrayPatternGenerator::generateData()
{
    const int patternSize = mSettings.pattern.size();

    const int insertDataSize = mSettings.count * patternSize;

    QByteArray insertData(insertDataSize, '\0');

    char* rawInsertData = insertData.data();
    const char* rawPatternData = mSettings.pattern.constData();

    for (int i = 0; i < insertDataSize; i += patternSize) {
        memcpy(&rawInsertData[i], rawPatternData, patternSize);
    }

    auto* mimeData = new QMimeData;
    mimeData->setData(mimeType(), insertData);

// TODO: a method to get the description of the change, e.g.
#if 0
    Okteta::ChangesDescribable* changesDescribable =
        qobject_cast<Okteta::ChangesDescribable*>(mByteArrayModel);

    if (changesDescribable) {
        changesDescribable->openGroupedChange(i18n("Pattern inserted."));
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
