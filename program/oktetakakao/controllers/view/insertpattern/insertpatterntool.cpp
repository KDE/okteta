/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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
#include <kbytearraydisplay.h>
#include <kbytearraydocument.h>
// Okteta core
#include <abstractbytearraymodel.h>
#include <changesdescribable.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QByteArray>


InsertPatternTool::InsertPatternTool()
 : mByteArrayDisplay( 0 ), mByteArrayModel( 0 )
{
}

QString InsertPatternTool::charCodecName() const
{
    return mByteArrayDisplay ? mByteArrayDisplay->charCodingName() : QString();
}

bool InsertPatternTool::hasWriteable() const
{
    return ( mByteArrayDisplay && mByteArrayModel ) ? !mByteArrayDisplay->isReadOnly() : false;
}

void InsertPatternTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayDisplay ) mByteArrayDisplay->disconnect( this );

    mByteArrayDisplay = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;

    KByteArrayDocument* document =
        mByteArrayDisplay ? qobject_cast<KByteArrayDocument*>( mByteArrayDisplay->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    const bool hasView = ( mByteArrayDisplay && mByteArrayModel );
    if( hasView )
    {
        connect( mByteArrayDisplay, SIGNAL(readOnlyChanged( bool )), SLOT(onReadOnlyChanged( bool )) );
    }

    const bool isWriteable = ( hasView && !mByteArrayDisplay->isReadOnly() );

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
//     const KHE::Section selection = mByteArrayDisplay->selection();

    KHECore::ChangesDescribable *changesDescribable =
        qobject_cast<KHECore::ChangesDescribable*>( mByteArrayModel );

    if( changesDescribable )
        changesDescribable->openGroupedChange( i18n("Pattern inserted.") );
    mByteArrayDisplay->insert( insertData );
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
