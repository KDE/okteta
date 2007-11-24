/***************************************************************************
                          copyascontroller.h  -  description
                             -------------------
    begin                : Thu Nov 22 2007
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


#ifndef COPYASCONTROLLER_H
#define COPYASCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class QAction;
class QActionGroup;
class KXmlGuiWindow;
class KDocumentManager;


class CopyAsController : public KViewController
{
  Q_OBJECT

  public:
    CopyAsController( KDocumentManager *documentManager, KXmlGuiWindow *window );

  public: // KToolet API
    virtual void setView( KAbstractView *view );

  private Q_SLOTS:
    void updateActions();
    void onActionTriggered( QAction *action );

  protected:
    KDocumentManager *mDocumentManager;
    KXmlGuiWindow *mMainWindow;
    KAbstractView *mView;

    QActionGroup *mCopyAsActionGroup;
};

#endif
