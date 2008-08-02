/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "poddecodertool.h"

//TODO: remove if signal works again
#include "kprimitivetypesview.h"
// lib
#include <kbytearraydocument.h>
// Kakao gui
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kbytearraymodel.h>
// KDE
#include <KLocale>


PODDecoderTool::PODDecoderTool()
 : mByteArrayView( 0 ), mByteArrayModel( 0 ), mCursorIndex( 0 ),
   mPODView( 0 )
{
    setObjectName( "PODDecoder" );
}

QString PODDecoderTool::title() const { return i18nc("@title:window", "Decoding Table"); }
PODData &PODDecoderTool::podData() { return mPODData; }

void PODDecoderTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );
    if( mByteArrayModel ) mByteArrayModel->disconnect( this );

    KAbstractView* view = model ? qobject_cast<KAbstractView*>( model ) : 0;
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

#include "poddecodertool.moc"
