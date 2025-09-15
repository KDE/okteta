/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYPATTERNGENERATOR_HPP
#define KASTEN_BYTEARRAYPATTERNGENERATOR_HPP

// Kasten core
#include <Kasten/AbstractModelDataGenerator>

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

private:
    static const QByteArray DefaultPattern;

    static inline constexpr Coding DefaultPatternCoding = HexadecimalCoding;
    static inline constexpr int DefaultCount = 1;

    static inline constexpr char PatternConfigKey[] = "Pattern";
    static inline constexpr char PatternCodingConfigKey[] = "PatternCoding";
    static inline constexpr char CountConfigKey[] = "Count";

public:
    ByteArrayPatternGeneratorSettings();
    ByteArrayPatternGeneratorSettings(const ByteArrayPatternGeneratorSettings&) = default;
    ByteArrayPatternGeneratorSettings(ByteArrayPatternGeneratorSettings&&) = default;

    ~ByteArrayPatternGeneratorSettings() = default;

    ByteArrayPatternGeneratorSettings& operator=(const ByteArrayPatternGeneratorSettings&) = default;
    ByteArrayPatternGeneratorSettings& operator=(ByteArrayPatternGeneratorSettings&&) = default;

    [[nodiscard]]
    bool operator==(const ByteArrayPatternGeneratorSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    QByteArray pattern = DefaultPattern;
    Coding patternCoding = DefaultPatternCoding;
    int count = DefaultCount;
};

class ByteArrayPatternGenerator : public AbstractModelDataGenerator
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "ByteArrayPatternGenerator";

public:
    ByteArrayPatternGenerator();

    ~ByteArrayPatternGenerator() override;

public: // AbstractModelDataGenerator API
    [[nodiscard]]
    std::unique_ptr<QMimeData> generateData() override;

public:
    [[nodiscard]]
    ByteArrayPatternGeneratorSettings settings() const;
    void setSettings(const ByteArrayPatternGeneratorSettings& settings);

private:
    ByteArrayPatternGeneratorSettings mSettings;
};

inline ByteArrayPatternGeneratorSettings ByteArrayPatternGenerator::settings() const { return mSettings; }

}

#endif
