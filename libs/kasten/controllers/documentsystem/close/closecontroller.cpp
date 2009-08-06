/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "closecontroller.h"

// Kasten core
#include <documentmanager.h>
// KDE
#include <KActionCollection>
#include <KAction>
#include <KStandardAction>
#include <KXMLGUIClient>
#include <KLocale>
#include <KIcon>


namespace Kasten
{

CloseController::CloseController( DocumentManager* documentManager, KXMLGUIClient* guiClient )
: mDocumentManager( documentManager )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mCloseAction  = KStandardAction::close(  this, SLOT(close()),  actionCollection );

    mCloseAllAction = actionCollection->addAction( "file_close_all" );
    mCloseAllAction->setText( i18nc("@title:menu","Close All") );
    mCloseAllAction->setIcon( KIcon("window-close") );
    connect( mCloseAllAction, SIGNAL(triggered( bool )), SLOT(closeAll()) );

    setTargetModel( 0 );
}

void CloseController::setTargetModel( AbstractModel* model )
{
    mDocument = model ? model->findBaseModel<AbstractDocument*>() : 0;
    const bool hasDocument = ( mDocument != 0 );

    mCloseAction->setEnabled( hasDocument );
}


void CloseController::close()
{
    if( mDocumentManager->canClose(mDocument) )
        mDocumentManager->closeDocument( mDocument );
}


void CloseController::closeAll()
{
    if( mDocumentManager->canCloseAll() )
        mDocumentManager->closeAll();
}
}
