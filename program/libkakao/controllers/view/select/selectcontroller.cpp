/***************************************************************************
                          selectcontroller.cpp  -  description
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


#include "selectcontroller.h"

//
#include <kidataselectable.h>
// kakao
#include <kviewmanager.h>
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>


SelectController::SelectController( KXmlGuiWindow *window )
 : mMainWindow( window ), mView( 0 ), mSelectControl( 0 )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    mSelectAllAction = KStandardAction::selectAll( this, SLOT(selectAll()), actionCollection );
    mDeselectAction =  KStandardAction::deselect(  this, SLOT(unselect()),  actionCollection );

    setView( 0 );
}

void SelectController::setView( KAbstractView *view )
{
    mView->disconnect( this );

    mView = view;
    mSelectControl = view ? qobject_cast<KDE::If::DataSelectable *>( view ) : 0;

    if( mSelectControl )
    {
        connect( mView, SIGNAL(hasSelectedDataChanged( bool )), SLOT(onHasSelectedDataChanged( bool )) );
    }

    const bool hasSelectionControl = ( mSelectControl != 0 );
    mSelectAllAction->setEnabled( hasSelectionControl );
    mDeselectAction->setEnabled( hasSelectionControl ? mSelectControl->hasSelectedData() : false );
}


void SelectController::onHasSelectedDataChanged( bool hasSelectedData )
{
    mDeselectAction->setEnabled( hasSelectedData );
}


void SelectController::selectAll()
{
    mSelectControl->selectAllData( true );
}

void SelectController::unselect()
{
    mSelectControl->selectAllData( false );
}
