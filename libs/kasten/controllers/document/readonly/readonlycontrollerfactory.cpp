/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "readonlycontrollerfactory.hpp"

// lib
#include "readonlycontroller.hpp"

namespace Kasten {

ReadOnlyControllerFactory::ReadOnlyControllerFactory() = default;

ReadOnlyControllerFactory::~ReadOnlyControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> ReadOnlyControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<ReadOnlyController>(guiClient);
}

}
