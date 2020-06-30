/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "closecontrollerfactory.hpp"

// lib
#include "closecontroller.hpp"

namespace Kasten {

CloseControllerFactory::CloseControllerFactory(AbstractDocumentStrategy* documentStrategy)
    : m_documentStrategy(documentStrategy)
{
}

CloseControllerFactory::~CloseControllerFactory() = default;

AbstractXmlGuiController* CloseControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new CloseController(m_documentStrategy, guiClient);
}

}
