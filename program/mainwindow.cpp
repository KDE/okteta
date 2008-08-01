/*
    This file is part of the Okteta program, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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
// controllers
#include <overwriteonly/overwriteonlycontroller.h>
#include <overwritemode/overwritemodecontroller.h>
#include <poddecoder/poddecodercontroller.h>
#include <bytetable/bytetablecontroller.h>
#include <info/infocontroller.h>
#include <stringsextract/stringsextractcontroller.h>
#include <filter/filtercontroller.h>
#include <insertpattern/insertpatterncontroller.h>
#include <gotooffset/gotooffsetcontroller.h>
#include <search/searchcontroller.h>
#include <replace/replacecontroller.h>
#include <bookmarks/bookmarkscontroller.h>
#include <print/printcontroller.h>
#include <viewconfig/viewconfigcontroller.h>
// Kakao controllers
#include <document/versionview/versionviewcontroller.h>
#include <document/readonly/readonlycontroller.h>
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
#include <view/select/selectcontroller.h>
#include <shellwindow/viewlistmenu/viewlistmenucontroller.h>
#include <shellwindow/fullscreen/fullscreencontroller.h>
/*#include <viewsystem/close/closecontroller.h>*/
#include <program/quit/quitcontroller.h>
// Kakao gui
#include <kviewmanager.h>
#include <tabbedviews.h>
// Kakao core
#include <kdocumentcreatemanager.h>
#include <kdocumentsyncmanager.h>
#include <kdocumentmanager.h>
// KDE
#include <KGlobal>
#include <KConfigGroup>

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
    const QString mainWindowState = "AAAA/wAAAAD9AAAAAgAAAAEAAAFMAAABe/wCAAAAAvsAAAAQAFYAZQByAHMAaQBvAG4AcwEAAABNAAAAVgAAAAAAAAAA/AAAAE0AAAF7AAABOAEAACL6AAAAAAEAAAAE+wAAABgAQgBpAG4AYQByAHkARgBpAGwAdABlAHIBAAAAAP////8AAAD0AP////sAAAAOAFMAdAByAGkAbgBnAHMBAAAAAP////8AAAExAP////sAAAASAEIAeQB0AGUAVABhAGIAbABlAQAAAAD/////AAABTAD////7AAAACABJAG4AZgBvAQAAAZEAAAFMAAABEAD///8AAAADAAAC3QAAANb8AQAAAAH7AAAAFABQAE8ARABEAGUAYwBvAGQAZQByAQAAAAAAAALdAAAC3QD///8AAAGLAAABewAAAAQAAAAEAAAACAAAAAj8AAAAAQAAAAIAAAABAAAAFgBtAGEAaQBuAFQAbwBvAGwAQgBhAHIBAAAAAAAAAt0AAAAAAAAAAA==";

    const char mainWindowStateKey[] = "State";
    KConfigGroup group( KGlobal::config(), "MainWindow" );
    if( !group.hasKey(mainWindowStateKey) )
        group.writeEntry( mainWindowStateKey, mainWindowState );

    setupControllers();
    setupGUI();
}

void OktetaMainWindow::setupControllers()
{
    mControllers.append( new CreatorController(mProgram->documentManager()->createManager(),this) );
    mControllers.append( new LoaderController(mProgram->documentManager()->syncManager(),this) );
    mControllers.append( new SetRemoteController(mProgram->documentManager()->syncManager(),this) );
    mControllers.append( new SynchronizeController(this) );
    mControllers.append( new CloseController(mProgram->documentManager(),this) );
    mControllers.append( new VersionController(this) );
#ifndef NDEBUG
    mControllers.append( new VersionViewController(this) );
#endif
    mControllers.append( new ReadOnlyController(this) );
    mControllers.append( new ViewListMenuController(mProgram->viewManager(),mTabbedViews,this) );
    mControllers.append( new FullScreenController(this) );
    mControllers.append( new QuitController(0,this) );

    mControllers.append( new ZoomController(this) );
    mControllers.append( new SelectController(this) );
    mControllers.append( new ClipboardController(this) );
    mControllers.append( new CopyAsController(mProgram->documentManager(),this) );
    mControllers.append( new ExportController(mProgram->documentManager(),this) );

//     mControllers.append( new OverwriteOnlyController(this) );
    mControllers.append( new OverwriteModeController(this) );
    mControllers.append( new PODDecoderController(this) );
    mControllers.append( new ByteTableController(this) );
    mControllers.append( new InfoController(this) );
    mControllers.append( new StringsExtractController(this) );
    mControllers.append( new SearchController(this) );
    mControllers.append( new ReplaceController(this) );
    mControllers.append( new GotoOffsetController(this) );
    mControllers.append( new FilterController(this) );
    mControllers.append( new InsertPatternController(this) );
    mControllers.append( new BookmarksController(this) );
    mControllers.append( new PrintController(this) );
    mControllers.append( new ViewConfigController(this) );
}

void OktetaMainWindow::saveProperties( KConfigGroup &configGroup )
{
    const QStringList urls = mDocumentManager->urls();
    configGroup.writePathEntry( LoadedUrlsKey, urls );
}

void OktetaMainWindow::readProperties( const KConfigGroup &configGroup )
{
    const QStringList urls = configGroup.readPathEntry( LoadedUrlsKey, QStringList() );

    KDocumentSyncManager *syncManager = mDocumentManager->syncManager();
    KDocumentCreateManager *createManager = mDocumentManager->createManager();
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

#include "mainwindow.moc"
