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

#include "gotooffsetcontroller.h"

// controller
#include "kgotooffsetdialog.h"
// lib
#include <kbytearraydisplay.h>
#include <kbytearraydocument.h>
// Okteta core
#include <charcodec.h>
#include <kbytearraymodel.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KMessageBox>


// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
GotoOffsetController::GotoOffsetController( KXMLGUIClient* guiClient )
 : mByteArrayDisplay( 0 ), mByteArray( 0 ), mGotoOffsetDialog( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mGotoOffsetAction = actionCollection->addAction( "goto_offset" );
    mGotoOffsetAction->setText( i18nc("@action:inmenu","&Goto Offset...") );
    mGotoOffsetAction->setIcon( KIcon("go-jump") );
    mGotoOffsetAction->setShortcut( Qt::CTRL + Qt::Key_G );
    connect( mGotoOffsetAction, SIGNAL(triggered(bool) ), SLOT(gotoOffset()) );

    setTargetModel( 0 );
}

void GotoOffsetController::setTargetModel( AbstractModel* model )
{
//     if( mByteArrayDisplay ) mByteArrayDisplay->disconnect( this );

    mByteArrayDisplay = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;
    KByteArrayDocument* document =
        mByteArrayDisplay ? qobject_cast<KByteArrayDocument*>( mByteArrayDisplay->baseModel() ) : 0;
    mByteArray = document ? document->content() : 0;

    const bool hasView = ( mByteArray && mByteArrayDisplay );
    if( hasView )
    {
//         connect( mByteArrayDisplay, SIGNAL( selectionChanged( bool )), SLOT( onSelectionChanged( bool )) );
    }
    mGotoOffsetAction->setEnabled( hasView );
}


void GotoOffsetController::gotoOffset()
{
    // ensure dialog
    if( !mGotoOffsetDialog )
    {
        mGotoOffsetDialog = new KGotoOffsetDialog( 0 );
        const int startOffset = mByteArrayDisplay->startOffset();
        mGotoOffsetDialog->setRange( startOffset, startOffset+mByteArray->size()-1 );
        connect( mGotoOffsetDialog, SIGNAL(okClicked()), SLOT(onOkClicked()) );
    }

    mGotoOffsetDialog->show();
}


void GotoOffsetController::onOkClicked()
{
    mGotoOffsetDialog->hide();

    const bool IsRelative = mGotoOffsetDialog->isRelative();
    const int Offset = mGotoOffsetDialog->offset();
    const bool isSelectionToExtent = mGotoOffsetDialog->isSelectionToExtent();
    const bool isBackwards = mGotoOffsetDialog->isBackwards();

    const int newPosition =
        IsRelative ?
            ( isBackwards ? mByteArrayDisplay->cursorPosition() - Offset :
                            mByteArrayDisplay->cursorPosition() + Offset ) :
            ( isBackwards ? mByteArray->size() - Offset :
                            Offset );

    if( isSelectionToExtent )
        mByteArrayDisplay->setSelectionCursorPosition( newPosition );
    else
        mByteArrayDisplay->setCursorPosition( newPosition );
}

GotoOffsetController::~GotoOffsetController() {}
