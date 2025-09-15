/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bookmarkstoolview.hpp"

// tool
#include "bookmarksview.hpp"
#include "bookmarkstool.hpp"

namespace Kasten {

BookmarksToolView::BookmarksToolView(BookmarksTool* tool)
    : mWidget(std::make_unique<BookmarksView>(tool))
{
}

BookmarksToolView::~BookmarksToolView() = default;

QWidget* BookmarksToolView::widget()    const { return mWidget.get(); }
QString BookmarksToolView::title()      const { return mWidget->tool()->title(); }
AbstractTool* BookmarksToolView::tool() const { return mWidget->tool(); }

}

#include "moc_bookmarkstoolview.cpp"
