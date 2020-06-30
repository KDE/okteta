/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "overwritemodecontrollerfactory.hpp"

// lib
#include "overwritemodecontroller.hpp"

namespace Kasten {

OverwriteModeControllerFactory::OverwriteModeControllerFactory() = default;

OverwriteModeControllerFactory::~OverwriteModeControllerFactory() = default;

AbstractXmlGuiController* OverwriteModeControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new OverwriteModeController(guiClient);
}

}
