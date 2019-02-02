/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BYTEARRAYSEQUENCEGENERATOR_HPP
#define BYTEARRAYSEQUENCEGENERATOR_HPP

// Kasten core
#include <kasten/abstractmodeldatagenerator.hpp>
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

protected:
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