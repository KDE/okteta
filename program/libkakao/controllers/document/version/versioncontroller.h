/***************************************************************************
                          versioncontroller.h  -  description
                             -------------------
    begin                : Fri Feb 8 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef VERSIONCONTROLLER_H
#define VERSIONCONTROLLER_H


// kakao
#include <kviewcontroller.h>

namespace KDE { namespace If {
class Versionable;
}}
class KAbstractDocument;
class KXmlGuiWindow;
class KToolBarPopupAction;
class QAction;

class VersionController : public KViewController
{
  Q_OBJECT

  public:
    VersionController( KXmlGuiWindow *window );

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS: // action slots
    void onSetToOlderVersionTriggered();
    void onSetToNewerVersionTriggered();
    void onOlderVersionMenuAboutToShow();
    void onNewerVersionMenuAboutToShow();
    void onOlderVersionMenuTriggered( QAction *action );
    void onNewerVersionMenuTriggered( QAction *action );

    void onVersionIndexChanged( int versionIndex );

  protected:
    KAbstractDocument *mDocument;
    KDE::If::Versionable *mVersionControl;

    KToolBarPopupAction *mSetToOlderVersionAction;
    KToolBarPopupAction *mSetToNewerVersionAction;
};

#endif
