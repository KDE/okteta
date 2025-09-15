/*
    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewareacontextmenucontrollerfactory.hpp"

// lib
#include "viewareacontextmenucontroller.hpp"

namespace Kasten {

ViewAreaContextMenuControllerFactory::ViewAreaContextMenuControllerFactory(MultiViewAreas* multiViewAreas,
                                                                           DocumentSyncManager* syncManager)
    : m_multiViewAreas(multiViewAreas)
    , m_syncManager(syncManager)
{
}

ViewAreaContextMenuControllerFactory::~ViewAreaContextMenuControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> ViewAreaContextMenuControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<ViewAreaContextMenuController>(m_multiViewAreas, m_syncManager, guiClient);
}

}
