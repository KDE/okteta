/***************************************************************************
                          overwritemodecontroller.cpp  -  description
                             -------------------
    begin                : Fri Feb 8 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "overwritemodecontroller.h"

// kakao
#include <kviewmanager.h>
// Okteta gui
#include <kbytearrayview.h>
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KActionCollection>
#include <KToggleAction>


OverwriteModeController::OverwriteModeController( KXmlGuiWindow *window )
 : mByteArrayView( 0 )
{
    KActionCollection *actionCollection = window->actionCollection();

    mSetOverWriteAction = actionCollection->add<KToggleAction>( "set_overwrite" );
    mSetOverWriteAction->setText( i18n("Overwrite Mode") );
    mSetOverWriteAction->setWhatsThis(
        i18n("Choose whether you want the input to be inserted or to overwrite existing data.") );
    // TODO: or should we catch the signal from the view (needs to be added)
    mSetOverWriteAction->setShortcut( QKeySequence(Qt::Key_Insert) );
    connect( mSetOverWriteAction, SIGNAL(triggered(bool)), SLOT(setOverWrite(bool)) );

    setView( 0 );
}

void OverwriteModeController::setView( KAbstractView *view )
{
//     if( mByteArrayView ) mByteArrayView->disconnect( mSetOverWriteAction );

    mByteArrayView = view ? qobject_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    if( mByteArrayView )
    {
        mSetOverWriteAction->setChecked( mByteArrayView->isOverwriteMode() );

//         connect( mByteArrayView, SIGNAL(overwriteModeChanged( bool )),
//                  mSetOverWriteAction, SLOT(setChecked( bool )) );
        // TODO: catch if isOverwriteOnly changes
    }

    const bool canInsert = mByteArrayView && !mByteArrayView->isOverwriteOnly();
    mSetOverWriteAction->setEnabled( canInsert );
}


void OverwriteModeController::setOverWrite( bool isOverWrite )
{
    mByteArrayView->setOverwriteMode( isOverWrite );
}
