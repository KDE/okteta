/*
    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarkscontrollerfactory.hpp"

// lib
#include "bookmarkscontroller.hpp"

namespace Kasten {

BookmarksControllerFactory::BookmarksControllerFactory() = default;

BookmarksControllerFactory::~BookmarksControllerFactory() = default;

std::unique_ptr<AbstractXmlGuiController> BookmarksControllerFactory::create(KXMLGUIClient* guiClient) const
{
    return std::make_unique<BookmarksController>(guiClient);
}

}
