/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include "searchjob.h"
// lib
#include <kbytearraydocument.h>
// Kakao gui
#include <kabstractview.h>
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
// Qt
#include <QtGui/QApplication>


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
    if( ViewWidget ) ViewWidget->disconnect( this );

    ViewWidget = View ? static_cast<KHEUI::KByteArrayView *>( View->widget() ) : 0;
    KByteArrayDocument* Document = View ? static_cast<KByteArrayDocument*>( View->baseModel() ) : 0;
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
    SearchDialog->setCharCodec( ViewWidget->encodingName() );

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

        QApplication::setOverrideCursor( Qt::WaitCursor );

        SearchJob *searchJob = new SearchJob( ByteArray, SearchData, StartIndex, (Direction==FindForward) );
        const int Pos = searchJob->exec();

        QApplication::restoreOverrideCursor();

        if( Pos != -1 )
        {
            PreviousFound = true;
            ViewWidget->setSelection( Pos, Pos+SearchData.size()-1 );
            break;
        }

        const QString messageBoxTitle = i18nc( "@title:window", "Find" );
        if( DoWrap )
        {
            const QString Question = ( Direction == FindForward ) ?
                i18nc( "@info", "End of byte array reached.<nl/>Continue from the beginning?" ) :
                i18nc( "@info", "Beginning of byte array reached.<nl/>Continue from the end?" );

            int Result = KMessageBox::questionYesNo( MainWindow, Question, messageBoxTitle,
                                                     KStandardGuiItem::cont(), KStandardGuiItem::cancel() );
            if( Result == KMessageBox::No )
                break;
            StartIndex = ( Direction == FindForward ) ? 0 : ByteArray->size()-1;
            DoWrap = false;
        }
        else
        {
            if( !PreviousFound )
                KMessageBox::sorry( MainWindow, i18nc("@info","Search key not found in byte array."), messageBoxTitle );
            break;
        }
    }
}

SearchController::~SearchController() {}

#include "searchcontroller.moc"
