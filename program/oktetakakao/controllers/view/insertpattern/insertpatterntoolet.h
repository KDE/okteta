/***************************************************************************
                          insertpatterntoolet.h  -  description
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


#ifndef INSERTPATTERNTOOLET_H
#define INSERTPATTERNTOOLET_H


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
class KInsertPatternDialog;


class InsertPatternController : public KViewController
{
  Q_OBJECT

  public:
    explicit InsertPatternController( KXmlGuiWindow *MW );
    virtual ~InsertPatternController();

  public: // KToolet API
    virtual void setView( KAbstractView *View );

  protected Q_SLOTS: // action slots
    void insertPattern();

  private Q_SLOTS:
    void onOkClicked();

  protected:
    KXmlGuiWindow *MainWindow;
    KHEUI::KByteArrayView *HexEdit;
    KHECore::KAbstractByteArrayModel *ByteArray;

    QAction *InsertPatternAction;

    KInsertPatternDialog *InsertPatternDialog;
};

#endif
