/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BOOKMARKSCONTROLLER_H
#define BOOKMARKSCONTROLLER_H

// lib
#include "oktetakastencontrollers_export.h"
// Kasten gui
#include <abstractxmlguicontroller.h>
// Okteta core
#include <address.h>
// Qt
#include <QtCore/QList>

namespace Okteta {
class Bookmarkable;
class Bookmark;
class AbstractByteArrayModel;
}
class KXMLGUIClient;
class KAction;
class QActionGroup;
class QAction;


namespace Kasten
{

class KByteArrayDisplay;


class OKTETAKASTENCONTROLLERS_EXPORT BookmarksController : public AbstractXmlGuiController
{
  Q_OBJECT

  public:
    explicit BookmarksController( KXMLGUIClient* guiClient );
    virtual ~BookmarksController();

  public: // AbstractXmlGuiController API
    virtual void setTargetModel( AbstractModel* model );

  protected Q_SLOTS: // action slots
    void createBookmark();
    void deleteBookmark();
    void deleteAllBookmarks();
    void gotoNextBookmark();
    void gotoPreviousBookmark();

    void updateBookmarks();
    void onBookmarkTriggered( QAction* );

  private Q_SLOTS:
    void onBookmarksAdded( const QList<Okteta::Bookmark> &bookmarks );
    void onBookmarksRemoved( const QList<Okteta::Bookmark> &bookmarks );
    void onCursorPositionChanged( Okteta::Address newPosition );

  protected:
    KXMLGUIClient* mGuiClient;
    KByteArrayDisplay* mByteArrayDisplay;
    Okteta::AbstractByteArrayModel *mByteArray;
    Okteta::Bookmarkable *mBookmarks;

    KAction* mCreateAction;
    KAction* mDeleteAction;
    KAction* mDeleteAllAction;

    KAction* mGotoNextBookmarkAction;
    KAction* mGotoPreviousBookmarkAction;

    QActionGroup *mBookmarksActionGroup;
};

}

#endif
