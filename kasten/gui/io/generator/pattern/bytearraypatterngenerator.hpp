/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYPATTERNGENERATOR_HPP
#define KASTEN_BYTEARRAYPATTERNGENERATOR_HPP

// Kasten core
#include <Kasten/AbstractModelDataGenerator>
// Qt
#include <QObject>
#include <QString>

class KConfigGroup;

namespace Kasten {

class ByteArrayPatternGeneratorSettings
{
public:
    // matching Okteta::ByteArrayComboBox
    enum Coding
    {
        InvalidCoding = -1,
        HexadecimalCoding = 0,
        DecimalCoding = 1,
        OctalCoding = 2,
        BinaryCoding = 3,
        CharCoding = 4,
        Utf8Coding = 5,
        _CodingCount
    };

public:
    ByteArrayPatternGeneratorSettings();
    ByteArrayPatternGeneratorSettings(const ByteArrayPatternGeneratorSettings&) = default;

    ~ByteArrayPatternGeneratorSettings() = default;

    ByteArrayPatternGeneratorSettings& operator=(const ByteArrayPatternGeneratorSettings&) = default;

    bool operator==(const ByteArrayPatternGeneratorSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    QByteArray pattern = QByteArray(1, 0);
    Coding patternCoding = HexadecimalCoding;
    int count = 1;
};

class ByteArrayPatternGenerator : public AbstractModelDataGenerator
{
    Q_OBJECT

public:
    ByteArrayPatternGenerator();

    ~ByteArrayPatternGenerator() override;

public: // AbstractModelDataGenerator API
    QMimeData* generateData() override;

public:
    ByteArrayPatternGeneratorSettings settings() const;
    void setSettings(const ByteArrayPatternGeneratorSettings& settings);

private:
    ByteArrayPatternGeneratorSettings mSettings;
};

inline ByteArrayPatternGeneratorSettings ByteArrayPatternGenerator::settings() const { return mSettings; }

}

#endif
