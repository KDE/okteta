/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "clipboardcontrollerfactory.hpp"

// lib
#include "clipboardcontroller.hpp"

namespace Kasten {

ClipboardControllerFactory::ClipboardControllerFactory() = default;

ClipboardControllerFactory::~ClipboardControllerFactory() = default;

AbstractXmlGuiController* ClipboardControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new ClipboardController(guiClient);
}

}
