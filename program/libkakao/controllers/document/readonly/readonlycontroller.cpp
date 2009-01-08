/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#include "readonlycontroller.h"

// Kakao core
#include <kabstractdocument.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KActionCollection>
#include <KToggleAction>


ReadOnlyController::ReadOnlyController( KXMLGUIClient* guiClient )
 : mDocument( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mSetReadOnlyAction = actionCollection->add<KToggleAction>( "isreadonly" );
    const QString text = i18nc( "@option:check set the document to read-only", "Set Read-only" );
    mSetReadOnlyAction->setText( text );
    mSetReadOnlyAction->setIcon( KIcon("object-unlocked.png") );
    const QString checkedText = i18nc( "@option:check set the document to read-write", "Set Read-write" );
    const KGuiItem checkedState( checkedText, KIcon("object-locked.png") );
    mSetReadOnlyAction->setCheckedState( checkedState );
    connect( mSetReadOnlyAction, SIGNAL(triggered(bool)), SLOT(setReadOnly(bool)) );

    setTargetModel( 0 );
}

void ReadOnlyController::setTargetModel( AbstractModel* model )
{
    if( mDocument ) mDocument->disconnect( mSetReadOnlyAction );

    mDocument = model ? model->findBaseModel<KAbstractDocument*>() : 0;

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
