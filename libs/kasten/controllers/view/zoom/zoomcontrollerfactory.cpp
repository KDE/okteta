/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "zoomcontrollerfactory.hpp"

// lib
#include "zoomcontroller.hpp"

namespace Kasten {

ZoomControllerFactory::ZoomControllerFactory() = default;

ZoomControllerFactory::~ZoomControllerFactory() = default;

AbstractXmlGuiController* ZoomControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new ZoomController(guiClient);
}

}
