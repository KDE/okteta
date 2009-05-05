/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "insertpatterncontroller.h"

// controller
#include "insertpatterndialog.h"
#include "insertpatterntool.h"
// KDE
#include <KActionCollection>
#include <KXMLGUIClient>
#include <KLocale>
#include <KMessageBox>
#include <KAction>


// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
InsertPatternController::InsertPatternController( KXMLGUIClient* guiClient )
 : mInsertPatternTool( new InsertPatternTool() ), mInsertPatternDialog( 0 )
{
    connect( mInsertPatternTool, SIGNAL(hasWriteableChanged( bool )), SLOT(onHasWriteableChanged( bool )) );

    KActionCollection *actionCollection = guiClient->actionCollection();

    mInsertPatternAction = actionCollection->addAction( "insert_pattern" );
    mInsertPatternAction->setText( i18nc("@action:inmenu","&Insert Pattern...") );
    connect( mInsertPatternAction, SIGNAL(triggered(bool) ), SLOT(onActionTriggered()));
    mInsertPatternAction->setShortcut( Qt::CTRL + Qt::Key_Insert );

    setTargetModel( 0 );
}

void InsertPatternController::setTargetModel( AbstractModel* model )
{
    mInsertPatternTool->setTargetModel( model );
}

void InsertPatternController::onHasWriteableChanged( bool hasWriteable )
{
    if( mInsertPatternDialog )
        mInsertPatternDialog->setHasWriteable( hasWriteable );

    mInsertPatternAction->setEnabled( hasWriteable );
}


void InsertPatternController::onActionTriggered()
{
    // ensure dialog
    if( !mInsertPatternDialog )
    {
        mInsertPatternDialog = new InsertPatternDialog( 0 );
        mInsertPatternDialog->setHasWriteable( mInsertPatternTool->hasWriteable() );
        connect( mInsertPatternDialog, SIGNAL(okClicked()), SLOT(onOkClicked()) );
    }
    mInsertPatternDialog->setCharCodec( mInsertPatternTool->charCodecName() );

    mInsertPatternDialog->show();
}

void InsertPatternController::onOkClicked()
{
    mInsertPatternDialog->hide();

    const QByteArray pattern = mInsertPatternDialog->pattern();
    const int patternNumber = mInsertPatternDialog->number();

    mInsertPatternTool->insertPattern( pattern, patternNumber );
}

InsertPatternController::~InsertPatternController()
{
    delete mInsertPatternDialog;
    delete mInsertPatternTool;
}
