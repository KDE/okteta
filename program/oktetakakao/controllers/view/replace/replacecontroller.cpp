/***************************************************************************
                          replacecontroller.cpp  -  description
                             -------------------
    begin                : Fri Nov 24 2006
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


#include "replacecontroller.h"

// controller
#include "kreplacedialog.h"
#include "kreplaceprompt.h"
//
#include <kbytearraydocument.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kcharcodec.h>
#include <kbytearraymodel.h>
// kakao
#include <kviewmanager.h>
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
    ViewWidget->disconnect( this );

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
            i18n( "No replacements done.") :
            i18np( "1 replacement done.", "%1 replacements done.", NoOfReplacements );
        // reached end and
        if( DoWrap )
        {
            const QString Question = ((Direction==FindForward) ?
                i18n( "\nEnd of document reached.\nContinue from the beginning?" ) :
                i18n( "\nBeginning of document reached.\nContinue from the end?" ));

            int Result = KMessageBox::questionYesNo( MainWindow, ReplacementReport+Question, i18n("Replace"),
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
                KMessageBox::sorry( MainWindow, i18n("Replace pattern not found in document."), i18n("Replace") );
            else
                KMessageBox::information( MainWindow, ReplacementReport, i18n("Replace") );
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
