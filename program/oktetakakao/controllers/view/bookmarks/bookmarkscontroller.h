/***************************************************************************
                          bookmarkscontroller.h  -  description
                             -------------------
    begin                : Thu Nov 1 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef BOOKMARKSCONTROLLER_H
#define BOOKMARKSCONTROLLER_H


// kakao
#include <kviewcontroller.h>
// Qt
#include <QtCore/QList>

class QAction;
class QActionGroup;
class KXmlGuiWindow;
class KViewManager;
namespace KHEUI {
class KByteArrayView;
}
namespace KHECore {
class KBookmark;
class KAbstractByteArrayModel;
}


class BookmarksController : public KViewController
{
  Q_OBJECT

  public:
    explicit BookmarksController( KXmlGuiWindow *window );
    virtual ~BookmarksController();

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected:
    void updateBookmarks();

  protected Q_SLOTS: // action slots
    void createBookmark();
    void deleteBookmark();
    void deleteAllBookmarks();
    void gotoNextBookmark();
    void gotoPreviousBookmark();

    void onBookmarkTriggered( QAction* );

  private Q_SLOTS:
    void onBookmarksAdded( const QList<KHECore::KBookmark*> &bookmarks );
    void onBookmarksRemoved( const QList<KHECore::KBookmark*> &bookmarks );
    void onCursorPositionChanged( int newPosition );

  protected:
    KXmlGuiWindow *mWindow;
    KHEUI::KByteArrayView *mByteArrayView;
    KHECore::KAbstractByteArrayModel *mByteArray;

    QAction *mCreateAction;
    QAction *mDeleteAction;
    QAction *mDeleteAllAction;

    QAction *mGotoNextBookmarkAction;
    QAction *mGotoPreviousBookmarkAction;

    QActionGroup *mBookmarksActionGroup;
};

#endif
