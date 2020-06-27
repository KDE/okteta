/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "printcontrollerfactory.hpp"

// lib
#include "printcontroller.hpp"

namespace Kasten {

PrintControllerFactory::PrintControllerFactory() = default;

PrintControllerFactory::~PrintControllerFactory() = default;

AbstractXmlGuiController* PrintControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new PrintController(guiClient);
}

}
