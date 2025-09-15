/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewcontextmenucontrollerfactory.hpp"

// lib
#include "viewcontextmenucontroller.hpp"

namespace Kasten {

ViewContextMenuControllerFactory::ViewContextMenuControllerFactory() = default;

ViewContextMenuControllerFactory::~ViewContextMenuControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> ViewContextMenuControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<ViewContextMenuController>(guiClient);
}

}
