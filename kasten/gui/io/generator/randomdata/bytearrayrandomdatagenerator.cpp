/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrandomdatagenerator.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
#include <QRandomGenerator>
#endif
#include <QMimeData>
#include <QByteArray>
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
// Std
#include <ctime>
#endif

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
#if (QT_VERSION < QT_VERSION_CHECK(5, 10, 0))
    qsrand((unsigned int)time(nullptr));
#endif

    const int insertDataSize = mSettings.size;
    QByteArray insertData(insertDataSize, '\0');

#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    auto* randomGenerator = QRandomGenerator::global();
#endif
    for (int i = 0; i < insertDataSize; ++i) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        insertData[i] = static_cast<char>(randomGenerator->bounded(256));
#else
        insertData[i] = qrand() % 256; // TODO: modulo is expensive, even if easy to use
#endif
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
