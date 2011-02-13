/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "fullscreencontroller.h"

// KDE
#include <KToggleFullScreenAction>
#include <KXmlGuiWindow>
#include <KActionCollection>


namespace Kasten
{

FullScreenController::FullScreenController( KXmlGuiWindow *window )
: mMainWindow( window )
{
    KActionCollection *actionCollection = mMainWindow->actionCollection();

    KToggleFullScreenAction *fullScreenAction = new KToggleFullScreenAction( actionCollection );
    fullScreenAction->setWindow( window );
    fullScreenAction->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_F11 );
    actionCollection->addAction( QLatin1String("view_fullscreen"), fullScreenAction );
    connect( fullScreenAction , SIGNAL(toggled(bool)) , SLOT(switchFullScreen(bool)) ); 
}

void FullScreenController::setTargetModel( AbstractModel* model )
{
Q_UNUSED( model )
}

void FullScreenController::switchFullScreen( bool toFullScreen )
{
    KToggleFullScreenAction::setFullScreen( mMainWindow, toFullScreen );
}

}
