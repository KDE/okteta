/*
    This file is part of the Okteta program, part of the KDE project.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <stringsextract/stringsextracttoolview.h>
#include <stringsextract/stringsextracttool.h>
#include <info/infotoolview.h>
#include <info/infotool.h>
#include <filter/filtertoolview.h>
#include <filter/filtertool.h>
#include <checksum/checksumtoolview.h>
#include <checksum/checksumtool.h>
#include <info/documentinfotoolview.h>
#include <info/documentinfotool.h>
#include <poddecoder/poddecodertoolview.h>
#include <poddecoder/poddecodertool.h>
#include <bytetable/bytetabletoolview.h>
#include <bytetable/bytetabletool.h>
#include <bookmarks/bookmarkstoolview.h>
#include <bookmarks/bookmarkstool.h>
// Kasten tools
#include <document/versionview/versionviewtoolview.h>
#include <document/versionview/versionviewtool.h>
#include <documentsystem/filesystembrowser/filesystembrowsertoolview.h>
#include <documentsystem/filesystembrowser/filesystembrowsertool.h>
#include <documentsystem/documentsbrowser/documentstoolview.h>
#include <documentsystem/documentsbrowser/documentstool.h>
// controllers
#include <overwriteonly/overwriteonlycontroller.h>
#include <overwritemode/overwritemodecontroller.h>
#include <insertpattern/insertpatterncontroller.h>
#include <gotooffset/gotooffsetcontroller.h>
#include <selectrange/selectrangecontroller.h>
#include <search/searchcontroller.h>
#include <replace/replacecontroller.h>
#include <bookmarks/bookmarkscontroller.h>
#include <print/printcontroller.h>
#include <viewconfig/viewconfigcontroller.h>
#include <viewmode/viewmodecontroller.h>
#include <viewstatus/viewstatuscontroller.h>
// Kasten controllers
#include <document/readonly/readonlycontroller.h>
#include <document/readonly/readonlybarcontroller.h>
#include <documentsystem/creator/creatorcontroller.h>
#include <documentsystem/loader/loadercontroller.h>
#include <documentsystem/close/closecontroller.h>
#include <io/setremote/setremotecontroller.h>
#include <io/synchronize/synchronizecontroller.h>
#include <io/clipboard/clipboardcontroller.h>
#include <io/copyas/copyascontroller.h>
#include <io/export/exportcontroller.h>
#include <view/version/versioncontroller.h>
#include <view/zoom/zoomcontroller.h>
#include <view/zoom/zoombarcontroller.h>
#include <view/select/selectcontroller.h>
#include <shellwindow/switchview/switchviewcontroller.h>
#include <shellwindow/viewlistmenu/viewlistmenucontroller.h>
#include <shellwindow/toollistmenu/toollistmenucontroller.h>
#include <shellwindow/fullscreen/fullscreencontroller.h>
/*#include <viewsystem/close/closecontroller.h>*/
#include <program/quit/quitcontroller.h>
// Kasten gui
#include <viewmanager.h>
#include <tabbedviews.h>
// #include <parallelviews/parallelviews.h>
#include <statusbar.h>
// Kasten core
#include <documentcreatemanager.h>
#include <documentsyncmanager.h>
#include <documentmanager.h>
// KDE
#include <KGlobal>
#include <KConfigGroup>


