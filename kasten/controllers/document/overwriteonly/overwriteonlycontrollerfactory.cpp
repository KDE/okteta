/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "overwriteonlycontrollerfactory.hpp"

// lib
#include "overwriteonlycontroller.hpp"

namespace Kasten {

OverwriteOnlyControllerFactory::OverwriteOnlyControllerFactory() = default;

OverwriteOnlyControllerFactory::~OverwriteOnlyControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> OverwriteOnlyControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<OverwriteOnlyController>(guiClient);
}

}
