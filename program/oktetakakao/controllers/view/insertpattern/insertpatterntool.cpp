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
#include <kbytearraydocument.h>
// Kakao gui
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kabstractbytearraymodel.h>
// Qt
#include <QtCore/QByteArray>


InsertPatternTool::InsertPatternTool()
 : mByteArrayView( 0 ), mByteArrayModel( 0 )
{
}

QString InsertPatternTool::charCodecName() const
{
    return mByteArrayView ? mByteArrayView->encodingName() : QString();
}

void InsertPatternTool::setView( KAbstractView *view )
{
//     if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    KByteArrayDocument *document = view ? static_cast<KByteArrayDocument*>( view->document() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    const bool hasView = ( mByteArrayView != 0 );
    emit viewChanged( hasView );
}

void InsertPatternTool::insertPattern( const QByteArray &pattern, int count )
{
    const int patternSize = pattern.size();

    const int insertDataSize = count * patternSize;
    QByteArray insertData( insertDataSize, '\0' );

    for( int i=0; i < insertDataSize; i+= patternSize )
        memcpy( &insertData.data()[i], pattern.constData(), patternSize );

    //TODO: support insert to selection
    mByteArrayView->insert( insertData );
}

InsertPatternTool::~InsertPatternTool() {}

#include "insertpatterntool.moc"
