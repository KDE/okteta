/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H


// libfinddialog
#include <kfinddirection.h>
// Kakao gui
#include <kviewcontroller.h>

class KXmlGuiWindow;
class KAction;
namespace KHEUI {
class KByteArrayView;
}
namespace KHECore {
class KAbstractByteArrayModel;
}
class KSearchDialog;


class SearchController : public KViewController
{
  Q_OBJECT

  public:
    SearchController( KXmlGuiWindow *window );
    virtual ~SearchController();

  public: // KToolet API
    virtual void setView( KAbstractView *View );

  protected:
    void showDialog( KFindDirection Direction );
    void searchData( KFindDirection Direction, int StartIndex );

  protected Q_SLOTS: // action slots
    void find();
    void findNext();
    void findPrevious();

  private Q_SLOTS:
    void onOkClicked();

  protected:
    KXmlGuiWindow *MainWindow;
    KHEUI::KByteArrayView *ViewWidget;
    KHECore::KAbstractByteArrayModel *ByteArray;

    KAction *mFindAction;
    KAction *mFindNextAction;
    KAction *mFindPrevAction;

    QByteArray SearchData;
    bool PreviousFound;
    bool IgnoreCase;
    int SearchFirstIndex;
    int SearchLastIndex;
    KSearchDialog *SearchDialog;
};

#endif
