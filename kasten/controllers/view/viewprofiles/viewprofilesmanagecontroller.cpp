/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewprofilesmanagecontroller.h"

// Okteta Gui Kasten
#include <bytearrayviewprofilemanager.h>
// controller
#include "viewprofilesmanagedialog.h"
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KActionCollection>
#include <KAction>


namespace Kasten2
{

ViewProfilesManageController::ViewProfilesManageController( KXMLGUIClient* guiClient,
                                                            ByteArrayViewProfileManager* viewProfileManager,
                                                            QWidget* parentWidget )
  : mParentWidget( parentWidget ),
    mViewProfileManager( viewProfileManager )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mManageAction = actionCollection->addAction( QLatin1String("settings_profiles_manage"),
                                                 this, SLOT(manageProfiles()) );
    mManageAction->setText( i18nc("@action:inmenu",
                                  "Manage View Profiles...") );
}

void ViewProfilesManageController::setTargetModel( AbstractModel* model )
{
    Q_UNUSED( model );
}

void ViewProfilesManageController::manageProfiles()
{
    ViewProfilesManageDialog* dialog = new ViewProfilesManageDialog( mViewProfileManager, mParentWidget );
    dialog->exec();

    delete dialog;
}

}
