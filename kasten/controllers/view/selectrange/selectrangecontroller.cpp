/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "selectrangecontroller.h"

// controller
#include "selectrangetoolview.h"
#include "selectrangetool.h"
// Kasten gui
#include <toolinlineviewable.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KAction>
#include <KActionCollection>


namespace Kasten2
{

// TODO: a tool(view) might perhaps be invoked indirectly by asking for a tool for the object, here select
// so the status bar selection indicator can invoke the same tool, without knowing about it
SelectRangeController::SelectRangeController( If::ToolInlineViewable* toolInlineViewable, KXMLGUIClient* guiClient )
  : mToolInlineViewable( toolInlineViewable )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mSelectAction = actionCollection->addAction( QLatin1String("edit_select"),
                                                 this, SLOT(select()) );
    mSelectAction->setText( i18nc("@action:inmenu","&Select range...") );
    mSelectAction->setIcon( KIcon(QLatin1String("select-rectangular")) );
    mSelectAction->setShortcut( Qt::CTRL + Qt::Key_E );

    mTool = new SelectRangeTool();
    connect( mTool, SIGNAL(isUsableChanged(bool)),
             mSelectAction, SLOT(setEnabled(bool)) );
    mSelectAction->setEnabled( mTool->isUsable() );

    mView = new SelectRangeToolView( mTool );
}

void SelectRangeController::setTargetModel( AbstractModel* model )
{
    mTool->setTargetModel( model );
}


void SelectRangeController::select()
{
//     mView->activate(); // TODO: show would be better here, or should instead toolInlineViewable be asked?
    mToolInlineViewable->setCurrentToolInlineView( mView );
}


SelectRangeController::~SelectRangeController()
{
    delete mView;
    delete mTool;
}

}
