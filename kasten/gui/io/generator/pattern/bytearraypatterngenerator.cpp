/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraypatterngenerator.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QMimeData>
#include <QByteArray>

namespace Kasten {

ByteArrayPatternGeneratorSettings::ByteArrayPatternGeneratorSettings() = default;

// TODO: support insert to selection, cmp. fill in painting program
// there are two kinds of generated datam fixed size (e.g. sequence) and endless size?
// perhaps by option fill selection? or a separate menu entry fill, which only works on selections?

ByteArrayPatternGenerator::ByteArrayPatternGenerator()
    : AbstractModelDataGenerator(
        i18nc("name of the generated data", "Pattern..."),
        QStringLiteral("application/octet-stream"),
        DynamicGeneration)
{}

ByteArrayPatternGenerator::~ByteArrayPatternGenerator() = default;

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
