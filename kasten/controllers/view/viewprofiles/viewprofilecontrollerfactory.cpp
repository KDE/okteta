/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewprofilecontrollerfactory.hpp"

// lib
#include "viewprofilecontroller.hpp"

namespace Kasten {

ViewProfileControllerFactory::ViewProfileControllerFactory(ByteArrayViewProfileManager* viewProfileManager,
                                                           QWidget* parentWidget)
    : m_viewProfileManager(viewProfileManager)
    , m_parentWidget(parentWidget)
{
}

ViewProfileControllerFactory::~ViewProfileControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> ViewProfileControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<ViewProfileController>(m_viewProfileManager, m_parentWidget, guiClient);
}

}
