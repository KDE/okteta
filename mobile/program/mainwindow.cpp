/*
    This file is part of the Okteta Mobile program, made within the KDE community.

    Copyright 2006-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"

// program
#include "program.h"
// tools
// #include <stringsextract/stringsextracttoolview.h>
// #include <stringsextract/stringsextracttool.h>
// #include <info/infotoolview.h>
// #include <info/infotool.h>
// #include <filter/filtertoolview.h>
// #include <filter/filtertool.h>
// #include <checksum/checksumtoolview.h>
// #include <checksum/checksumtool.h>
// #include <info/documentinfotoolview.h>
// #include <info/documentinfotool.h>
// #include <poddecoder/poddecodertoolview.h>
// #include <poddecoder/poddecodertool.h>
// #include <bytetable/bytetabletoolview.h>
// #include <bytetable/bytetabletool.h>
// #include <bookmarks/bookmarkstoolview.h>
// #include <bookmarks/bookmarkstool.h>
// #include <structures/structtoolview.h>
// #include <structures/structtool.h>
// Kasten tools
// #include <document/versionview/versionviewtoolview.h>
// #include <document/versionview/versionviewtool.h>
// controllers
// #include <overwriteonly/overwriteonlycontroller.h>
// #include <overwritemode/overwritemodecontroller.h>
// #include <gotooffset/gotooffsetcontroller.h>
// #include <selectrange/selectrangecontroller.h>
// #include <search/searchcontroller.h>
// #include <replace/replacecontroller.h>
// #include <bookmarks/bookmarkscontroller.h>
// #include <print/printcontroller.h>
// #include <viewconfig/viewconfigcontroller.h>
// #include <viewmode/viewmodecontroller.h>
// #include <viewstatus/viewstatuscontroller.h>
// Mobile Kasten controllers
// #include <document/readonly/readonlycontroller.h>
// #include <documentsystem/creator/creatorcontroller.h>
#include <documentsystem/loader/loaderactioncontroller.h>
// #include <documentsystem/close/closecontroller.h>
// #include <io/setremote/setremotecontroller.h>
// #include <io/synchronize/synchronizecontroller.h>
// #include <io/clipboard/clipboardcontroller.h>
// #include <io/insert/insertcontroller.h>
// #include <io/copyas/copyascontroller.h>
// #include <io/export/exportcontroller.h>
// #include <view/version/versioncontroller.h>
// #include <view/zoom/zoomcontroller.h>
// #include <view/zoom/zoombarcontroller.h>
// #include <view/select/selectcontroller.h>
// #include <shellwindow/switchview/switchviewcontroller.h>
// #include <shellwindow/viewlistmenu/viewlistmenucontroller.h>
// #include <shellwindow/viewareasplit/viewareasplitcontroller.h>
#include <shellwindow/toollistmenu/toollistmenucontroller.h>
// #include <shellwindow/fullscreen/fullscreencontroller.h>
/*#include <viewsystem/close/closecontroller.h>*/
// Kasten gui
// #include <viewmanager.h>
// #include <tabbedviews.h>
// #include <multiviewareas.h>
// #include <statusbar.h>
// Kasten core
// #include <documentcreatemanager.h>
// #include <documentsyncmanager.h>
#include <documentmanager.h>
// KDE
#include <KMenuBar>
#include <KGlobal>
#include <KConfigGroup>


