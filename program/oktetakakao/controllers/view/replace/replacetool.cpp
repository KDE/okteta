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

#include "replacetool.h"

// controller
#include "replaceuserqueryable.h"
// search controller
#include "../search/searchjob.h"
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


ReplaceTool::ReplaceTool()
  : mUserQueryAgent( 0 ),
    mByteArrayDisplay( 0 ),
    mByteArrayModel( 0 )
{
    setObjectName( "Replace" );
}

bool ReplaceTool::isApplyable() const
{
    return ( mByteArrayDisplay && mByteArrayModel && !mByteArrayDisplay->isReadOnly() );
//     const int newPosition = finalTargetOffset();

//     return ( mByteArrayDisplay && mByteArrayModel
//              && (0 <= newPosition) && (newPosition <= mByteArrayModel->size()) );
}

QString ReplaceTool::title() const { return i18nc("@title", "Replace"); }

bool ReplaceTool::hasSelectedData()   const { return mByteArrayDisplay->hasSelectedData(); }
QString ReplaceTool::charCodingName() const { return mByteArrayDisplay->charCodingName(); }


void ReplaceTool::setTargetModel( AbstractModel* model )
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
        connect( mByteArrayDisplay, SIGNAL(readOnlyChanged( bool )), SLOT(onReadOnlyChanged( bool )) );
        // TODO: update isApplyable on cursor movement and size changes
    }

    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}


void ReplaceTool::setUserQueryAgent( KDE::If::ReplaceUserQueryable* userQueryAgent )
{
    mUserQueryAgent = userQueryAgent;
}

void ReplaceTool::setSearchData( const QByteArray& searchData )
{
//     const bool oldIsApplyable = isApplyable();

    mSearchData = searchData;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setReplaceData( const QByteArray& replaceData )
{
//     const bool oldIsApplyable = isApplyable();

    mReplaceData = replaceData;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setIgnoreCase( int ignoreCase )
{
//     const bool oldIsApplyable = isApplyable();

    mIgnoreCase = ignoreCase;

//     const bool newIsApplyable = isApplyable();
//     if( oldIsApplyable != newIsApplyable )
//         emit isApplyableChanged( newIsApplyable );
}

void ReplaceTool::setDoPrompt( int doPrompt )
{
    mDoPrompt = doPrompt;
}

void ReplaceTool::replace( KFindDirection direction, bool fromCursor, bool inSelection )
{
    mPreviousFound = false;

    int startIndex;

    if( inSelection )
    {
        const KHE::Section selection = mByteArrayDisplay->selection();
        mReplaceFirstIndex = selection.start();
        mReplaceLastIndex =  selection.end();
        startIndex = selection.start();
        mDoWrap = true; // TODO: no wrapping needed, or?
        direction = FindForward; // TODO: why only forward?
    }
    else
    {
        const int cursorPosition = mByteArrayDisplay->cursorPosition();
        if( fromCursor && (cursorPosition!=0) )
        {
            mReplaceFirstIndex = cursorPosition;
            mReplaceLastIndex =  cursorPosition-1;
        }
        else
        {
            mReplaceFirstIndex = 0;
            mReplaceLastIndex =  mByteArrayModel->size()-1;
        }
        startIndex = (direction==FindForward) ? mReplaceFirstIndex : mReplaceLastIndex/*-mSearchData.size()*/;
        mDoWrap = (direction==FindForward) ? (mReplaceLastIndex<startIndex) : (startIndex<mReplaceFirstIndex);
    }

    doReplace( direction, startIndex );
}

void ReplaceTool::doReplace( KFindDirection direction, int startIndex )
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    int noOfReplacements = 0;

    while( true )
    {
        // TODO: support ignorecase
        const bool isForward = ( direction == FindForward );
        SearchJob* searchJob =
            new SearchJob( mByteArrayModel, mSearchData, startIndex, isForward, mIgnoreCase, mByteArrayDisplay->charCodingName() );
        const int pos = searchJob->exec();

        if( pos != -1 )
        {
            mPreviousFound = true;

            startIndex = pos;
            bool currentToBeReplaced;
            bool isCancelled;

            if( mDoPrompt )
            {
                QApplication::restoreOverrideCursor();

                mByteArrayDisplay->setSelection( pos, pos+mSearchData.size()-1 );

                const KDE::ReplaceBehaviour replaceBehaviour = mUserQueryAgent ?
                     mUserQueryAgent->queryReplaceCurrent() :
                     KDE::ReplaceAll;

                mByteArrayDisplay->selectAllData( false );

                switch( replaceBehaviour )
                {
                case KDE::ReplaceAll:
                    mDoPrompt = false;
                    currentToBeReplaced = true;
                    isCancelled = false;
                    break;
                case KDE::ReplaceCurrent:
                    currentToBeReplaced = true;
                    isCancelled = false;
                    break;
                case KDE::SkipCurrent:
                    if( isForward )
                        ++startIndex;
                    else
                        --startIndex;
                    currentToBeReplaced = false;
                    isCancelled = false;
                    break;
                case KDE::CancelReplacing:
                default:
                    currentToBeReplaced = false;
                    isCancelled = true;
                    mDoWrap = false;
                }
            }
            else
            {
                currentToBeReplaced = true;
                isCancelled = false;
            }

            if( currentToBeReplaced )
            {
                ++noOfReplacements;
                const int inserted = mByteArrayModel->replace( startIndex, mSearchData.size(),
                                                               mReplaceData.constData(), mReplaceData.size() );
                if( isForward )
                    startIndex += inserted;
                else
                    startIndex -= inserted;
            }

            if( ! isCancelled )
                continue;
        }

        QApplication::restoreOverrideCursor();
        // reached end
        if( mDoWrap )
        {
            const bool wrapping = mUserQueryAgent ? mUserQueryAgent->queryContinue( direction, noOfReplacements ) : true;

            if( ! wrapping )
                break;

            startIndex = (direction==FindForward) ? 0 : mByteArrayModel->size()-1;
            mDoWrap = false;
            noOfReplacements = 0;

            QApplication::setOverrideCursor( Qt::WaitCursor );
        }
        else
        {
            emit finished( mPreviousFound, noOfReplacements );

            break;
        }
    }
}

void ReplaceTool::onReadOnlyChanged( bool isReadOnly )
{
Q_UNUSED( isReadOnly )

    // TODO: find out if isApplyable really changed, perhaps by caching the readonly state?
    emit isApplyableChanged( isApplyable() );
}

ReplaceTool::~ReplaceTool()
{
}
