/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BOOKMARKSTOOLVIEW_HPP
#define KASTEN_BOOKMARKSTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>
// Std
#include <memory>

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
    [[nodiscard]]
    QWidget* widget() const override;
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    AbstractTool* tool() const override;

private:
    const std::unique_ptr<BookmarksView> mWidget;
};

}

#endif
