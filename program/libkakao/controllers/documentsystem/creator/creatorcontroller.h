/***************************************************************************
                          creatorcontroller.h  -  description
                             -------------------
    begin                : Fri Jun 2 2006
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


#ifndef CREATORCONTROLLER_H
#define CREATORCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class KXmlGuiWindow;
class KDocumentCreator;


class CreatorController : public KViewController
{
  Q_OBJECT

  public:
    CreatorController( KDocumentCreator *creator, KXmlGuiWindow *window );
    ~CreatorController();

  public: // KToolet API
    virtual void setView( KAbstractView *view );

  public Q_SLOTS:
    void createNew();

  protected:
    KDocumentCreator *mCreator;
    KXmlGuiWindow *mMainWindow;
};

#endif
