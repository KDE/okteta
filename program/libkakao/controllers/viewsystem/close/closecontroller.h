/***************************************************************************
                          closecontroller.h  -  description
                             -------------------
    begin                : Tue Nov 7 2006
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


#ifndef CLOSECONTROLLER_H
#define CLOSECONTROLLER_H


// kakao
#include <kviewcontroller.h>
// KDE
#include <KUrl>

class KViewManager;
class KAction;
class KXmlGuiWindow;


class CloseController : public KViewController
{
  Q_OBJECT

  public:
    CloseController( KViewManager *viewManager, KXmlGuiWindow *window );

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS:
    void close();

  protected:
    KViewManager *mViewManager;
    KXmlGuiWindow *mMainWindow;

    KAbstractView *mView;
    KAction *mCloseAction;
};

#endif
