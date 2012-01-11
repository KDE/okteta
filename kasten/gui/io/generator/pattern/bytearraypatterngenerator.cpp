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

#include "bytearraypatterngenerator.h"

// KDE
#include <KLocale>
// Qt
#include <QtCore/QMimeData>
#include <QtCore/QByteArray>


namespace Kasten2
{
static const char PatternMimeType[] = "application/octet-stream";


ByteArrayPatternGeneratorSettings::ByteArrayPatternGeneratorSettings()
  : pattern( 1, 0 ), count( 1 )
{
}


//TODO: support insert to selection, cmp. fill in painting program
// there are two kinds of generated datam fixed size (e.g. sequence) and endless size?
// perhaps by option fill selection? or a separate menu entry fill, which only works on selections?

ByteArrayPatternGenerator::ByteArrayPatternGenerator()
  : AbstractModelDataGenerator(
        i18nc("name of the generated data", "Pattern..."),
        QString::fromLatin1(PatternMimeType),
        DynamicGeneration )
{}


// TODO: optimize and check if pattern is just one byte, so memset can be used
// TODO: see if copying larger chunks with memcpy is faster, so 
QMimeData* ByteArrayPatternGenerator::generateData()
{
    const int patternSize = mSettings.pattern.size();

    const int insertDataSize = mSettings.count * patternSize;

    QByteArray insertData( insertDataSize, '\0' );

    char* rawInsertData = insertData.data();
    const char* rawPatternData = mSettings.pattern.constData();

    for( int i=0; i < insertDataSize; i+= patternSize )
        memcpy( &rawInsertData[i], rawPatternData, patternSize );

    QMimeData* mimeData = new QMimeData;
    mimeData->setData( mimeType(), insertData );

// TODO: a method to get the description of the change, e.g. 
#if 0
    Okteta::ChangesDescribable *changesDescribable =
        qobject_cast<Okteta::ChangesDescribable*>( mByteArrayModel );

    if( changesDescribable )
        changesDescribable->openGroupedChange( i18n("Pattern inserted.") );
    mByteArrayView->insert( insertData );
//     mByteArrayModel->replace( filteredSection, filterResult );
    if( changesDescribable )
        changesDescribable->closeGroupedChange();
#endif

    return mimeData;
}

ByteArrayPatternGenerator::~ByteArrayPatternGenerator() {}

}
