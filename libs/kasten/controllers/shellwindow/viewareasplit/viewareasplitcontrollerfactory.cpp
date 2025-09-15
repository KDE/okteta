/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewareasplitcontrollerfactory.hpp"

// lib
#include "viewareasplitcontroller.hpp"

namespace Kasten {

ViewAreaSplitControllerFactory::ViewAreaSplitControllerFactory(ViewManager* viewManager,
                                                               AbstractGroupedViews* groupedViews)
    : m_viewManager(viewManager)
    , m_groupedViews(groupedViews)
{
}

ViewAreaSplitControllerFactory::~ViewAreaSplitControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> ViewAreaSplitControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<ViewAreaSplitController>(m_viewManager, m_groupedViews, guiClient);
}

}
