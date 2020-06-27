/*
    This file is part of the Kasten Framework, made within the KDE community.

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

AbstractXmlGuiController* SwitchViewControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new SwitchViewController(m_groupedViews, guiClient);
}

}
