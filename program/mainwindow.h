/***************************************************************************
                          mainwindow.h  -  description
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


#ifndef OKTETAMAINWINDOW_H
#define OKTETAMAINWINDOW_H


// kakao
#include <shellwindow.h>

class OktetaProgram;


class OktetaMainWindow : public ShellWindow
{
   Q_OBJECT

  public:
    explicit OktetaMainWindow( OktetaProgram *program );
    virtual ~OktetaMainWindow();

  protected: // KMainWindow API
    virtual void saveProperties( KConfigGroup &configGroup );
    virtual void readProperties( const KConfigGroup &configGroup );

  protected:
    void setupControllers();

  protected:
    OktetaProgram* mProgram;
};

#endif
