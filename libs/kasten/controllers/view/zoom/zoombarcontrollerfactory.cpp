/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "zoombarcontrollerfactory.hpp"

// lib
#include "zoombarcontroller.hpp"

namespace Kasten {

ZoomBarControllerFactory::ZoomBarControllerFactory(StatusBar* statusBar)
    : m_statusBar(statusBar)
{
}

ZoomBarControllerFactory::~ZoomBarControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> ZoomBarControllerFactory::create(KXMLGUIClient* guiClient) const
{
    Q_UNUSED(guiClient)

    return std::make_unique<ZoomBarController>(m_statusBar);
}

}
