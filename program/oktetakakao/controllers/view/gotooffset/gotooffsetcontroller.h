/***************************************************************************
                          gotooffsetcontroller.h  -  description
                             -------------------
    begin                : Thu Nov 23 2006
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


#ifndef GOTOOFFSETCONTROLLER_H
#define GOTOOFFSETCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class QAction;
class KXmlGuiWindow;
class KViewManager;
namespace KHEUI {
class KByteArrayView;
}
namespace KHECore {
class KAbstractByteArrayModel;
}
class KGotoOffsetDialog;


class GotoOffsetController : public KViewController
{
  Q_OBJECT

  public:
    explicit GotoOffsetController( KXmlGuiWindow *MW );
    virtual ~GotoOffsetController();

  public: // KToolet API
    virtual void setView( KAbstractView *View );

  protected Q_SLOTS: // action slots
    void gotoOffset();

  private Q_SLOTS:
    void onOkClicked();

  protected:
    KXmlGuiWindow *MainWindow;
    KHEUI::KByteArrayView *ViewWidget;
    KHECore::KAbstractByteArrayModel *ByteArray;

    QAction *GotoOffsetAction;

    KGotoOffsetDialog *GotoOffsetDialog;
};

#endif
