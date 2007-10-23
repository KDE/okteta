/***************************************************************************
                          quitcontroller.h  -  description
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


#ifndef QUITCONTROLLER_H
#define QUITCONTROLLER_H


// lib
#include <kviewcontroller.h>
// KDE
#include <KUrl>

class KXmlGuiWindow;
class KProgram;


class QuitController : public KViewController
{
  Q_OBJECT

  public:
    QuitController( KProgram *program, KXmlGuiWindow *window );

  public: // KToolet API
    virtual void setView( KAbstractView *view );

  public Q_SLOTS:
    void quit();

  protected:
//     KProgram *Program;
    KXmlGuiWindow *mMainWindow;
};

#endif
