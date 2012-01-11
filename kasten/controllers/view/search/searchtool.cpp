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

#include "searchtool.h"

// controller
#include "searchuserqueryable.h"
#include "searchjob.h"
// lib
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Okteta core
#include <charcodec.h>
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>


namespace Kasten2
{

SearchTool::SearchTool()
  : mCaseSensitivity( Qt::CaseSensitive ),
    mUserQueryAgent( 0 ),
    mByteArrayView( 0 ),
    mByteArrayModel( 0 )
{
    setObjectName( QLatin1String( "Search" ) );
}

bool SearchTool::isApplyable() const
{
    return ( mByteArrayView && mByteArrayModel );
//     const int newPosition = finalTargetOffset();

//     return ( mByteArrayView && mByteArrayModel
//              && (0 <= newPosition) && (newPosition <= mByteArrayModel->size()) );
}

QString SearchTool::title() const { return i18nc("@title", "Search"); }

bool SearchTool::hasSelectedData()   const { return mByteArrayView->hasSelectedData(); }
QString SearchTool::charCodingName() const { return mByteArrayView->charCodingName(); }

void SearchTool::setTargetModel( AbstractModel* model )
{
    const bool oldIsApplyable = isApplyable();

    if( mByteArrayView ) mByteArrayView->disconnect( this );
    if( mByteArrayModel ) mByteArrayModel->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayView && mByteArrayModel )
    {
        connect( mByteArrayView,  SIGNAL(charCodecChanged(QString)),
                 SIGNAL(charCodecChanged(QString)) );
        // TODO: update isApplyable on cursor movement and size changes
    }

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}


void SearchTool::setUserQueryAgent( If::SearchUserQueryable* userQueryAgent )
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

void SearchTool::setCaseSensitivity( Qt::CaseSensitivity caseSensitivity )
{
//     const bool oldIsApplyable = isApplyable();

    mCaseSensitivity = caseSensitivity;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void SearchTool::search( KFindDirection direction, bool fromCursor, bool inSelection )
{
    mPreviousFound = false;

    if( inSelection )
    {
        const Okteta::AddressRange selection = mByteArrayView->selection();
        mSearchFirstIndex = selection.start();
        mSearchLastIndex =  selection.end();
    }
    else
    {
        const Okteta::Address cursorPosition = mByteArrayView->cursorPosition();
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
    }

    doSearch( direction );
}

void SearchTool::doSearch( KFindDirection direction )
{
    // TODO: should start at last
    Okteta::Address startIndex = (direction==FindForward) ? mSearchFirstIndex : mSearchLastIndex/*-mSearchData.size()*/;
    bool wrapEnabled = (direction==FindForward) ? (mSearchLastIndex<startIndex) : (startIndex<mSearchFirstIndex);

    while( true )
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );

        Okteta::Address endIndex = wrapEnabled ?
            ( (direction==FindForward) ? mByteArrayModel->size()-1 : 0 ) :
            ( (direction==FindForward) ? mSearchLastIndex : mSearchFirstIndex );

        SearchJob* searchJob =
            new SearchJob( mByteArrayModel, mSearchData, startIndex, endIndex, mCaseSensitivity, mByteArrayView->charCodingName() );
        const Okteta::Address pos = searchJob->exec();

        QApplication::restoreOverrideCursor();

        if( pos != -1 )
        {
            mPreviousFound = true;
            mByteArrayView->setSelection( pos, pos+mSearchData.size()-1 );
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
    mByteArrayView->setFocus();
}


SearchTool::~SearchTool()
{
}

}