namespace Kasten
{

static const char LoadedUrlsKey[] = "LoadedUrls";

OktetaMainWindow::OktetaMainWindow( OktetaProgram *program )
 : ShellWindow( program->documentManager(), program->viewManager() ), mProgram( program )
{
    setObjectName( QLatin1String("Shell") );

    // XXX: Workaround for Qt 4.4's lacking of proper handling of the initial layout of dock widgets
    //      This state is taken from an oktetarc where the docker constellation was configured by hand.
    //      Setting this state if none is present seems to work, but there's
    //      still the versioning problem to be accounted for.
    //      Hack borrowed from trunk/koffice/krita/ui/kis_view2.cpp:
    const QString mainWindowState =
"AAAA/wAAAAD9AAAAAgAAAAAAAADPAAACOPwCAAAAAfwAAABFAAACOAAAAH4BAAAg+gAAAAEBAAAAAvsAAAASAEQAbwBjAHUAbQBlAG4AdABzAQAAAAD/////AAAATwEAAAX7AAAAIgBGAGkAbABlAFMAeQBzAHQAZQBtAEIAcgBvAHcAcwBlAHIBAAAAAAAAAM8AAABqAQAABQAAAAEAAAE2AAACOPwCAAAAA/sAAAAQAFYAZQByAHMAaQBvAG4AcwAAAABNAAAAVgAAAF0BAAAC/AAAAEUAAAEYAAABGAEAACD6AAAAAAEAAAAG+wAAABIAQgBvAG8AawBtAGEAcgBrAHMBAAAAAP////8AAADiAQAABfsAAAAUAFAATwBEAEQAZQBjAG8AZABlAHIBAAAAAP////8AAAE2AQAABfsAAAAYAEIAaQBuAGEAcgB5AEYAaQBsAHQAZQByAQAAAAD/////AAAA5AEAAAX7AAAADgBTAHQAcgBpAG4AZwBzAQAAAAD/////AAABFgEAAAX7AAAAEgBCAHkAdABlAFQAYQBiAGwAZQAAAAAA/////wAAAS8BAAAF+wAAAAgASQBuAGYAbwEAAAGRAAABTAAAAGoBAAAF/AAAAWAAAAEdAAABHQEAACD6AAAAAQEAAAAC+wAAABgARABvAGMAdQBtAGUAbgB0AEkAbgBmAG8BAAAAAP////8AAACGAQAABfsAAAAQAEMAaABlAGMAawBzAHUAbQEAAAHUAAABNgAAAOQBAAAFAAABOQAAAjgAAAAEAAAABAAAAAgAAAAI/AAAAAEAAAACAAAAAQAAABYAbQBhAGkAbgBUAG8AbwBsAEIAYQByAQAAAAAAAANsAAAAAAAAAAA=";

    const char mainWindowStateKey[] = "State";
    KConfigGroup group( KGlobal::config(), "MainWindow" );
    if( !group.hasKey(mainWindowStateKey) )
        group.writeEntry( mainWindowStateKey, mainWindowState );

    setStatusBar( new Kasten::StatusBar(this) );

    setupControllers();
    setupGUI();

    // all controllers which use plugActionList have to do so after(!) setupGUI() or their entries will be removed
    // TODO: why is this so?
    addXmlGuiController( new ToolListMenuController(this,this) );
    addXmlGuiController( new ViewListMenuController(mProgram->viewManager(),mGroupedViews,this) );
}

void OktetaMainWindow::setupControllers()
{
    // general, part of Kasten
    addXmlGuiController( new CreatorController(mProgram->documentManager()->createManager(),this) );
    addXmlGuiController( new LoaderController(mProgram->documentManager()->syncManager(),this) );
    addXmlGuiController( new SetRemoteController(mProgram->documentManager()->syncManager(),this) );
    addXmlGuiController( new SynchronizeController(this) );
    addXmlGuiController( new ExportController(mProgram->viewManager(),mProgram->documentManager(),this) );
    addXmlGuiController( new CloseController(mProgram->documentManager(),this) );
    addXmlGuiController( new VersionController(this) );
    addXmlGuiController( new ReadOnlyController(this) );
    addXmlGuiController( new SwitchViewController(mGroupedViews,this) );
    addXmlGuiController( new FullScreenController(this) );
    addXmlGuiController( new QuitController(0,this) );

    addXmlGuiController( new ZoomController(this) );
    addXmlGuiController( new SelectController(this) );
    addXmlGuiController( new ClipboardController(this) );
    addXmlGuiController( new CopyAsController(mProgram->viewManager(),mProgram->documentManager(),this) );

    addTool( new FileSystemBrowserToolView(new FileSystemBrowserTool( mProgram->documentManager() )) );
    addTool( new DocumentsToolView(new DocumentsTool( mProgram->documentManager() )) );
#ifndef NDEBUG
    addTool( new VersionViewToolView(new VersionViewTool()) );
#endif

    // Okteta specific
//     addXmlGuiController( new OverwriteOnlyController(this) );
    addXmlGuiController( new OverwriteModeController(this) );
    addXmlGuiController( new SearchController(this) );
    addXmlGuiController( new ReplaceController(this) );
    addXmlGuiController( new GotoOffsetController(mGroupedViews,this) );
    addXmlGuiController( new SelectRangeController(mGroupedViews,this) );
    addXmlGuiController( new InsertPatternController(this) );
    addXmlGuiController( new BookmarksController(this) );
    addXmlGuiController( new PrintController(this) );
    addXmlGuiController( new ViewConfigController(this) );
    addXmlGuiController( new ViewModeController(this) );

    Kasten::StatusBar* bottomBar = static_cast<Kasten::StatusBar*>( statusBar() );
    addXmlGuiController( new ViewStatusController(bottomBar) );
    addXmlGuiController( new ReadOnlyBarController(bottomBar) );
    addXmlGuiController( new ZoomBarController(bottomBar) );

    addTool( new DocumentInfoToolView(new DocumentInfoTool(mProgram->documentManager()->syncManager())) );
    addTool( new ChecksumToolView(new ChecksumTool()) );
    addTool( new FilterToolView(new FilterTool()) );
    addTool( new StringsExtractToolView(new StringsExtractTool()) );
    addTool( new ByteTableToolView(new ByteTableTool()) );
    addTool( new InfoToolView(new InfoTool()) );
    addTool( new PODDecoderToolView(new PODDecoderTool()) );
    addTool( new BookmarksToolView(new BookmarksTool()) );
}

void OktetaMainWindow::saveProperties( KConfigGroup &configGroup )
{
    const QStringList urls = mDocumentManager->urls();
    configGroup.writePathEntry( LoadedUrlsKey, urls );
}

void OktetaMainWindow::readProperties( const KConfigGroup &configGroup )
{
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
}


OktetaMainWindow::~OktetaMainWindow() {}

}
