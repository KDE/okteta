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

#include "bytearrayrandomdatagenerator.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF5
#include <KLocalizedString>
// Qt
#include <QMimeData>
#include <QByteArray>
// Std
#include <ctime>

namespace Kasten {

ByteArrayRandomDataGeneratorSettings::ByteArrayRandomDataGeneratorSettings() = default;

// TODO: support insert to selection, cmp. fill in painting program
// there are two kinds of generated datam fixed size (e.g. sequence) and endless size?
// perhaps by option fill selection? or a separate menu entry fill, which only works on selections?

ByteArrayRandomDataGenerator::ByteArrayRandomDataGenerator()
    : AbstractModelDataGenerator(
        i18nc("name of the generated data", "Random Data..."),
        QStringLiteral("application/octet-stream"),
        DynamicGeneration)
{}

ByteArrayRandomDataGenerator::~ByteArrayRandomDataGenerator() = default;

// TODO: use different RNG, with multiple characteristics and offer them in the config
QMimeData* ByteArrayRandomDataGenerator::generateData()
{
    qsrand((unsigned int)time(nullptr));

    const int insertDataSize = mSettings.size;
    QByteArray insertData(insertDataSize, '\0');

    for (int i = 0; i < insertDataSize; ++i) {
        insertData[i] = qrand() % 256; // TODO: modulo is expensive, even if easy to use

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
