/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewstatuscontrollerfactory.hpp"

// lib
#include "viewstatuscontroller.hpp"

namespace Kasten {

ViewStatusControllerFactory::ViewStatusControllerFactory(StatusBar* statusBar)
    : m_statusBar(statusBar)
{
}

ViewStatusControllerFactory::~ViewStatusControllerFactory() = default;

AbstractXmlGuiController* ViewStatusControllerFactory::create(KXMLGUIClient* guiClient) const
{
    Q_UNUSED(guiClient);

    return new ViewStatusController(m_statusBar);
}

}
