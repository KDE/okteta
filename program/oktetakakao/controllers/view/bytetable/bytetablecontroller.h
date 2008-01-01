/***************************************************************************
                          bytetablecontroller.h  -  description
                             -------------------
    begin                : Mon Dec 31 2007
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


#ifndef BYTETABLECONTROLLER_H
#define BYTETABLECONTROLLER_H


// kakao
#include <kviewcontroller.h>

class ByteTableTool;
class ByteTableView;

class QAction;
class KXmlGuiWindow;
class KAbstractView;


class ByteTableController : public KViewController
{
  Q_OBJECT

  public:
    ByteTableController( KXmlGuiWindow *window );
    virtual ~ByteTableController();

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected:
    ByteTableTool *mTool;
    ByteTableView *mView;
    QAction *mAction;
};

#endif
