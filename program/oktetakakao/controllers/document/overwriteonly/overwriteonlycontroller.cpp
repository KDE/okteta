/***************************************************************************
                          overwriteonlycontroller.cpp  -  description
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


#include "overwriteonlycontroller.h"

// kakao
#include <kviewmanager.h>
// Okteta core
#include <kabstractbytearraymodel.h>
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KActionCollection>
#include <KToggleAction>


OverwriteOnlyController::OverwriteOnlyController( KXmlGuiWindow *window )
 : mByteArrayModel( 0 )
{
    KActionCollection *actionCollection = window->actionCollection();

    mSetOverwriteOnlyAction = actionCollection->add<KToggleAction>( "isoverwriteonly" );
    mSetOverwriteOnlyAction->setText( i18n("Overwrite only") );
    connect( mSetOverwriteOnlyAction, SIGNAL(triggered(bool)), SLOT(setOverwriteOnly(bool)) );

    setView( 0 );
}

void OverwriteOnlyController::setView( KAbstractView *view )
{
Q_UNUSED( view )
//     if( mByteArrayModel ) mByteArrayModel->disconnect( mSetOverwriteOnlyAction );

    mByteArrayModel = 0; //view ? view->document() : 0;

    if( mByteArrayModel )
    {
//         mSetOverwriteOnlyAction->setChecked( mByteArrayModel->isOverwriteOnly() );

//         connect( mByteArrayModel, SIGNAL(overwriteOnlyChanged( bool )),
//                  mSetOverwriteOnlyAction, SLOT(setChecked( bool )) );
//         connect( mByteArrayModel, SIGNAL(modifiableChanged( bool )),
//                  mSetOverwriteOnlyAction, SLOT(setEnabled( bool )) );
    }

    mSetOverwriteOnlyAction->setEnabled( false );//mByteArrayModel ? mByteArrayModel->isModifiable() : false );
}


void OverwriteOnlyController::setOverwriteOnly( bool isOverwriteOnly )
{
Q_UNUSED( isOverwriteOnly )
//     mByteArrayModel->setOverwriteOnly( isOverwriteOnly );
}
