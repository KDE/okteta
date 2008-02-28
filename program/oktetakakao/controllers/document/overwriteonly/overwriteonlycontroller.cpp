/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "overwriteonlycontroller.h"

// Kakao gui
#include <kabstractview.h>
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
