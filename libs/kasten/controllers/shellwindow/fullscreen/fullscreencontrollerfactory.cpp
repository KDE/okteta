/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "fullscreencontrollerfactory.hpp"

// lib
#include "fullscreencontroller.hpp"

namespace Kasten {

FullScreenControllerFactory::FullScreenControllerFactory(KXmlGuiWindow* window)
    : m_window(window)
{
}

FullScreenControllerFactory::~FullScreenControllerFactory() = default;

AbstractXmlGuiController* FullScreenControllerFactory::create(KXMLGUIClient* guiClient) const
{
    Q_UNUSED(guiClient);

    return new FullScreenController(m_window);
}

}
