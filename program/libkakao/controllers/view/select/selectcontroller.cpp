/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#include "selectcontroller.h"

// Kakao gui
#include <kidataselectable.h>
#include <kabstractview.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>


SelectController::SelectController( KXMLGUIClient* guiClient )
 : mView( 0 ), mSelectControl( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mSelectAllAction = KStandardAction::selectAll( this, SLOT(selectAll()), actionCollection );
    mDeselectAction =  KStandardAction::deselect(  this, SLOT(unselect()),  actionCollection );

    setView( 0 );
}

void SelectController::setView( KAbstractView *view )
{
    if( mView ) mView->disconnect( this );

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
