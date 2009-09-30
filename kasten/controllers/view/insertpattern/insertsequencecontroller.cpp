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

#include "insertsequencecontroller.h"

// controller
#include "insertsequencetool.h"
// KDE
#include <KActionCollection>
#include <KXMLGUIClient>
#include <KLocale>
#include <KAction>


namespace Kasten
{

InsertSequenceController::InsertSequenceController( KXMLGUIClient* guiClient )
  : mInsertSequenceTool( new InsertSequenceTool() )
{
    connect( mInsertSequenceTool, SIGNAL(hasWriteableChanged( bool )), SLOT(onHasWriteableChanged( bool )) );

    KActionCollection* actionCollection = guiClient->actionCollection();

    mInsertSequenceAction = actionCollection->addAction( "insert_sequence" );
    mInsertSequenceAction->setText( i18nc("@action:inmenu","Insert Complete Byte Sequence") );
    connect( mInsertSequenceAction, SIGNAL(triggered(bool) ), SLOT(onActionTriggered()));

    setTargetModel( 0 );
}

void InsertSequenceController::setTargetModel( AbstractModel* model )
{
    mInsertSequenceTool->setTargetModel( model );
}

void InsertSequenceController::onHasWriteableChanged( bool hasWriteable )
{
    mInsertSequenceAction->setEnabled( hasWriteable );
}

void InsertSequenceController::onActionTriggered()
{
    mInsertSequenceTool->insertSequence();
}

InsertSequenceController::~InsertSequenceController()
{
    delete mInsertSequenceTool;
}

}
