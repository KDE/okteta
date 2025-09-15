/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "switchviewcontrollerfactory.hpp"

// lib
#include "switchviewcontroller.hpp"

namespace Kasten {

SwitchViewControllerFactory::SwitchViewControllerFactory(AbstractGroupedViews* groupedViews)
    : m_groupedViews(groupedViews)
{
}

SwitchViewControllerFactory::~SwitchViewControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> SwitchViewControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<SwitchViewController>(m_groupedViews, guiClient);
}

}
