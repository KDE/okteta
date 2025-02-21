/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "searchcontrollerfactory.hpp"

// lib
#include "searchcontroller.hpp"

namespace Kasten {

SearchControllerFactory::SearchControllerFactory(AbstractUserMessagesHandler* userMessagesHandler, QWidget* parentWidget)
    : m_userMessagesHandler(userMessagesHandler)
    , m_parentWidget(parentWidget)
{
}

SearchControllerFactory::~SearchControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> SearchControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<SearchController>(guiClient, m_userMessagesHandler, m_parentWidget);
}

}
