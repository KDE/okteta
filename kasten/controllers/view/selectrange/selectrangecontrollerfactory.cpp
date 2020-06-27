/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

AbstractXmlGuiController* SelectRangeControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new SelectRangeController(m_toolInlineViewable, guiClient);
}

}
