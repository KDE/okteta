/***************************************************************************
                          versionviewcontroller.h  -  description
                             -------------------
    begin                : Sat Feb 9 2008
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


#ifndef VERSIONVIEWCONTROLLER_H
#define VERSIONVIEWCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class VersionView;
class KXmlGuiWindow;
class QAction;

class VersionViewController : public KViewController
{
  Q_OBJECT

  public:
    explicit VersionViewController( KXmlGuiWindow *window );
    virtual ~VersionViewController();

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected:
    VersionView *mView;
    QAction *mAction;
};

#endif
