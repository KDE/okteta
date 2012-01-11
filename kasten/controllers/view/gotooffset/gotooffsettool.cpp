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

#include "gotooffsettool.h"

// lib
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>


namespace Kasten2
{

GotoOffsetTool::GotoOffsetTool()
  : mTargetOffset( 0 ),
    mIsRelative( false ),
    mIsSelectionToExtent( false ),
    mIsBackwards( false ),
    mByteArrayView( 0 ),
    mByteArrayModel( 0 )
{
    setObjectName( QLatin1String( "GotoOffset" ) );
}

int GotoOffsetTool::currentOffset() const
{
    return mByteArrayView ?
        mByteArrayView->startOffset() + mByteArrayView->cursorPosition() :
        -1;
}

bool GotoOffsetTool::isUsable() const
{
    return ( mByteArrayView && mByteArrayModel && (mByteArrayModel->size() > 0) );
}

bool GotoOffsetTool::isApplyable() const
{
    const int newPosition = finalTargetOffset();

    return ( mByteArrayView && mByteArrayModel
             && (0 <= newPosition) && (newPosition <= mByteArrayModel->size()) );
}

QString GotoOffsetTool::title() const { return i18nc("@title:window of the tool to set a new offset for the cursor", "Goto"); }

void GotoOffsetTool::setTargetModel( AbstractModel* model )
{
    const bool oldIsUsable = isUsable();
    const bool oldIsApplyable = isApplyable();

    if( mByteArrayView ) mByteArrayView->disconnect( this );
    if( mByteArrayModel ) mByteArrayModel->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayView && mByteArrayModel )
    {
        connect( mByteArrayModel, SIGNAL(contentsChanged(Okteta::ArrayChangeMetricsList)),
                 SLOT(onContentsChanged()) );
        // TODO: update isApplyable on cursor movement and size changes
    }

    const bool newIsUsable = isUsable();
    const bool newIsApplyable = isApplyable();
    if( oldIsUsable != newIsUsable )
        emit isUsableChanged( newIsUsable );
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}

void GotoOffsetTool::setTargetOffset( Okteta::Address targetOffset )
{
    const bool oldIsApplyable = isApplyable();

    mTargetOffset = targetOffset;

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}

void GotoOffsetTool::setIsRelative( bool isRelative )
{
    const bool oldIsApplyable = isApplyable();

    mIsRelative = isRelative;

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}

void GotoOffsetTool::setIsSelectionToExtent( bool isSelectionToExtent )
{
    const bool oldIsApplyable = isApplyable();

    mIsSelectionToExtent = isSelectionToExtent;

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}

void GotoOffsetTool::setIsBackwards( bool isBackwards )
{
    const bool oldIsApplyable = isApplyable();

    mIsBackwards = isBackwards;

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}


void GotoOffsetTool::gotoOffset()
{
    const int newPosition = finalTargetOffset();

    if( mIsSelectionToExtent )
        mByteArrayView->setSelectionCursorPosition( newPosition );
    else
        mByteArrayView->setCursorPosition( newPosition );
    mByteArrayView->setFocus();
}


int GotoOffsetTool::finalTargetOffset() const
{
    const int newPosition =
        (! mByteArrayView) ? -1 :
        mIsRelative ?
            ( mIsBackwards ? mByteArrayView->cursorPosition() - mTargetOffset :
                             mByteArrayView->cursorPosition() + mTargetOffset ) :
            ( mIsBackwards ? mByteArrayModel->size() - mTargetOffset :
                             mTargetOffset );

    return newPosition;
}

void GotoOffsetTool::onContentsChanged()
{
    // TODO: find status before content changed, e.g. by caching
    emit isUsableChanged( isUsable() );
}

GotoOffsetTool::~GotoOffsetTool()
{
}

}
