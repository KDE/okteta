/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "selectcontrollerfactory.hpp"

// lib
#include "selectcontroller.hpp"

namespace Kasten {

SelectControllerFactory::SelectControllerFactory() = default;

SelectControllerFactory::~SelectControllerFactory() = default;

AbstractXmlGuiController* SelectControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new SelectController(guiClient);
}

}
