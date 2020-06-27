/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewprofilesmanagecontroller.hpp"

// Okteta Gui Kasten
#include <Kasten/Okteta/ByteArrayViewProfileManager>
// controller
#include "viewprofilesmanagedialog.hpp"
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
// Qt
#include <QAction>

namespace Kasten {

ViewProfilesManageController::ViewProfilesManageController(KXMLGUIClient* guiClient,
                                                           ByteArrayViewProfileManager* viewProfileManager,
                                                           QWidget* parentWidget)
    : mParentWidget(parentWidget)
    , mViewProfileManager(viewProfileManager)
{
    mManageAction = new QAction(QIcon::fromTheme(QStringLiteral("configure")),
                                i18nc("@action:inmenu", "Manage View Profiles..."), this);
    connect(mManageAction, &QAction::triggered,
            this, &ViewProfilesManageController::manageProfiles);

    guiClient->actionCollection()->addAction(QStringLiteral("settings_viewprofiles_manage"), mManageAction);
}

void ViewProfilesManageController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model);
}

void ViewProfilesManageController::manageProfiles()
{
    auto* dialog = new ViewProfilesManageDialog(mViewProfileManager, mParentWidget);
    dialog->open();
}

}
