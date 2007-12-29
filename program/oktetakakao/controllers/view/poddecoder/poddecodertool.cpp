/***************************************************************************
                          poddecodertool.cpp  -  description
                             -------------------
    begin                : Wed Dec 26 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "poddecodertool.h"

//TODO: remove if signal works again
#include "kprimitivetypesview.h"
//
#include <kbytearraydocument.h>
// Kakao
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kbytearraymodel.h>

PODDecoderTool::PODDecoderTool()
 : mByteArrayView( 0 ), mByteArrayModel( 0 ), mCursorIndex( 0 ),
   mPODView( 0 )
{
}

PODData &PODDecoderTool::podData() { return mPODData; }

void PODDecoderTool::setView( KAbstractView *view )
{
    disconnect( mByteArrayView );
    disconnect( mByteArrayModel );

    mByteArrayView = view ? qobject_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;
    KByteArrayDocument *document = view ? qobject_cast<KByteArrayDocument*>( view->document() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayModel && mByteArrayView )
    {
        mCursorIndex = mByteArrayView->cursorPosition();
        connect( mByteArrayView, SIGNAL(cursorPositionChanged( int )), SLOT(onCursorPositionChange( int )) );
        connect( mByteArrayModel, SIGNAL(contentsChanged( int, int )), SLOT(onContentsChange( int, int )) );
    }

    updateData();
}

void PODDecoderTool::setPODView( KPrimitiveTypesView *view )
{
    mPODView = view;
}

void PODDecoderTool::onCursorPositionChange( int pos )
{
    mCursorIndex = pos;
    updateData();
}

void PODDecoderTool::onContentsChange( int start, int end )
{
    if( start-mPODData.Size < mCursorIndex && mCursorIndex <= end )
        updateData();
}


void PODDecoderTool::updateData()
{
    int dataSize;
    if( mByteArrayModel )
    {
        dataSize = mByteArrayModel->size() - mCursorIndex;
        if( dataSize > mPODData.Size )
            dataSize = mPODData.Size;
        else if( dataSize < 0 )
            dataSize = 0;
    }
    else
        dataSize = 0;

    const bool hasDataSet = dataSize > 0;
    if( hasDataSet )
        mByteArrayModel->copyTo( (char*)mPODData.rawData(), mCursorIndex, mPODData.Size );

    const bool hasChanged = mPODData.updateRawData( dataSize );

    if( hasChanged )
    {
       //TODO: remove if signal works again
       if( mPODView ) mPODView->onDataChange();
       emit dataChanged( mPODData.data() );
    }
}
