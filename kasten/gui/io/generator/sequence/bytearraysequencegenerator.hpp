/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSEQUENCEGENERATOR_HPP
#define KASTEN_BYTEARRAYSEQUENCEGENERATOR_HPP

// Kasten core
#include <Kasten/AbstractModelDataGenerator>
// Qt
#include <QObject>
#include <QString>

namespace Kasten {

class ByteArraySequenceGeneratorSettings
{
public:
    ByteArraySequenceGeneratorSettings();

public:
    QByteArray pattern;
    int count = 1;
};

class ByteArraySequenceGenerator : public AbstractModelDataGenerator
{
    Q_OBJECT

public:
    ByteArraySequenceGenerator();

    ~ByteArraySequenceGenerator() override;

public: // AbstractModelDataGenerator API
    QMimeData* generateData() override;

public:
    ByteArraySequenceGeneratorSettings settings() const;
    void setSettings(const ByteArraySequenceGeneratorSettings& settings);

private:
    ByteArraySequenceGeneratorSettings mSettings;
};

inline ByteArraySequenceGeneratorSettings ByteArraySequenceGenerator::settings() const { return mSettings; }
inline void ByteArraySequenceGenerator::setSettings(const ByteArraySequenceGeneratorSettings& settings)
{
    mSettings = settings;
//     emit settingsChanged();
}

}

#endif
