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

#include "overwritemodecontroller.h"

// lib
#include <kbytearraydisplay.h>
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KActionCollection>
#include <KToggleAction>


OverwriteModeController::OverwriteModeController( KXMLGUIClient* guiClient )
 : mByteArrayDisplay( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mSetOverWriteAction = actionCollection->add<KToggleAction>( "set_overwrite" );
    mSetOverWriteAction->setText( i18nc("@option:check","Overwrite Mode") );
    mSetOverWriteAction->setWhatsThis(
        i18nc("@info:whatsthis",
              "Choose whether you want the input to be inserted or to overwrite existing data.") );
    // TODO: or should we catch the signal from the view (needs to be added)
    mSetOverWriteAction->setShortcut( QKeySequence(Qt::Key_Insert) );
    connect( mSetOverWriteAction, SIGNAL(triggered(bool)), SLOT(setOverWrite(bool)) );

    setTargetModel( 0 );
}

void OverwriteModeController::setTargetModel( AbstractModel* model )
{
//     if( mByteArrayDisplay ) mByteArrayDisplay->disconnect( mSetOverWriteAction );

    mByteArrayDisplay = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;

    if( mByteArrayDisplay )
    {
        mSetOverWriteAction->setChecked( mByteArrayDisplay->isOverwriteMode() );

//         connect( mByteArrayDisplay, SIGNAL(overwriteModeChanged( bool )),
//                  mSetOverWriteAction, SLOT(setChecked( bool )) );
        // TODO: catch if isOverwriteOnly changes
    }

    const bool canInsert = mByteArrayDisplay && !mByteArrayDisplay->isOverwriteOnly();
    mSetOverWriteAction->setEnabled( canInsert );
}


void OverwriteModeController::setOverWrite( bool isOverWrite )
{
    mByteArrayDisplay->setOverwriteMode( isOverWrite );
}
