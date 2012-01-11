/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "searchcontroller.h"

// controller
#include "ksearchdialog.h"
#include "searchtool.h"
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KMessageBox>


namespace Kasten2
{

// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
SearchController::SearchController( KXMLGUIClient* guiClient, QWidget* parentWidget )
  : mParentWidget( parentWidget ),
    mSearchDialog( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mFindAction     = KStandardAction::find(     this, SLOT(find()),         actionCollection );
    mFindNextAction = KStandardAction::findNext( this, SLOT(findNext()),     actionCollection );
    mFindPrevAction = KStandardAction::findPrev( this, SLOT(findPrevious()), actionCollection );

    mTool = new SearchTool();
    mTool->setUserQueryAgent( this );

    connect( mTool, SIGNAL(isApplyableChanged(bool)),
             mFindAction, SLOT(setEnabled(bool)) );
    connect( mTool, SIGNAL(isApplyableChanged(bool)),
             mFindNextAction, SLOT(setEnabled(bool)) );
    connect( mTool, SIGNAL(isApplyableChanged(bool)),
             mFindPrevAction, SLOT(setEnabled(bool)) );

    connect( mTool, SIGNAL(dataNotFound()), SLOT(onDataNotFound()) );

    mFindAction->setEnabled( false );
    mFindNextAction->setEnabled( false );
    mFindPrevAction->setEnabled( false );
}

void SearchController::setTargetModel( AbstractModel* model )
{
    mTool->setTargetModel( model );
}

void SearchController::find()
{
    showDialog( FindForward );
}

void SearchController::findNext()
{
    if( mTool->searchData().isEmpty() )
        showDialog( FindForward );
    else
        mTool->search( FindForward, true, false );
;
}

void SearchController::findPrevious()
{
    if( mTool->searchData().isEmpty() )
        showDialog( FindBackward );
    else
        mTool->search( FindBackward, true, false );
}

void SearchController::showDialog( KFindDirection direction )
{
    // ensure dialog
    if( !mSearchDialog )
        mSearchDialog = new KSearchDialog( mTool, mParentWidget );

    mSearchDialog->setDirection( direction );

    mSearchDialog->show();
}

void SearchController::onDataNotFound()
{
    const QString messageBoxTitle = i18nc( "@title:window", "Find" );
    KMessageBox::sorry( mParentWidget, i18nc("@info","Search key not found in byte array."), messageBoxTitle );
}

bool SearchController::queryContinue( KFindDirection direction ) const
{
    const QString messageBoxTitle = i18nc( "@title:window", "Find" );
    const QString question = ( direction == FindForward ) ?
        i18nc( "@info", "End of byte array reached.<nl/>Continue from the beginning?" ) :
        i18nc( "@info", "Beginning of byte array reached.<nl/>Continue from the end?" );

    const int answer = KMessageBox::questionYesNo( mParentWidget, question, messageBoxTitle,
                                                   KStandardGuiItem::cont(), KStandardGuiItem::cancel() );

    const bool result = ( answer != KMessageBox::No );

    return result;
}

SearchController::~SearchController()
{
    delete mSearchDialog;
    delete mTool;
}

}
