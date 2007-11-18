/***************************************************************************
                          searchcontroller.h  -  description
                             -------------------
    begin                : Mon Nov 13 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H


// libfinddialog
#include <kfinddirection.h>
// kakao
#include <kviewcontroller.h>

class KXmlGuiWindow;
class KAction;
class KViewManager;
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
    KHEUI::KByteArrayView *HexEdit;
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
