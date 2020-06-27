/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewmodecontrollerfactory.hpp"

// lib
#include "viewmodecontroller.hpp"

namespace Kasten {

ViewModeControllerFactory::ViewModeControllerFactory() = default;

ViewModeControllerFactory::~ViewModeControllerFactory() = default;

AbstractXmlGuiController* ViewModeControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new ViewModeController(guiClient);
}

}
