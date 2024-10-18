/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "printcontrollerfactory.hpp"

// lib
#include "printcontroller.hpp"

namespace Kasten {

PrintControllerFactory::PrintControllerFactory(AbstractUserMessagesHandler* userMessagesHandler)
    : m_userMessagesHandler(userMessagesHandler)
{
}

PrintControllerFactory::~PrintControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> PrintControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<PrintController>(guiClient, m_userMessagesHandler);
}

}
