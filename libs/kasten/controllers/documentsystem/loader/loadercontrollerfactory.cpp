/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "loadercontrollerfactory.hpp"

// lib
#include "loadercontroller.hpp"

namespace Kasten {

LoaderControllerFactory::LoaderControllerFactory(AbstractDocumentStrategy* documentStrategy)
    : m_documentStrategy(documentStrategy)
{
}

LoaderControllerFactory::~LoaderControllerFactory() = default;

AbstractXmlGuiController* LoaderControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return new LoaderController(m_documentStrategy, guiClient);
}

}
