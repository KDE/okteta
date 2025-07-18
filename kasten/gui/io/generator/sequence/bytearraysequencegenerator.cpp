/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraysequencegenerator.hpp"

// Okteta core
#include <Okteta/Byte>
// KF
#include <KLocalizedString>
// Qt
#include <QMimeData>

namespace Kasten {

ByteArraySequenceGeneratorSettings::ByteArraySequenceGeneratorSettings() = default;

// TODO: support insert to selection, cmp. fill in painting program
// there are two kinds of generated datam fixed size (e.g. sequence) and endless size?
// perhaps by option fill selection? or a separate menu entry fill, which only works on selections?

ByteArraySequenceGenerator::ByteArraySequenceGenerator()
    : AbstractModelDataGenerator(
        i18nc("name of the generated data", "Sequence"),
        QStringLiteral("application/octet-stream"),
        StaticGeneration)
{}

ByteArraySequenceGenerator::~ByteArraySequenceGenerator() = default;

// TODO: optimize and check if pattern is just one byte, so memset can be used
// TODO: see if copying larger chunks with memcpy is faster, so
std::unique_ptr<QMimeData> ByteArraySequenceGenerator::generateData()
{
    const Okteta::Byte firstByte = 0;
    const Okteta::Byte lastByte = 255;

    const int insertDataSize = lastByte - firstByte + 1;
    QByteArray insertData(insertDataSize, '\0');

    Okteta::Byte byte = firstByte;
    for (int i = 0; i < insertDataSize; ++i, ++byte) {
        insertData[i] = byte;
    }

    auto mimeData = std::make_unique<QMimeData>();
    mimeData->setData(mimeType(), insertData);

// TODO: a method to get the description of the change, e.g.
#if 0
    auto* changesDescribable = qobject_cast<Okteta::ChangesDescribable*>(mByteArrayModel);

    if (changesDescribable) {
        changesDescribable->openGroupedChange(i18nc("@item", "Sequence Inserted"));
    }
    mByteArrayView->insertBytes(insertData);
//     mByteArrayModel->replaceBytes( filteredSection, filterResult );
    if (changesDescribable) {
        changesDescribable->closeGroupedChange();
    }
#endif

    return mimeData;
}

}

#include "moc_bytearraysequencegenerator.cpp"
