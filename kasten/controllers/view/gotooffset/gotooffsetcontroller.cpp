/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "gotooffsetcontroller.h"

// controller
#include "gotooffsettoolview.h"
#include "gotooffsettool.h"
// Kasten gui
#include <toolinlineviewable.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KAction>
#include <KActionCollection>


namespace Kasten2
{

// TODO: for docked widgets signal widgets if embedded or floating, if horizontal/vertical
GotoOffsetController::GotoOffsetController( If::ToolInlineViewable* toolInlineViewable, KXMLGUIClient* guiClient )
 : mToolInlineViewable( toolInlineViewable )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mGotoOffsetAction = actionCollection->addAction( QLatin1String("goto_offset"),
                                                     this, SLOT(gotoOffset()) );
    mGotoOffsetAction->setText( i18nc("@action:inmenu","&Go to Offset...") );
    mGotoOffsetAction->setIcon( KIcon(QLatin1String("go-jump")) );
    mGotoOffsetAction->setShortcut( Qt::CTRL + Qt::Key_G );

    mTool = new GotoOffsetTool();
    connect( mTool, SIGNAL(isUsableChanged(bool)),
             mGotoOffsetAction, SLOT(setEnabled(bool)) );
    mGotoOffsetAction->setEnabled( mTool->isUsable() );

    mView = new GotoOffsetToolView( mTool );
}

void GotoOffsetController::setTargetModel( AbstractModel* model )
{
    mTool->setTargetModel( model );
}


void GotoOffsetController::gotoOffset()
{
    mToolInlineViewable->setCurrentToolInlineView( mView );
}


GotoOffsetController::~GotoOffsetController()
{
    delete mView;
    delete mTool;
}

}
