/***************************************************************************
                          fullscreencontroller.h  -  description
                             -------------------
    begin                : Mon Oct 15 2007
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


#ifndef FULLSCREENCONTROLLER_H
#define FULLSCREENCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class KXmlGuiWindow;


class FullScreenController : public KViewController
{
  Q_OBJECT

  public:
    FullScreenController( KXmlGuiWindow *window );

  public: // KToolet API
    virtual void setView( KAbstractView *view );

  public Q_SLOTS:
    void switchFullScreen( bool toFullScreen );

  protected:
//     KProgram *Program;
    KXmlGuiWindow *mMainWindow;
};

#endif
