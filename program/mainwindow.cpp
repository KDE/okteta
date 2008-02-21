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
#include <overwriteonly/overwriteonlycontroller.h>
#include <overwritemode/overwritemodecontroller.h>
#include <poddecoder/poddecodercontroller.h>
#include <bytetable/bytetablecontroller.h>
#include <info/infocontroller.h>
// #include <stringsextract/stringsextractcontroller.h>
#include <filter/filtercontroller.h>
#include <insertpattern/insertpatterncontroller.h>
#include <gotooffset/gotooffsetcontroller.h>
#include <search/searchcontroller.h>
#include <replace/replacecontroller.h>
#include <bookmarks/bookmarkscontroller.h>
#include <print/printcontroller.h>
#include <viewconfig/viewconfigcontroller.h>
// kakao controllers
#include <document/version/versioncontroller.h>
#include <document/versionview/versionviewcontroller.h>
#include <document/readonly/readonlycontroller.h>
#include <documentsystem/creator/creatorcontroller.h>
#include <documentsystem/loader/loadercontroller.h>
#include <io/setremote/setremotecontroller.h>
#include <io/synchronize/synchronizecontroller.h>
#include <view/zoom/zoomcontroller.h>
#include <view/select/selectcontroller.h>
#include <view/clipboard/clipboardcontroller.h>
#include <view/copyas/copyascontroller.h>
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
  mControllers.append( new CreatorController(mProgram->documentManager()->createManager(),this) );
  mControllers.append( new LoaderController(mProgram->documentManager()->syncManager(),this) );
  mControllers.append( new SetRemoteController(mProgram->documentManager()->syncManager(),this) );
  mControllers.append( new SynchronizeController(this) );
  mControllers.append( new CloseController(mProgram->viewManager(),this) );
  mControllers.append( new VersionController(this) );
  mControllers.append( new VersionViewController(this) );
  mControllers.append( new ReadOnlyController(this) );
  mControllers.append( new ViewListMenuController(mProgram->viewManager(),mTabbedViews,this) );
  mControllers.append( new FullScreenController(this) );
  mControllers.append( new QuitController(0,this) );

  mControllers.append( new ZoomController(this) );
  mControllers.append( new SelectController(this) );
  mControllers.append( new ClipboardController(this) );
  mControllers.append( new CopyAsController(mProgram->documentManager(),this) );

  mControllers.append( new OverwriteOnlyController(this) );
  mControllers.append( new OverwriteModeController(this) );
  mControllers.append( new PODDecoderController(this) );
  mControllers.append( new ByteTableController(this) );
  mControllers.append( new InfoController(this) );
//   mControllers.append( new StringsExtractController(this) );
  mControllers.append( new SearchController(this) );
  mControllers.append( new ReplaceController(this) );
  mControllers.append( new GotoOffsetController(this) );
  mControllers.append( new FilterController(this) );
  mControllers.append( new InsertPatternController(this) );
  mControllers.append( new BookmarksController(this) );
  mControllers.append( new PrintController(this) );
  mControllers.append( new ViewConfigController(this) );
}


OktetaMainWindow::~OktetaMainWindow()
{
}

#include "mainwindow.moc"
