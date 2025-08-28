/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRANDOMDATAGENERATOR_HPP
#define KASTEN_BYTEARRAYRANDOMDATAGENERATOR_HPP

// Kasten core
#include <Kasten/AbstractModelDataGenerator>

class KConfigGroup;

namespace Kasten {

class ByteArrayRandomDataGeneratorSettings
{
private:
    static inline constexpr int DefaultSize = 256;

    static inline constexpr char SizeConfigKey[] = "Size";

public:
    ByteArrayRandomDataGeneratorSettings();
    ByteArrayRandomDataGeneratorSettings(const ByteArrayRandomDataGeneratorSettings&) = default;
    ByteArrayRandomDataGeneratorSettings(ByteArrayRandomDataGeneratorSettings&&) = default;

    ~ByteArrayRandomDataGeneratorSettings() = default;

    ByteArrayRandomDataGeneratorSettings& operator=(const ByteArrayRandomDataGeneratorSettings&) = default;
    ByteArrayRandomDataGeneratorSettings& operator=(ByteArrayRandomDataGeneratorSettings&&) = default;

    [[nodiscard]]
    bool operator==(const ByteArrayRandomDataGeneratorSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    int size = DefaultSize;
};

class ByteArrayRandomDataGenerator : public AbstractModelDataGenerator
{
    Q_OBJECT

private:
    static inline constexpr char ConfigGroupId[] = "ByteArrayRandomDataGenerator";

public:
    ByteArrayRandomDataGenerator();

    ~ByteArrayRandomDataGenerator() override;

public: // AbstractModelDataGenerator API
    [[nodiscard]]
    std::unique_ptr<QMimeData> generateData() override;

public:
    [[nodiscard]]
    ByteArrayRandomDataGeneratorSettings settings() const;
    void setSettings(const ByteArrayRandomDataGeneratorSettings& settings);

private:
    ByteArrayRandomDataGeneratorSettings mSettings;
};

inline ByteArrayRandomDataGeneratorSettings ByteArrayRandomDataGenerator::settings() const { return mSettings; }

}

#endif
