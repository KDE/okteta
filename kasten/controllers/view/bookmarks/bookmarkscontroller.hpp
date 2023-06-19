/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BOOKMARKSCONTROLLER_HPP
#define KASTEN_BOOKMARKSCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
// Okteta core
#include <Okteta/Address>

template <class T> class QVector;

namespace Okteta {
class Bookmarkable;
class Bookmark;
class AbstractByteArrayModel;
}
class KXMLGUIClient;
class KAction;
class QActionGroup;
class QAction;

namespace Kasten {

class ByteArrayView;

class BookmarksController : public AbstractXmlGuiController
{
    Q_OBJECT

private:
    static constexpr int MaxEntryLength = 150;
    static constexpr int MaxBookmarkNameSize = 40;

public:
    explicit BookmarksController(KXMLGUIClient* guiClient);
    ~BookmarksController() override;

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS: // action slots
    void createBookmark();
    void deleteBookmark();
    void deleteAllBookmarks();
    void gotoNextBookmark();
    void gotoPreviousBookmark();

    void updateBookmarks();
    void onBookmarkTriggered(QAction*);

private Q_SLOTS: // exectute slots
    void addBookmark(int cursorPosition, const QString& name);

private Q_SLOTS:
    void onBookmarksAdded(const QVector<Okteta::Bookmark>& bookmarks);
    void onBookmarksRemoved(const QVector<Okteta::Bookmark>& bookmarks);
    void onCursorPositionChanged(Okteta::Address newPosition);

private:
    KXMLGUIClient* mGuiClient;

    ByteArrayView* mByteArrayView = nullptr;
    Okteta::AbstractByteArrayModel* mByteArray = nullptr;
    Okteta::Bookmarkable* mBookmarks = nullptr;

    QAction* mCreateAction;
    QAction* mDeleteAction;
    QAction* mDeleteAllAction;

    QAction* mGotoNextBookmarkAction;
    QAction* mGotoPreviousBookmarkAction;

    QActionGroup* mBookmarksActionGroup;
};

}

#endif
