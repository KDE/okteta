/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacecontrollerfactory.hpp"

// lib
#include "replacecontroller.hpp"

namespace Kasten {

ReplaceControllerFactory::ReplaceControllerFactory(AbstractUserMessagesHandler* userMessagesHandler, QWidget* parentWidget)
    : m_userMessagesHandler(userMessagesHandler)
    , m_parentWidget(parentWidget)
{
}

ReplaceControllerFactory::~ReplaceControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> ReplaceControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<ReplaceController>(guiClient, m_userMessagesHandler, m_parentWidget);
}

}
