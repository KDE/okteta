/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "readonlybarcontrollerfactory.hpp"

// lib
#include "readonlybarcontroller.hpp"

namespace Kasten {

ReadOnlyBarControllerFactory::ReadOnlyBarControllerFactory(StatusBar* statusBar)
    : m_statusBar(statusBar)
{
}

ReadOnlyBarControllerFactory::~ReadOnlyBarControllerFactory() = default;

AbstractXmlGuiController* ReadOnlyBarControllerFactory::create(KXMLGUIClient* guiClient) const
{
    Q_UNUSED(guiClient);

    return new ReadOnlyBarController(m_statusBar);
}

}
