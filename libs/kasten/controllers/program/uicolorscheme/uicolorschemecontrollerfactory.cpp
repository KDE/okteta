/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uicolorschemecontrollerfactory.hpp"

// lib
#include "uicolorschemecontroller.hpp"

namespace Kasten {

UiColorSchemeControllerFactory::UiColorSchemeControllerFactory(KXmlGuiWindow* window)
    : m_window(window)
{
}

UiColorSchemeControllerFactory::~UiColorSchemeControllerFactory() = default;

AbstractXmlGuiController* UiColorSchemeControllerFactory::create(KXMLGUIClient* guiClient) const
{
    Q_UNUSED(guiClient);

    return new UiColorSchemeController(m_window);
}

}