namespace Kasten2
{

static const char LoadedUrlsKey[] = "LoadedUrls";

OktetaMainWindow::OktetaMainWindow( OktetaProgram *program )
 : MobileShellWindow( program->documentManager()/*, program->viewManager()*/ ), mProgram( program )
{
    setObjectName( QLatin1String("Shell") );

//     setStatusBar( new Kasten2::StatusBar(this) );

    setupControllers();
//     setupGUI();

    // all controllers which use plugActionList have to do so after(!) setupGUI() or their entries will be removed
    // TODO: why is this so?
    addActionController( new ToolListMenuController(/*this,this*/) );
//     addXmlGuiController( new ViewListMenuController(mProgram->viewManager(),mGroupedViews,this) );
}

void OktetaMainWindow::setupControllers()
{
    // general, part of Kasten
//     addXmlGuiController( new CreatorController(mProgram->viewManager(),mProgram->documentManager(),this) );
    addActionController( new LoaderActionController(mProgram->documentManager()->syncManager(),menuBar()) );
#if 0
    addXmlGuiController( new SetRemoteController(mProgram->documentManager()->syncManager(),this) );
    addXmlGuiController( new SynchronizeController(mProgram->documentManager()->syncManager(),this) );
    addXmlGuiController( new ExportController(mProgram->viewManager(),mProgram->documentManager(),this) );
    addXmlGuiController( new CloseController(mProgram->documentManager(),this) );
    addXmlGuiController( new VersionController(this) );
    addXmlGuiController( new ReadOnlyController(this) );
    addXmlGuiController( new SwitchViewController(mGroupedViews,this) );
    addXmlGuiController( new ViewAreaSplitController(mProgram->viewManager(),mGroupedViews,this) );
    addXmlGuiController( new FullScreenController(this) );
    addXmlGuiController( new QuitController(0,this) );

    addXmlGuiController( new ZoomController(this) );
    addXmlGuiController( new SelectController(this) );
    addXmlGuiController( new ClipboardController(this) );
    addXmlGuiController( new InsertController(mProgram->viewManager(),mProgram->documentManager(),this) );
    addXmlGuiController( new CopyAsController(mProgram->viewManager(),mProgram->documentManager(),this) );

    addTool( new FileSystemBrowserToolView(new FileSystemBrowserTool( mProgram->documentManager() )) );
    addTool( new DocumentsToolView(new DocumentsTool( mProgram->documentManager() )) );
    addTool( new TerminalToolView(new TerminalTool( mProgram->documentManager() )) );
#ifndef NDEBUG
    addTool( new VersionViewToolView(new VersionViewTool()) );
#endif

    // Okteta specific
//     addXmlGuiController( new OverwriteOnlyController(this) );
    addXmlGuiController( new OverwriteModeController(this) );
    addXmlGuiController( new SearchController(this,this) );
    addXmlGuiController( new ReplaceController(this,this) );
    addXmlGuiController( new GotoOffsetController(mGroupedViews,this) );
    addXmlGuiController( new SelectRangeController(mGroupedViews,this) );
    addXmlGuiController( new BookmarksController(this) );
    addXmlGuiController( new PrintController(this) );
    addXmlGuiController( new ViewConfigController(this) );
    addXmlGuiController( new ViewModeController(this) );

    Kasten2::StatusBar* bottomBar = static_cast<Kasten2::StatusBar*>( statusBar() );
    addXmlGuiController( new ViewStatusController(bottomBar) );
    addXmlGuiController( new ModifiedBarController(bottomBar) );
    addXmlGuiController( new ReadOnlyBarController(bottomBar) );
    addXmlGuiController( new ZoomBarController(bottomBar) );

    addTool( new DocumentInfoToolView(new DocumentInfoTool(mProgram->documentManager()->syncManager())) );
    addTool( new ChecksumToolView(new ChecksumTool()) );
    addTool( new FilterToolView(new FilterTool()) );
    addTool( new StringsExtractToolView(new StringsExtractTool()) );
    addTool( new ByteTableToolView(new ByteTableTool()) );
    addTool( new InfoToolView(new InfoTool()) );
    addTool( new PODDecoderToolView(new PODDecoderTool()) );
    addTool( new StructToolView(new StructTool()) );
    addTool( new BookmarksToolView(new BookmarksTool()) );
#endif
}

void OktetaMainWindow::saveProperties( KConfigGroup &configGroup )
{
#if 0
    const QStringList urls = mDocumentManager->urls();
    configGroup.writePathEntry( LoadedUrlsKey, urls );
#endif
}

void OktetaMainWindow::readProperties( const KConfigGroup &configGroup )
{
#if 0
    const QStringList urls = configGroup.readPathEntry( LoadedUrlsKey, QStringList() );

    DocumentSyncManager* syncManager = mDocumentManager->syncManager();
    DocumentCreateManager* createManager = mDocumentManager->createManager();
    for( int i=0; i<urls.count(); ++i )
    {
        if( urls[i].isEmpty() )
            createManager->createNew();
        else
            syncManager->load( urls[i] );
        // TODO: set view to offset
        // if( offset != -1 )
    }
#endif
}


OktetaMainWindow::~OktetaMainWindow() {}

}
