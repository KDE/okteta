/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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
#include <kbytearraydisplay.h>
#include <kbytearraydocument.h>
// Okteta core
#include <kbytearraymodel.h>
// KDE
#include <KLocale>


GotoOffsetTool::GotoOffsetTool()
  : mTargetOffset( 0 ),
    mIsRelative( false ),
    mIsSelectionToExtent( false ),
    mIsBackwards( false ),
    mByteArrayDisplay( 0 ),
    mByteArrayModel( 0 )
{
    setObjectName( "GotoOffset" );
}

int GotoOffsetTool::currentOffset() const
{
    return mByteArrayDisplay ?
        mByteArrayDisplay->startOffset() + mByteArrayDisplay->cursorPosition() :
        -1;
}

bool GotoOffsetTool::isApplyable() const
{
    const int newPosition = finalTargetOffset();

    return ( mByteArrayDisplay && mByteArrayModel
             && (0 <= newPosition) && (newPosition <= mByteArrayModel->size()) );
}

QString GotoOffsetTool::title() const { return i18nc("@title:window of the tool to set a new offset for the cursor", "Goto"); }

void GotoOffsetTool::setTargetModel( AbstractModel* model )
{
    const bool oldIsApplyable = isApplyable();

    if( mByteArrayDisplay ) mByteArrayDisplay->disconnect( this );
    if( mByteArrayModel ) mByteArrayModel->disconnect( this );

    mByteArrayDisplay = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;

    KByteArrayDocument* document =
        mByteArrayDisplay ? qobject_cast<KByteArrayDocument*>( mByteArrayDisplay->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayDisplay && mByteArrayModel )
    {
        // TODO: update isApplyable on cursor movement and size changes
    }

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}

void GotoOffsetTool::setTargetOffset( int targetOffset )
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
        mByteArrayDisplay->setSelectionCursorPosition( newPosition );
    else
        mByteArrayDisplay->setCursorPosition( newPosition );
    mByteArrayDisplay->setFocus();
}


int GotoOffsetTool::finalTargetOffset() const
{
    const int newPosition =
        (! mByteArrayDisplay) ? -1 :
        mIsRelative ?
            ( mIsBackwards ? mByteArrayDisplay->cursorPosition() - mTargetOffset :
                             mByteArrayDisplay->cursorPosition() + mTargetOffset ) :
            ( mIsBackwards ? mByteArrayModel->size() - mTargetOffset :
                             mTargetOffset );

    return newPosition;
}

GotoOffsetTool::~GotoOffsetTool()
{
}
