/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewprofilesmanagecontrollerfactory.hpp"

// lib
#include "viewprofilesmanagecontroller.hpp"

namespace Kasten {

ViewProfilesManageControllerFactory::ViewProfilesManageControllerFactory(ByteArrayViewProfileManager* viewProfileManager,
                                                                         QWidget* parentWidget)
    : m_viewProfileManager(viewProfileManager)
    , m_parentWidget(parentWidget)
{
}

ViewProfilesManageControllerFactory::~ViewProfilesManageControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> ViewProfilesManageControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<ViewProfilesManageController>(guiClient, m_viewProfileManager, m_parentWidget);
}

}
