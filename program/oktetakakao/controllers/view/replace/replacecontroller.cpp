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

#include "replacecontroller.h"

// controller
#include "kreplacedialog.h"
#include "kreplaceprompt.h"
// search controller
#include "../search/searchjob.h"
// lib
#include <kbytearraydocument.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kcharcodec.h>
#include <kbytearraymodel.h>
// Kakao gui
#include <kabstractview.h>
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KMessageBox>
// Qt
#include <QtGui/QApplication>


// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
ReplaceController::ReplaceController( KXmlGuiWindow *window )
 : mWindow( window ), mByteArrayView( 0 ), mByteArrayModel( 0 ), mReplaceDialog( 0 ), mReplacePrompt( 0 )
{
    KActionCollection* ActionCollection = mWindow->actionCollection();

    mReplaceAction = KStandardAction::replace( this, SLOT(replace()), ActionCollection );

    setView( 0 );
}

void ReplaceController::setView( KAbstractView *view )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;
    KByteArrayDocument *Document = view ? static_cast<KByteArrayDocument*>( view->document() ) : 0;
    mByteArrayModel = Document ? Document->content() : 0;

    if( mByteArrayModel )
    {
    }
    const bool hasView = ( mByteArrayModel != 0 );
    mReplaceAction->setEnabled( hasView );
}


void ReplaceController::replace()
{
    // ensure dialog
    if( !mReplaceDialog )
    {
        mReplaceDialog = new KReplaceDialog( mWindow );
        connect( mReplaceDialog, SIGNAL(okClicked()), SLOT(onDialogOkClicked()) );
    }

    mReplaceDialog->setInSelection( mByteArrayView->hasSelectedData() );
    mReplaceDialog->setCharCodec( mByteArrayView->encodingName() );

    mReplaceDialog->show();
}

void ReplaceController::onDialogOkClicked()
{
    mPreviousFound = false;
    mNoOfReplacements = 0;

    mReplaceDialog->hide();

    // prepare settings
    mSearchData = mReplaceDialog->data();
    mReplaceData = mReplaceDialog->replaceData();
    mIgnoreCase = mReplaceDialog->ignoreCase();
    mDoPrompt = mReplaceDialog->prompt();

    if( mReplaceDialog->inSelection() )
    {
        mDirection = FindForward;
        const KHE::KSection selection = mByteArrayView->selection();
        mReplaceFirstIndex = selection.start();
        mReplaceLastIndex =  selection.end();
        mCurrentIndex = selection.start();
        mDoWrap = true;
    }
    else
    {
        mDirection = mReplaceDialog->direction();
        const int cursorPosition = mByteArrayView->cursorPosition();
        if( mReplaceDialog->fromCursor() && (cursorPosition!=0) )
        {
            mReplaceFirstIndex = cursorPosition;
            mReplaceLastIndex =  cursorPosition-1;
        }
        else
        {
            mReplaceFirstIndex = 0;
            mReplaceLastIndex =  mByteArrayModel->size()-1;
        }
        mCurrentIndex = (mDirection==FindForward) ? mReplaceFirstIndex : mReplaceLastIndex;
        mDoWrap = (mDirection==FindForward) ? (mReplaceLastIndex<mCurrentIndex) : (mCurrentIndex<mReplaceFirstIndex);
    }

    findNext();
}

void ReplaceController::findNext()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    while( true )
    {
        // TODO: support ignorecase
        SearchJob *searchJob = new SearchJob( mByteArrayModel, mSearchData, mCurrentIndex, (mDirection==FindForward) );
        const int pos = searchJob->exec();

        if( pos != -1 )
        {
            mPreviousFound = true;

            mCurrentIndex = pos;
            if( mDoPrompt )
            {
                QApplication::restoreOverrideCursor();

                mByteArrayView->setSelection( pos, pos+mSearchData.size()-1 );
                if( !mReplacePrompt )
                {
                    mReplacePrompt = new KReplacePrompt( mWindow );
                    connect( mReplacePrompt, SIGNAL(user1Clicked()), SLOT(onPromptAllClicked()) );
                    connect( mReplacePrompt, SIGNAL(user2Clicked()), SLOT(onPromptSkipClicked()) );
                    connect( mReplacePrompt, SIGNAL(user3Clicked()), SLOT(onPromptReplaceClicked()) );
                }
                if( mReplacePrompt->isHidden() )
                    mReplacePrompt->show();
             break;
           }
           else
           {
               replaceCurrent();
               continue;
           }
        }

        if( mReplacePrompt )
            mReplacePrompt->hide();
        mByteArrayView->selectAll( false );

        const QString replacementReport = (mNoOfReplacements==0) ?
            i18nc( "@info", "No replacements done.") :
            i18ncp( "@info", "1 replacement done.", "%1 replacements done.", mNoOfReplacements );
        const QString messageBoxTitle = i18nc( "@title:window", "Replace" );
        // reached end and
        if( mDoWrap )
        {
            QApplication::restoreOverrideCursor();

            const QString Question = ((mDirection==FindForward) ?
                i18nc( "@info", "<nl/>End of byte array reached.<nl/>Continue from the beginning?" ) :
                i18nc( "@info", "<nl/>Beginning of byte array reached.<nl/>Continue from the end?" ));

            const int answer = KMessageBox::questionYesNo( mWindow, replacementReport+Question, messageBoxTitle,
                                                           KStandardGuiItem::cont(), KStandardGuiItem::cancel() );
            if( answer == KMessageBox::No )
                break;
            mCurrentIndex = (mDirection==FindForward) ? 0 : mByteArrayModel->size()-1;
            mDoWrap = false;
            mNoOfReplacements = 0;

            QApplication::setOverrideCursor( Qt::WaitCursor );
        }
        else
        {
            QApplication::restoreOverrideCursor();

            if( !mPreviousFound )
                KMessageBox::sorry( mWindow, i18nc("@info","Replace pattern not found in byte array."), messageBoxTitle );
            else
                KMessageBox::information( mWindow, replacementReport, messageBoxTitle );
            break;
        }
    }
}

void ReplaceController::replaceCurrent()
{
    ++mNoOfReplacements;
    const int inserted = mByteArrayModel->replace( mCurrentIndex, mSearchData.size(),
                                             mReplaceData.constData(), mReplaceData.size() );
    if( mDirection == FindForward )
        mCurrentIndex += inserted;
    else
        --mCurrentIndex;
}

void ReplaceController::onPromptSkipClicked()
{
    // skip current
    {
        if( mDirection == FindForward )
            mCurrentIndex += mReplaceData.size();
        else
            --mCurrentIndex;
    }
    findNext();
}

void ReplaceController::onPromptReplaceClicked()
{
    replaceCurrent();
    findNext();
}

void ReplaceController::onPromptAllClicked()
{
    mDoPrompt = false;
    mReplacePrompt->hide();

    replaceCurrent();
    findNext();
}

ReplaceController::~ReplaceController() {}

#include "replacecontroller.moc"
