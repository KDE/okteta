/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "toollistmenucontrollerfactory.hpp"

// lib
#include "toollistmenucontroller.hpp"

namespace Kasten {

ToolListMenuControllerFactory::ToolListMenuControllerFactory(If::WidgetsDockable* widgetsDockable)
    : m_widgetsDockable(widgetsDockable)
{
}

ToolListMenuControllerFactory::~ToolListMenuControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> ToolListMenuControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<ToolListMenuController>(m_widgetsDockable, guiClient);
}

}
