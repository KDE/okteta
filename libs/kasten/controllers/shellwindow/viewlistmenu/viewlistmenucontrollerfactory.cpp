/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewlistmenucontrollerfactory.hpp"

// lib
#include "viewlistmenucontroller.hpp"

namespace Kasten {

ViewListMenuControllerFactory::ViewListMenuControllerFactory(ViewManager* viewManager,
                                                             AbstractGroupedViews* groupedViews)
    : m_viewManager(viewManager)
    , m_groupedViews(groupedViews)
{
}

ViewListMenuControllerFactory::~ViewListMenuControllerFactory() = default;

AbstractXmlGuiController* ViewListMenuControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new ViewListMenuController(m_viewManager, m_groupedViews, guiClient);
}

}
