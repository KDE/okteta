/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYRANDOMDATAGENERATOR_HPP
#define KASTEN_BYTEARRAYRANDOMDATAGENERATOR_HPP

// Kasten core
#include <Kasten/AbstractModelDataGenerator>
// Qt
#include <QObject>
#include <QString>

class KConfigGroup;

namespace Kasten {

class ByteArrayRandomDataGeneratorSettings
{
public:
    ByteArrayRandomDataGeneratorSettings();

    bool operator==(const ByteArrayRandomDataGeneratorSettings& other) const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

public:
    int size = 256;
};

class ByteArrayRandomDataGenerator : public AbstractModelDataGenerator
{
    Q_OBJECT

public:
    ByteArrayRandomDataGenerator();

    ~ByteArrayRandomDataGenerator() override;

public: // AbstractModelDataGenerator API
    QMimeData* generateData() override;

public:
    ByteArrayRandomDataGeneratorSettings settings() const;
    void setSettings(const ByteArrayRandomDataGeneratorSettings& settings);

private:
    ByteArrayRandomDataGeneratorSettings mSettings;
};

inline ByteArrayRandomDataGeneratorSettings ByteArrayRandomDataGenerator::settings() const { return mSettings; }

}

#endif
