/***************************************************************************
                          selectcontroller.h  -  description
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


#ifndef SELECTCONTROLLER_H
#define SELECTCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class KXmlGuiWindow;
class KAction;
class KViewManager;
namespace KDE { namespace If {
class DataSelectable;
} }


class SelectController : public KViewController
{
  Q_OBJECT

  public:
    explicit SelectController( KXmlGuiWindow *window );

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS: // action slots
    void selectAll();
    void unselect();

  private Q_SLOTS:
    void onHasSelectedDataChanged( bool hasSelectedData );

  protected:
    KXmlGuiWindow *mMainWindow;
    KAbstractView *mView;
    KDE::If::DataSelectable *mSelectControl;

    KAction *mSelectAllAction;
    KAction *mDeselectAction;
};

#endif
