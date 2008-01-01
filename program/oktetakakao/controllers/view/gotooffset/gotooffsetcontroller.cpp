/***************************************************************************
                          gotooffsetcontroller.cpp  -  description
                             -------------------
    begin                : Thu Nov 23 2006
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


#include "gotooffsetcontroller.h"

// controller
#include "kgotooffsetdialog.h"
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
#include <KMessageBox>
// Qt
#include <QtGui/QAction>


// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
GotoOffsetController::GotoOffsetController( KXmlGuiWindow *MW )
 : MainWindow( MW ), ViewWidget( 0 ), ByteArray( 0 ), GotoOffsetDialog( 0 )
{
    KActionCollection* ActionCollection = MainWindow->actionCollection();

    GotoOffsetAction = ActionCollection->addAction( "goto_offset" );
    GotoOffsetAction->setText( i18n("&Goto Offset...") );
    connect( GotoOffsetAction, SIGNAL(triggered(bool) ), SLOT(gotoOffset()) );
    GotoOffsetAction->setShortcut( Qt::CTRL + Qt::Key_G );

    setView( 0 );
}

void GotoOffsetController::setView( KAbstractView *View )
{
    ViewWidget->disconnect( this );

    ViewWidget = View ? static_cast<KHEUI::KByteArrayView *>( View->widget() ) : 0;
    KByteArrayDocument *Document = View ? static_cast<KByteArrayDocument*>( View->document() ) : 0;
    ByteArray = Document ? Document->content() : 0;

    if( ByteArray )
    {
//         connect( ViewWidget, SIGNAL( selectionChanged( bool )), SLOT( onSelectionChanged( bool )) );
    }
    const bool HasView = ( ByteArray != 0 );
    GotoOffsetAction->setEnabled( HasView );
}


void GotoOffsetController::gotoOffset()
{
    // ensure dialog
    if( !GotoOffsetDialog )
    {
        GotoOffsetDialog = new KGotoOffsetDialog( MainWindow );
        const int StartOffset = ViewWidget->startOffset();
        GotoOffsetDialog->setRange( StartOffset, StartOffset+ByteArray->size()-1 );
        connect( GotoOffsetDialog, SIGNAL(okClicked()), SLOT(onOkClicked()) );
    }

    GotoOffsetDialog->show();
}


void GotoOffsetController::onOkClicked()
{
    GotoOffsetDialog->hide();

    const bool IsRelative = GotoOffsetDialog->isRelative();
    const int Offset = GotoOffsetDialog->offset();

    int NewPosition = IsRelative ?  ViewWidget->cursorPosition()+Offset :
                      Offset < 0 ? ByteArray->size()+Offset : Offset;
    ViewWidget->setCursorPosition( NewPosition );
}

GotoOffsetController::~GotoOffsetController()
{
}
