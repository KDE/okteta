/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "selectrangecontrollerfactory.hpp"

// lib
#include "selectrangecontroller.hpp"

namespace Kasten {

SelectRangeControllerFactory::SelectRangeControllerFactory(If::ToolInlineViewable* toolInlineViewable)
    : m_toolInlineViewable(toolInlineViewable)
{
}

SelectRangeControllerFactory::~SelectRangeControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> SelectRangeControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<SelectRangeController>(m_toolInlineViewable, guiClient);
}

}
