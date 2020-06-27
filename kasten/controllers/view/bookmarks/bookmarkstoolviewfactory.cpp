/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarkstoolviewfactory.hpp"

// lib
#include "bookmarkstoolview.hpp"
#include "bookmarkstool.hpp"
// KF
#include <KLocalizedString>

namespace Kasten {

BookmarksToolViewFactory::BookmarksToolViewFactory() = default;

BookmarksToolViewFactory::~BookmarksToolViewFactory() = default;

QString BookmarksToolViewFactory::iconName() const { return QStringLiteral("bookmarks"); }
QString BookmarksToolViewFactory::title()       const { return i18nc("@title:window", "Bookmarks"); }
QString BookmarksToolViewFactory::id()       const { return QStringLiteral("org.kde.okteta.BookmarksToolView"); }
SidePosition BookmarksToolViewFactory::defaultPosition() const { return LeftSidePosition; }

AbstractToolView* BookmarksToolViewFactory::create(AbstractTool* tool) const
{
    return new BookmarksToolView(qobject_cast<BookmarksTool*>(tool));
}

}
