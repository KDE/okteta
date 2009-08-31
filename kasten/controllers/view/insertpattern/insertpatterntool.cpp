/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "insertpatterntool.h"

// lib
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Okteta core
#include <abstractbytearraymodel.h>
#include <changesdescribable.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QByteArray>


namespace Kasten
{

InsertPatternTool::InsertPatternTool()
 : mByteArrayView( 0 ), mByteArrayModel( 0 )
{
}

QString InsertPatternTool::charCodecName() const
{
    return mByteArrayView ? mByteArrayView->charCodingName() : QString();
}

bool InsertPatternTool::hasWriteable() const
{
    return ( mByteArrayView && mByteArrayModel ) ? !mByteArrayView->isReadOnly() : false;
}

void InsertPatternTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    const bool hasView = ( mByteArrayView && mByteArrayModel );
    if( hasView )
    {
        connect( mByteArrayView, SIGNAL(readOnlyChanged( bool )), SLOT(onReadOnlyChanged( bool )) );
    }

    const bool isWriteable = ( hasView && !mByteArrayView->isReadOnly() );

    emit hasWriteableChanged( isWriteable );
}

void InsertPatternTool::insertPattern( const QByteArray &pattern, int count )
{
    const int patternSize = pattern.size();

    const int insertDataSize = count * patternSize;
    QByteArray insertData( insertDataSize, '\0' );

    for( int i=0; i < insertDataSize; i+= patternSize )
        memcpy( &insertData.data()[i], pattern.constData(), patternSize );

    //TODO: support insert to selection
//     const KDE::Section selection = mByteArrayView->selection();

    Okteta::ChangesDescribable *changesDescribable =
        qobject_cast<Okteta::ChangesDescribable*>( mByteArrayModel );

    if( changesDescribable )
        changesDescribable->openGroupedChange( i18n("Pattern inserted.") );
    mByteArrayView->insert( insertData );
//     mByteArrayModel->replace( filteredSection, filterResult );
    if( changesDescribable )
        changesDescribable->closeGroupedChange();
}


void InsertPatternTool::onReadOnlyChanged( bool isReadOnly )
{
    const bool isWriteable = !isReadOnly;

    emit hasWriteableChanged( isWriteable );
}

InsertPatternTool::~InsertPatternTool() {}

}
