/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modifiedbarcontrollerfactory.hpp"

// lib
#include "modifiedbarcontroller.hpp"

namespace Kasten {

ModifiedBarControllerFactory::ModifiedBarControllerFactory(StatusBar* statusBar)
    : m_statusBar(statusBar)
{
}

ModifiedBarControllerFactory::~ModifiedBarControllerFactory() = default;

AbstractXmlGuiController* ModifiedBarControllerFactory::create(KXMLGUIClient* guiClient) const
{
    Q_UNUSED(guiClient);

    return new ModifiedBarController(m_statusBar);
}

}
