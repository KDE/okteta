/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "closecontrollerfactory.hpp"

// lib
#include "closecontroller.hpp"

namespace Kasten {

CloseControllerFactory::CloseControllerFactory(ViewManager* viewManager)
    : m_viewManager(viewManager)
{
}

CloseControllerFactory::~CloseControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> CloseControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<CloseController>(m_viewManager, guiClient);
}

}
