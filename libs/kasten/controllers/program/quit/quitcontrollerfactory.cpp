/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "quitcontrollerfactory.hpp"

// lib
#include "quitcontroller.hpp"

namespace Kasten {

QuitControllerFactory::QuitControllerFactory(KXmlGuiWindow* window)
    : m_window(window)
{
}

QuitControllerFactory::~QuitControllerFactory() = default;

AbstractXmlGuiController* QuitControllerFactory::create(KXMLGUIClient* guiClient) const
{
    Q_UNUSED(guiClient);

    return new QuitController(m_window);
}

}
