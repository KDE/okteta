/***************************************************************************
                          readonlycontroller.cpp  -  description
                             -------------------
    begin                : Sun Jan 27 2008
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


#include "readonlycontroller.h"

// kakao
#include <kviewmanager.h>
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KActionCollection>
#include <KToggleAction>


ReadOnlyController::ReadOnlyController( KXmlGuiWindow *window )
 : mDocument( 0 )
{
    KActionCollection *actionCollection = window->actionCollection();

    mSetReadOnlyAction = actionCollection->add<KToggleAction>( "isreadonly" );
    mSetReadOnlyAction->setText( i18n("Read only") );
    connect( mSetReadOnlyAction, SIGNAL(triggered(bool)), SLOT(setReadOnly(bool)) );

    setView( 0 );
}

void ReadOnlyController::setView( KAbstractView *view )
{
    if( mDocument ) mDocument->disconnect( mSetReadOnlyAction );

    mDocument = view ? view->document() : 0;

    if( mDocument )
    {
        mSetReadOnlyAction->setChecked( mDocument->isReadOnly() );

        connect( mDocument, SIGNAL(readOnlyChanged( bool )),
                 mSetReadOnlyAction, SLOT(setChecked( bool )) );
        connect( mDocument, SIGNAL(modifiableChanged( bool )),
                 mSetReadOnlyAction, SLOT(setEnabled( bool )) );
    }

    mSetReadOnlyAction->setEnabled( mDocument ? mDocument->isModifiable() : false );
}


void ReadOnlyController::setReadOnly( bool isReadOnly )
{
    mDocument->setReadOnly( isReadOnly );
}
