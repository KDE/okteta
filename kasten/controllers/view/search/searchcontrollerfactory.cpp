/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "searchcontrollerfactory.hpp"

// lib
#include "searchcontroller.hpp"

namespace Kasten {

SearchControllerFactory::SearchControllerFactory(QWidget* parentWidget)
    : m_parentWidget(parentWidget)
{
}

SearchControllerFactory::~SearchControllerFactory() = default;

AbstractXmlGuiController* SearchControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new SearchController(guiClient, m_parentWidget);
}

}
