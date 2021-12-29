/*
    This file is part of the Kasten Framework, made within the KDE community.

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

AbstractXmlGuiController* ViewAreaContextMenuControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new ViewAreaContextMenuController(m_multiViewAreas, m_syncManager, guiClient);
}

}
