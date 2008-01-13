/***************************************************************************
                          filtercontroller.h  -  description
                             -------------------
    begin                : Fri Jan 11 2008
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


#ifndef FILTERCONTROLLER_H
#define FILTERCONTROLLER_H


// kakao
#include <kviewcontroller.h>

class KXmlGuiWindow;
class FilterTool;
class FilterView;
class QAction;


class FilterController : public KViewController
{
  Q_OBJECT

  public:
    explicit FilterController( KXmlGuiWindow *window );
    virtual ~FilterController();

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  protected:
    void loadFilter();

  protected:
    FilterTool *mTool;
    FilterView *mView;
};

#endif
