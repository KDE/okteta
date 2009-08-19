/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "modifiedbarcontroller.h"

// Kasten ui
#include <statusbar.h>
// KDE
#include <KLocale>
#include <KIcon>
// Qt
#include <QtGui/QLabel>


namespace Kasten
{

ModifiedBarController::ModifiedBarController( StatusBar* statusBar )
  : mDocument( 0 )
{
    mModifiedLabel = new QLabel( statusBar );
    mModifiedLabel->setAlignment( Qt::AlignCenter );
    statusBar->addWidget( mModifiedLabel );

    setTargetModel( 0 );
}


void ModifiedBarController::setTargetModel( AbstractModel* model )
{
    if( mDocument ) mDocument->disconnect( this );

    mDocument = model ? model->findBaseModel<AbstractDocument*>() : 0;

    AbstractDocument::SyncStates syncStates;
    if( mDocument )
    {
        syncStates = mDocument->syncStates();
        connect( mDocument, SIGNAL(syncStatesChanged( Kasten::AbstractDocument::SyncStates )),
                 SLOT(onSyncStatesChanged( Kasten::AbstractDocument::SyncStates )) );
    }
    else
        syncStates = AbstractDocument::InSync;

    onSyncStatesChanged( syncStates );
    mModifiedLabel->setEnabled( mDocument );
}


void ModifiedBarController::onSyncStatesChanged( Kasten::AbstractDocument::SyncStates syncStates )
{
    // TODO: handle changed on remote
    const bool isModified = (syncStates & AbstractDocument::LocalHasChanges );

    // TODO: depend an statusbar height
    mModifiedLabel->setPixmap( isModified ? KIcon("document-save").pixmap(16) : QPixmap() );

    mModifiedLabel->setToolTip( isModified ?
        i18nc( "@tooltip the document is modified", "Modified." ) :
        i18nc( "@tooltip the document is not modified", "Not modified." ) );

}

}
