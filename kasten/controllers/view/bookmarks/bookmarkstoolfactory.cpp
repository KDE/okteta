/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarkstoolfactory.hpp"

// lib
#include "bookmarkstool.hpp"

namespace Kasten {

BookmarksToolFactory::BookmarksToolFactory() = default;

BookmarksToolFactory::~BookmarksToolFactory() = default;

AbstractTool* BookmarksToolFactory::create() const
{
    return new BookmarksTool();
}

}
