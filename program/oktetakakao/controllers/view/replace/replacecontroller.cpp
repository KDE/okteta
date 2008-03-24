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

#include "replacecontroller.h"

// controller
#include "kreplacedialog.h"
#include "kreplaceprompt.h"
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


// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
ReplaceController::ReplaceController( KXmlGuiWindow *MW )
 : MainWindow( MW ), ViewWidget( 0 ), ByteArray( 0 ), ReplaceDialog( 0 ), ReplacePrompt( 0 )
{
    KActionCollection* ActionCollection = MainWindow->actionCollection();

    ReplaceAction = KStandardAction::replace( this, SLOT(replace()), ActionCollection );

    setView( 0 );
}

void ReplaceController::setView( KAbstractView *view )
{
    if( ViewWidget ) ViewWidget->disconnect( this );

    ViewWidget = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;
    KByteArrayDocument *Document = view ? static_cast<KByteArrayDocument*>( view->document() ) : 0;
    ByteArray = Document ? Document->content() : 0;

    if( ByteArray )
    {
    }
    const bool HasView = ( ByteArray != 0 );
    ReplaceAction->setEnabled( HasView );
}


void ReplaceController::replace()
{
    // ensure dialog
    if( !ReplaceDialog )
    {
        ReplaceDialog = new KReplaceDialog( MainWindow );
        connect( ReplaceDialog, SIGNAL(okClicked()), SLOT(onDialogOkClicked()) );
    }

    ReplaceDialog->setInSelection( ViewWidget->hasSelectedData() );
    ReplaceDialog->setCharCode( ViewWidget->encodingName() );

    ReplaceDialog->show();
}

void ReplaceController::onDialogOkClicked()
{
    PreviousFound = false;
    NoOfReplacements = 0;

    ReplaceDialog->hide();

    // prepare settings
    SearchData = ReplaceDialog->data();
    ReplaceData = ReplaceDialog->replaceData();
    IgnoreCase = ReplaceDialog->ignoreCase();
    Prompt = ReplaceDialog->prompt();

    if( ReplaceDialog->inSelection() )
    {
        Direction = FindForward;
        const KHE::KSection Selection = ViewWidget->selection();
        ReplaceFirstIndex = Selection.start();
        ReplaceLastIndex =  Selection.end();
        CurrentIndex = Selection.start();
        DoWrap = true;
    }
    else
    {
        Direction = ReplaceDialog->direction();
        const int CursorPosition = ViewWidget->cursorPosition();
        if( ReplaceDialog->fromCursor() && (CursorPosition!=0) )
        {
            ReplaceFirstIndex = CursorPosition;
            ReplaceLastIndex =  CursorPosition-1;
        }
        else
        {
            ReplaceFirstIndex = 0;
            ReplaceLastIndex =  ByteArray->size()-1;
        }
        CurrentIndex = (Direction==FindForward) ? ReplaceFirstIndex : ReplaceLastIndex;
        DoWrap = (Direction==FindForward) ? (ReplaceLastIndex<CurrentIndex) : (CurrentIndex<ReplaceFirstIndex);
    }

    findNext();
}

void ReplaceController::findNext()
{
    while( true )
    {
        // TODO: support ignorecase
        int Pos = ( Direction == FindForward ) ?
            ByteArray->indexOf( SearchData, CurrentIndex ) :
            ByteArray->lastIndexOf( SearchData, CurrentIndex-SearchData.size()+1 );
        if( Pos != -1 )
        {
            PreviousFound = true;

            CurrentIndex = Pos;
            if( Prompt )
            {
                ViewWidget->setSelection( Pos, Pos+SearchData.size()-1 );
                if( !ReplacePrompt )
                {
                    ReplacePrompt = new KReplacePrompt( MainWindow );
                    connect( ReplacePrompt, SIGNAL(user1Clicked()), SLOT(onPromptAllClicked()) );
                    connect( ReplacePrompt, SIGNAL(user2Clicked()), SLOT(onPromptSkipClicked()) );
                    connect( ReplacePrompt, SIGNAL(user3Clicked()), SLOT(onPromptReplaceClicked()) );
                }
                if( ReplacePrompt->isHidden() )
                    ReplacePrompt->show();
             break;
           }
           else
           {
               replaceCurrent();
               continue;
           }
        }

        if( ReplacePrompt )
            ReplacePrompt->hide();
        ViewWidget->selectAll( false );

        const QString ReplacementReport = (NoOfReplacements==0) ?
            i18nc( "@info", "No replacements done.") :
            i18ncp( "@info", "1 replacement done.", "%1 replacements done.", NoOfReplacements );
        const QString messageBoxTitle = i18nc( "@title:window", "Replace" );
        // reached end and
        if( DoWrap )
        {
            const QString Question = ((Direction==FindForward) ?
                i18nc( "@info", "<nl/>End of byte array reached.<nl/>Continue from the beginning?" ) :
                i18nc( "@info", "<nl/>Beginning of byte array reached.<nl/>Continue from the end?" ));

            int Result = KMessageBox::questionYesNo( MainWindow, ReplacementReport+Question, messageBoxTitle,
                                                     KStandardGuiItem::cont(), KStandardGuiItem::cancel() );
            if( Result == KMessageBox::No )
                break;
            CurrentIndex = (Direction==FindForward) ? 0 : ByteArray->size()-1;
            DoWrap = false;
            NoOfReplacements = 0;
        }
        else
        {
            if( !PreviousFound )
                KMessageBox::sorry( MainWindow, i18nc("@info","Replace pattern not found in byte array."), messageBoxTitle );
            else
                KMessageBox::information( MainWindow, ReplacementReport, messageBoxTitle );
            break;
        }
    }
}

void ReplaceController::replaceCurrent()
{
    ++NoOfReplacements;
    const int Inserted = ByteArray->replace( CurrentIndex, SearchData.size(),
                                             ReplaceData.constData(), ReplaceData.size() );
    if( Direction == FindForward )
        CurrentIndex += Inserted;
    else
        --CurrentIndex;
}

void ReplaceController::onPromptSkipClicked()
{
    // skip current
    {
        if( Direction == FindForward )
            CurrentIndex += ReplaceData.size();
        else
            --CurrentIndex;
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
    Prompt = false;
    ReplacePrompt->hide();

    replaceCurrent();
    findNext();
}

ReplaceController::~ReplaceController()
{
}
