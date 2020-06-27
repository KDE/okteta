/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BOOKMARKSTOOLVIEW_HPP
#define KASTEN_BOOKMARKSTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>

namespace Kasten {

class BookmarksView;
class BookmarksTool;

class BookmarksToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit BookmarksToolView(BookmarksTool* tool);
    ~BookmarksToolView() override;

public: // AbstractToolView API
    QWidget* widget() const override;
    QString title() const override;
    AbstractTool* tool() const override;

private:
    BookmarksView* mWidget;
};

}

#endif
