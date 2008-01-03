/***************************************************************************
                          infocontroller.h  -  description
                             -------------------
    begin                : Wed Jan 2 2008
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


#ifndef INFOCONTROLLER_H
#define INFOCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class InfoTool;
class InfoView;

class QAction;
class KXmlGuiWindow;
class KAbstractView;


class InfoController : public KViewController
{
  Q_OBJECT

  public:
    explicit InfoController( KXmlGuiWindow *window );
    virtual ~InfoController();

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected:
    InfoTool *mTool;
    InfoView *mView;
    QAction *mAction;
};

#endif
