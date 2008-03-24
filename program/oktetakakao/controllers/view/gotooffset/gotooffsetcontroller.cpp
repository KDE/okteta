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

#include "gotooffsetcontroller.h"

// controller
#include "kgotooffsetdialog.h"
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
#include <KMessageBox>
// Qt
#include <QtGui/QAction>


// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
GotoOffsetController::GotoOffsetController( KXmlGuiWindow *MW )
 : MainWindow( MW ), ViewWidget( 0 ), ByteArray( 0 ), GotoOffsetDialog( 0 )
{
    KActionCollection* ActionCollection = MainWindow->actionCollection();

    GotoOffsetAction = ActionCollection->addAction( "goto_offset" );
    GotoOffsetAction->setText( i18nc("@action:inmenu","&Goto Offset...") );
    connect( GotoOffsetAction, SIGNAL(triggered(bool) ), SLOT(gotoOffset()) );
    GotoOffsetAction->setShortcut( Qt::CTRL + Qt::Key_G );

    setView( 0 );
}

void GotoOffsetController::setView( KAbstractView *View )
{
    if( ViewWidget ) ViewWidget->disconnect( this );

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

GotoOffsetController::~GotoOffsetController() {}
