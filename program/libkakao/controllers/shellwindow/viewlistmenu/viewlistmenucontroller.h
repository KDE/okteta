/***************************************************************************
                          viewlistmenucontroller.h  -  description
                             -------------------
    begin                : Tue Nov 30 2006
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


#ifndef VIEWLISTMENUCONTROLLER_H
#define VIEWLISTMENUCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class QMenu;
class QAction;
class QActionGroup;
class KXmlGuiWindow;
class KViewManager;

namespace KDE { namespace If {
class ViewFocusable;
}}

class ViewListMenuController : public KViewController
{
  Q_OBJECT

  public:
    ViewListMenuController( KViewManager *viewManager, KDE::If::ViewFocusable *focusable, KXmlGuiWindow *window );

  public: // KToolet API
    virtual void setView( KAbstractView *view );

  private Q_SLOTS:
    void updateActions();
    void onActionTriggered( QAction *action );

  protected:
    KViewManager *mViewManager;
    KDE::If::ViewFocusable *mFocusable;
    KXmlGuiWindow *mMainWindow;

    QMenu *mWindowsMenu;
    QActionGroup *mWindowsActionGroup;
};

#endif
