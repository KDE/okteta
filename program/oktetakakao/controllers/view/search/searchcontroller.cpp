/***************************************************************************
                          searchcontroller.cpp  -  description
                             -------------------
    begin                : Mon Nov 13 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "searchcontroller.h"

// controller
#include "ksearchdialog.h"
//
#include <kbytearraydocument.h>
// kakao
#include <kviewmanager.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kcharcodec.h>
#include <kbytearraymodel.h>
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KMessageBox>


// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
SearchController::SearchController( KXmlGuiWindow *MW )
 : MainWindow( MW ), ViewWidget( 0 ), ByteArray( 0 ), SearchDialog( 0 )
{
    KActionCollection *actionCollection = MainWindow->actionCollection();

    mFindAction     = KStandardAction::find(     this, SLOT(find()),         actionCollection );
    mFindNextAction = KStandardAction::findNext( this, SLOT(findNext()),     actionCollection );
    mFindPrevAction = KStandardAction::findPrev( this, SLOT(findPrevious()), actionCollection );

    setView( 0 );
}

void SearchController::setView( KAbstractView *View )
{
    disconnect( ViewWidget );

    ViewWidget = View ? static_cast<KHEUI::KByteArrayView *>( View->widget() ) : 0;
    KByteArrayDocument *Document = View ? static_cast<KByteArrayDocument*>( View->document() ) : 0;
    ByteArray = Document ? Document->content() : 0;

    if( ByteArray )
    {
    }
    const bool HasView = ( ByteArray != 0 );
    mFindAction->setEnabled( HasView );
    mFindNextAction->setEnabled( HasView );
    mFindPrevAction->setEnabled( HasView );
}


void SearchController::find()
{
    showDialog( FindForward );
}

void SearchController::findNext()
{
    if( SearchData.isEmpty() )
        showDialog( FindForward );
    else
        searchData( FindForward, ViewWidget->cursorPosition() );
}

void SearchController::findPrevious()
{
    if( SearchData.isEmpty() )
        showDialog( FindBackward );
    else
    {
        int StartIndex = ViewWidget->cursorPosition()-SearchData.size()-1;
        searchData( FindBackward, StartIndex<0?0:StartIndex );
    }
}

void SearchController::showDialog( KFindDirection Direction )
{
    // ensure dialog
    if( !SearchDialog )
    {
        SearchDialog = new KSearchDialog( MainWindow );
        connect( SearchDialog, SIGNAL(okClicked()), SLOT(onOkClicked()) );
    }

    SearchDialog->setDirection( Direction );
    SearchDialog->setInSelection( ViewWidget->hasSelectedData() );
    SearchDialog->setCharCode( ViewWidget->encodingName() );

    SearchDialog->show();
}


void SearchController::onOkClicked()
{
    PreviousFound = false;

    SearchDialog->hide();

    SearchData = SearchDialog->data();
    IgnoreCase = SearchDialog->ignoreCase();
    KFindDirection Direction;
    int StartIndex;
    if( SearchDialog->inSelection() )
    {
        const KHE::KSection Selection = ViewWidget->selection();
        SearchFirstIndex = Selection.start();
        SearchLastIndex =  Selection.end();
        StartIndex = Selection.start();
        Direction = FindForward;
    }
    else
    {
        Direction = SearchDialog->direction();
        const int CursorPosition = ViewWidget->cursorPosition();
        if( SearchDialog->fromCursor() && (CursorPosition!=0) )
        {
            SearchFirstIndex = CursorPosition;
            SearchLastIndex =   CursorPosition-1;
        }
        else
        {
            SearchFirstIndex = 0;
            SearchLastIndex =  ByteArray->size()-1;
        }
        StartIndex = (Direction==FindForward) ? SearchFirstIndex : SearchLastIndex;
    }
    searchData( Direction, StartIndex );
}

void SearchController::searchData( KFindDirection Direction, int StartIndex )
{
    bool DoWrap = (Direction==FindForward) ? (SearchLastIndex<StartIndex) : (StartIndex<SearchFirstIndex);

    while( true )
    {
        // TODO: support ignorecase
        int Pos = ( Direction == FindForward ) ?
            ByteArray->indexOf( SearchData, StartIndex ) :
            ByteArray->lastIndexOf( SearchData, StartIndex-SearchData.size()+1 );
        if( Pos != -1 )
        {
            PreviousFound = true;
            ViewWidget->setSelection( Pos, Pos+SearchData.size()-1 );
            break;
        }

        if( DoWrap )
        {
            const QString Question = ( Direction == FindForward ) ?
                i18n( "End of document reached.\nContinue from the beginning?" ) :
                i18n( "Beginning of document reached.\nContinue from the end?" );

            int Result = KMessageBox::questionYesNo( MainWindow, Question, i18n("Find"),
                                                     KStandardGuiItem::cont(), KStandardGuiItem::cancel() );
            if( Result == KMessageBox::No )
                break;
            StartIndex = ( Direction == FindForward ) ? 0 : ByteArray->size()-1;
            DoWrap = false;
        }
        else
        {
            if( !PreviousFound )
                KMessageBox::sorry( MainWindow, i18n("Search key not found in document."), i18n("Find") );
            break;
        }
    }
}

SearchController::~SearchController() {}
