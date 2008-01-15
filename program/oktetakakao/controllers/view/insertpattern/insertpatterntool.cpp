/***************************************************************************
                          insertpatterntool.cpp  -  description
                             -------------------
    begin                : Fri Jan 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "insertpatterntool.h"

//
#include <kbytearraydocument.h>
// Kakao
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kbytearraymodel.h>
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
