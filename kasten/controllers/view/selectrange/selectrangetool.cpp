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

#include "selectrangetool.h"

// lib
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>


namespace Kasten2
{

SelectRangeTool::SelectRangeTool()
  : mTargetStart( 0 ),
    mTargetEnd( -1 ),
    mIsEndRelative( false ),
    mIsEndBackwards( false ),
    mByteArrayView( 0 ),
    mByteArrayModel( 0 )
{
    setObjectName( QLatin1String( "SelectRange" ) );
}

int SelectRangeTool::currentSelectionStart() const
{
    return mByteArrayView ?
        mByteArrayView->startOffset() + mByteArrayView->selection().start() :
        -1;
}
int SelectRangeTool::currentSelectionEnd() const
{
    return mByteArrayView ?
        mByteArrayView->startOffset() + mByteArrayView->selection().end() :
        -1;
}

bool SelectRangeTool::isUsable() const
{
    return ( mByteArrayView && mByteArrayModel && (mByteArrayModel->size() > 0) );
}

bool SelectRangeTool::isApplyable() const
{
    const int start = finalTargetSelectionStart();
    const int end =   finalTargetSelectionEnd();

    return ( mByteArrayView && mByteArrayModel
             && (start <= end)
             && (0 <= start) && (start < mByteArrayModel->size())
             && (0 <= end) && (end < mByteArrayModel->size()) );
}

QString SelectRangeTool::title() const { return i18nc("@title:window of the tool to select a range", "Select"); }

void SelectRangeTool::setTargetModel( AbstractModel* model )
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

void SelectRangeTool::setTargetStart( Okteta::Address start )
{
    const bool oldIsApplyable = isApplyable();

    mTargetStart = start;

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}

void SelectRangeTool::setTargetEnd( Okteta::Address end )
{
    const bool oldIsApplyable = isApplyable();

    mTargetEnd = end;

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}

void SelectRangeTool::setIsEndRelative( bool isEndRelative )
{
    const bool oldIsApplyable = isApplyable();

    mIsEndRelative = isEndRelative;

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}

void SelectRangeTool::setIsEndBackwards( bool isEndBackwards )
{
    const bool oldIsApplyable = isApplyable();

    mIsEndBackwards = isEndBackwards;

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}


void SelectRangeTool::select()
{
    const int start = finalTargetSelectionStart();
    const int end =   finalTargetSelectionEnd();

    mByteArrayView->setSelection( start, end );
    mByteArrayView->setFocus();
}


int SelectRangeTool::finalTargetSelectionStart() const
{
    const int end =
        (! mByteArrayView) ? -1 :
        mIsEndRelative && mIsEndBackwards ? mTargetStart - mTargetEnd + 1 :
            mTargetStart;

    return end;
}

int SelectRangeTool::finalTargetSelectionEnd() const
{
    const int end =
        (! mByteArrayView) ? -1 :
        mIsEndRelative ?
            ( mIsEndBackwards ? mTargetStart :
                                mTargetStart + mTargetEnd - 1 ) :
            mTargetEnd;

    return end;
}

void SelectRangeTool::onContentsChanged()
{
    // TODO: find status before content changed, e.g. by caching
    emit isUsableChanged( isUsable() );
}

SelectRangeTool::~SelectRangeTool()
{
}

}
