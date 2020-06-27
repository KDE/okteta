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

namespace Kasten {

class ByteArrayPatternGeneratorSettings
{
public:
    ByteArrayPatternGeneratorSettings();

public:
    QByteArray pattern = QByteArray(1, 0);
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
inline void ByteArrayPatternGenerator::setSettings(const ByteArrayPatternGeneratorSettings& settings)
{
    mSettings = settings;
//     emit settingsChanged();
}

}

#endif
