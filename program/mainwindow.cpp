/***************************************************************************
                          mainwindow.cpp  -  description
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


#include "mainwindow.h"

// program
#include "program.h"
// controllers
#include <poddecoder/poddecodercontroller.h>
// #include <chartable/chartablecontroller.h>
// #include <statistic/statisticcontroller.h>
#include <insertpattern/insertpatterncontroller.h>
#include <gotooffset/gotooffsetcontroller.h>
#include <search/searchcontroller.h>
#include <replace/replacecontroller.h>
#include <viewconfig/viewconfigcontroller.h>
// kakao controllers
#include <documentsystem/creator/creatorcontroller.h>
#include <documentsystem/loader/loadercontroller.h>
#include <document/setremote/setremotecontroller.h>
#include <document/synchronize/synchronizecontroller.h>
#include <view/zoom/zoomcontroller.h>
#include <view/select/selectcontroller.h>
#include <view/clipboard/clipboardcontroller.h>
#include <shellwindow/viewlistmenu/viewlistmenucontroller.h>
#include <shellwindow/fullscreen/fullscreencontroller.h>
#include <viewsystem/close/closecontroller.h>
#include <program/quit/quitcontroller.h>
// kakao
#include <kdocumentmanager.h>
#include <kviewmanager.h>
#include <tabbedviews.h>


OktetaMainWindow::OktetaMainWindow( OktetaProgram *program )
 : ShellWindow( program->documentManager(), program->viewManager() ), mProgram( program )
{
    setupControllers();
    setupGUI();
}

void OktetaMainWindow::setupControllers()
{
  mControllers.append( new CreatorController(mProgram->documentManager()->creator(),this) );
  mControllers.append( new LoaderController(mProgram->documentManager()->loaderManager(),this) );
  mControllers.append( new SetRemoteController(mProgram->documentManager()->loaderManager(),this) );
  mControllers.append( new SynchronizeController(this) );
  mControllers.append( new CloseController(mProgram->viewManager(),this) );
  mControllers.append( new ViewListMenuController(mProgram->viewManager(),mTabbedViews,this) );
  mControllers.append( new FullScreenController(this) );
  mControllers.append( new QuitController(0,this) );

  mControllers.append( new ZoomController(this) );
  mControllers.append( new SelectController(this) );
  mControllers.append( new ClipboardController(this) );

  mControllers.append( new PODDecoderController(mProgram->viewManager(),this) );
//   mControllers.append( new CharTableController(this) );
//   mControllers.append( new StatisticController(this) );
  mControllers.append( new SearchController(this) );
  mControllers.append( new ReplaceController(this) );
  mControllers.append( new GotoOffsetController(this) );
  mControllers.append( new InsertPatternController(this) );
  mControllers.append( new ViewConfigController(this) );
}


OktetaMainWindow::~OktetaMainWindow()
{
}

#include "mainwindow.moc"
