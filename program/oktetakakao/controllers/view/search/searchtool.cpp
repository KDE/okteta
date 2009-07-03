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

#include "searchtool.h"

// controller
#include "searchuserqueryable.h"
#include "searchjob.h"
// lib
#include <kbytearraydisplay.h>
#include <kbytearraydocument.h>
// Okteta core
#include <charcodec.h>
#include <kbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>


SearchTool::SearchTool()
  : mIgnoreCase( true ),
    mUserQueryAgent( 0 ),
    mByteArrayDisplay( 0 ),
    mByteArrayModel( 0 )
{
    setObjectName( "Search" );
}

bool SearchTool::isApplyable() const
{
    return ( mByteArrayDisplay && mByteArrayModel );
//     const int newPosition = finalTargetOffset();

//     return ( mByteArrayDisplay && mByteArrayModel
//              && (0 <= newPosition) && (newPosition <= mByteArrayModel->size()) );
}

QString SearchTool::title() const { return i18nc("@title", "Search"); }

bool SearchTool::hasSelectedData()   const { return mByteArrayDisplay->hasSelectedData(); }
QString SearchTool::charCodingName() const { return mByteArrayDisplay->charCodingName(); }

void SearchTool::setTargetModel( AbstractModel* model )
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


void SearchTool::setUserQueryAgent( KDE::If::SearchUserQueryable* userQueryAgent )
{
    mUserQueryAgent = userQueryAgent;
}

void SearchTool::setSearchData( const QByteArray& searchData )
{
//     const bool oldIsApplyable = isApplyable();

    mSearchData = searchData;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void SearchTool::setIgnoreCase( int ignoreCase )
{
//     const bool oldIsApplyable = isApplyable();

    mIgnoreCase = ignoreCase;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void SearchTool::search( KFindDirection direction, bool fromCursor, bool inSelection )
{
    mPreviousFound = false;

    int startIndex;

    if( inSelection )
    {
        const KHE::Section selection = mByteArrayDisplay->selection();
        mSearchFirstIndex = selection.start();
        mSearchLastIndex =  selection.end();
        startIndex = selection.start();
        direction = FindForward; // TODO: why only forward?
    }
    else
    {
        const int cursorPosition = mByteArrayDisplay->cursorPosition();
        if( fromCursor && (cursorPosition!=0) )
        {
            mSearchFirstIndex = cursorPosition;
            mSearchLastIndex =  cursorPosition - 1;
        }
        else
        {
            mSearchFirstIndex = 0;
            mSearchLastIndex =  mByteArrayModel->size() - 1;
        }
        // TODO: should start at last 
        startIndex = (direction==FindForward) ? mSearchFirstIndex : mSearchLastIndex/*-mSearchData.size()*/;
    }

    doSearch( direction, startIndex );
}

void SearchTool::doSearch( KFindDirection direction, int startIndex )
{
    bool wrapEnabled = (direction==FindForward) ? (mSearchLastIndex<startIndex) : (startIndex<mSearchFirstIndex);

    while( true )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );

        const bool isForward = ( direction == FindForward );
        SearchJob* searchJob =
            new SearchJob( mByteArrayModel, mSearchData, startIndex, isForward, mIgnoreCase, mByteArrayDisplay->charCodingName() );
        const int pos = searchJob->exec();

        QApplication::restoreOverrideCursor();

        if( pos != -1 )
        {
            mPreviousFound = true;
            mByteArrayDisplay->setSelection( pos, pos+mSearchData.size()-1 );
            break;
        }

        if( wrapEnabled )
        {
            const bool wrapping = mUserQueryAgent ? mUserQueryAgent->queryContinue( direction ) : true;

            if( ! wrapping )
                break;
            startIndex = ( direction == FindForward ) ? 0 : mByteArrayModel->size()-1;
            wrapEnabled = false;
        }
        else
        {
            if( !mPreviousFound )
                emit dataNotFound();
            break;
        }
    }
    mByteArrayDisplay->setFocus();
}


SearchTool::~SearchTool()
{
}
