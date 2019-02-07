/*
    This file is part of the Okteta program, made within the KDE community.

    Copyright 2006-2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "mainwindow.hpp"

// program
#include "program.hpp"
// tmp
#include <Kasten/Okteta/ByteArrayViewProfileManager>
// tools
#include <Kasten/Okteta/StringsExtractToolViewFactory>
#include <Kasten/Okteta/StringsExtractToolFactory>
#include <Kasten/Okteta/InfoToolViewFactory>
#include <Kasten/Okteta/InfoToolFactory>
#include <Kasten/Okteta/FilterToolViewFactory>
#include <Kasten/Okteta/FilterToolFactory>
#include <Kasten/Okteta/CharsetConversionToolViewFactory>
#include <Kasten/Okteta/CharsetConversionToolFactory>
#include <Kasten/Okteta/ChecksumToolViewFactory>
#include <Kasten/Okteta/ChecksumToolFactory>
#include <Kasten/Okteta/DocumentInfoToolViewFactory>
#include <Kasten/Okteta/DocumentInfoToolFactory>
#include <Kasten/Okteta/PODDecoderToolViewFactory>
#include <Kasten/Okteta/PODDecoderToolFactory>
#include <Kasten/Okteta/ByteTableToolViewFactory>
#include <Kasten/Okteta/ByteTableToolFactory>
#include <Kasten/Okteta/BookmarksToolViewFactory>
#include <Kasten/Okteta/BookmarksToolFactory>
#include <Kasten/Okteta/StructuresToolViewFactory>
#include <Kasten/Okteta/StructuresToolFactory>

// Kasten tools
#include <Kasten/VersionViewToolViewFactory>
#include <Kasten/VersionViewToolFactory>
#include <Kasten/FileSystemBrowserToolViewFactory>
#include <Kasten/FileSystemBrowserToolFactory>
#include <Kasten/DocumentsToolViewFactory>
#include <Kasten/DocumentsToolFactory>
#include <Kasten/TerminalToolViewFactory>
#include <Kasten/TerminalToolFactory>
// controllers
#include <Kasten/Okteta/OverwriteOnlyController>
#include <Kasten/Okteta/OverwriteModeController>
#include <Kasten/Okteta/GotoOffsetController>
#include <Kasten/Okteta/SelectRangeController>
#include <Kasten/Okteta/SearchController>
#include <Kasten/Okteta/ReplaceController>
#include <Kasten/Okteta/BookmarksController>
#include <Kasten/Okteta/PrintController>
#include <Kasten/Okteta/ViewConfigController>
#include <Kasten/Okteta/ViewModeController>
#include <Kasten/Okteta/ViewStatusController>
#include <Kasten/Okteta/ViewProfileController>
#include <Kasten/Okteta/ViewProfilesManageController>
// Kasten controllers
#include <Kasten/ModifiedBarController>
#include <Kasten/ReadOnlyController>
#include <Kasten/ReadOnlyBarController>
#include <Kasten/CreatorController>
#include <Kasten/LoaderController>
#include <Kasten/CloseController>
#include <Kasten/SetRemoteController>
#include <Kasten/SynchronizeController>
#include <Kasten/ClipboardController>
#include <Kasten/InsertController>
#include <Kasten/CopyAsController>
#include <Kasten/ExportController>
#include <Kasten/VersionController>
#include <Kasten/ZoomController>
#include <Kasten/ZoomBarController>
#include <Kasten/SelectController>
#include <Kasten/SwitchViewController>
#include <Kasten/ViewListMenuController>
#include <Kasten/ViewAreaSplitController>
#include <Kasten/ToolListMenuController>
#include <Kasten/FullScreenController>
#include <Kasten/QuitController>
// Kasten gui
#include <Kasten/MultiDocumentStrategy>
#include <Kasten/ModelCodecViewManager>
#include <Kasten/ViewManager>
#include <Kasten/MultiViewAreas>
#include <Kasten/StatusBar>
// Kasten core
#include <Kasten/ModelCodecManager>
#include <Kasten/DocumentCreateManager>
#include <Kasten/DocumentSyncManager>
#include <Kasten/DocumentManager>
// KF5
#include <KConfigGroup>
#include <KSharedConfig>
// Qt
#include <QUrl>
#include <QMimeData>

namespace Kasten {

static const char LoadedUrlsKey[] = "LoadedUrls";

OktetaMainWindow::OktetaMainWindow(OktetaProgram* program)
    : ShellWindow(program->viewManager())
    , mProgram(program)
{
    setObjectName(QStringLiteral("Shell"));

    // there is only one mainwindow, so have this show the document if requested
    connect(mProgram->documentManager(), &DocumentManager::focusRequested,
            this, &OktetaMainWindow::showDocument);
    connect(viewArea(), &MultiViewAreas::dataOffered,
            this, &OktetaMainWindow::onDataOffered);
    connect(viewArea(), &MultiViewAreas::dataDropped,
            this, &OktetaMainWindow::onDataDropped);
    connect(viewArea(), &AbstractGroupedViews::closeRequest,
            this, &OktetaMainWindow::onCloseRequest);

    // XXX: Workaround for Qt 4.4's lacking of proper handling of the initial layout of dock widgets
    //      This state is taken from an oktetarc where the docker constellation was configured by hand.
    //      Setting this state if none is present seems to work, but there's
    //      still the versioning problem to be accounted for.
    //      Hack borrowed from trunk/koffice/krita/ui/kis_view2.cpp:
    const QString mainWindowState = QStringLiteral(
        "AAAA/wAAAAD9AAAAAwAAAAAAAADPAAACg/wCAAAAAvsAAAAiAEYAaQBsAGUAUwB5AHMAdABlAG0AQgByAG8AdwBzAGUAcgAAAABJAAACgwAAAB4BAAAF+wAAABIARABvAGMAdQBtAGUAbgB0AHMAAAAASQAAAmMAAABeAQAABQAAAAEAAAGcAAACXPwCAAAACPsAAAAUAFAATwBEAEQAZQBjAG8AZABlAHIAAAAAQgAAARMAAAB9AQAABfsAAAAUAFMAdAByAHUAYwB0AFQAbwBvAGwAAAAAQgAAAlwAAAB9AQAABfsAAAAQAFYAZQByAHMAaQBvAG4AcwAAAABNAAAAVgAAAF4BAAAF+wAAABgAQgBpAG4AYQByAHkARgBpAGwAdABlAHIAAAABegAAAM0AAAC8AQAABfsAAAAQAEMAaABlAGMAawBzAHUAbQAAAAF8AAAAywAAAL0BAAAF/AAAAREAAADlAAAAAAD////6AAAAAAEAAAAE+wAAABAAQwBoAGUAYwBrAFMAdQBtAQAAAAD/////AAAAAAAAAAD7AAAAEgBCAG8AbwBrAG0AYQByAGsAcwIAAALBAAAAPQAAAT8AAAFk+wAAAA4AUwB0AHIAaQBuAGcAcwAAAAAA/////wAAAQ8BAAAF+wAAAAgASQBuAGYAbwAAAAGRAAABTAAAAIUBAAAF+wAAABIAQgB5AHQAZQBUAGEAYgBsAGUAAAAAUwAAAjkAAAB9AQAABfsAAAAYAEQAbwBjAHUAbQBlAG4AdABJAG4AZgBvAAAAAEkAAAJjAAAA+wEAAAUAAAADAAAAAAAAAAD8AQAAAAH7AAAAEABUAGUAcgBtAGkAbgBhAGwAAAAAAP////8AAABPAQAABQAABBUAAAGLAAAABAAAAAQAAAAIAAAACPwAAAABAAAAAgAAAAEAAAAWAG0AYQBpAG4AVABvAG8AbABCAGEAcgEAAAAAAAAEBgAAAAAAAAAA");
    const char mainWindowStateKey[] = "State";
    KConfigGroup group(KSharedConfig::openConfig(), QStringLiteral("MainWindow"));
    if (!group.hasKey(mainWindowStateKey)) {
        group.writeEntry(mainWindowStateKey, mainWindowState);
    }

    setStatusBar(new Kasten::StatusBar(this));

    setupControllers();
    setupGUI();

    // all controllers which use plugActionList have to do so after(!) setupGUI() or their entries will be removed
    // TODO: why is this so?
    // tmp
    addXmlGuiController(new ToolListMenuController(this, this));
    addXmlGuiController(new ViewListMenuController(viewManager(), viewArea(), this));
}

OktetaMainWindow::~OktetaMainWindow() = default;

void OktetaMainWindow::setupControllers()
{
    MultiDocumentStrategy* const documentStrategy = mProgram->documentStrategy();
    ViewManager* const viewManager = this->viewManager();
    MultiViewAreas* const viewArea = this->viewArea();
    ModelCodecViewManager* const codecViewManager = viewManager->codecViewManager();
    DocumentManager* const documentManager = mProgram->documentManager();
    ModelCodecManager* const codecManager = documentManager->codecManager();
    DocumentSyncManager* const syncManager = documentManager->syncManager();
    // tmp
    ByteArrayViewProfileManager* const byteArrayViewProfileManager = mProgram->byteArrayViewProfileManager();

    // general, part of Kasten
    addXmlGuiController(new CreatorController(codecManager,
                                              documentStrategy, this));
    addXmlGuiController(new LoaderController(documentStrategy, this));
    addXmlGuiController(new SetRemoteController(syncManager, this));
    addXmlGuiController(new SynchronizeController(syncManager, this));
    addXmlGuiController(new ExportController(codecViewManager,
                                             codecManager, this));
    addXmlGuiController(new CloseController(documentStrategy, this));
    addXmlGuiController(new VersionController(this));
    addXmlGuiController(new ReadOnlyController(this));
    addXmlGuiController(new SwitchViewController(viewArea, this));
    addXmlGuiController(new ViewAreaSplitController(viewManager, viewArea, this));
    addXmlGuiController(new FullScreenController(this));
    addXmlGuiController(new QuitController(nullptr, this));

    addXmlGuiController(new ZoomController(this));
    addXmlGuiController(new SelectController(this));
    addXmlGuiController(new ClipboardController(this));
    addXmlGuiController(new InsertController(codecViewManager,
                                             codecManager, this));
    addXmlGuiController(new CopyAsController(codecViewManager,
                                             codecManager, this));

    addToolFromFactory(FileSystemBrowserToolViewFactory(), FileSystemBrowserToolFactory(syncManager));
    addToolFromFactory(DocumentsToolViewFactory(), DocumentsToolFactory(documentManager));
    addToolFromFactory(TerminalToolViewFactory(), TerminalToolFactory(syncManager));
#ifndef NDEBUG
    addToolFromFactory(VersionViewToolViewFactory(), VersionViewToolFactory());
#endif

    // Okteta specific
//     addXmlGuiController( new OverwriteOnlyController(this) );
    addXmlGuiController(new OverwriteModeController(this));
    addXmlGuiController(new SearchController(this, this));
    addXmlGuiController(new ReplaceController(this, this));
    addXmlGuiController(new GotoOffsetController(viewArea, this));
    addXmlGuiController(new SelectRangeController(viewArea, this));
    addXmlGuiController(new BookmarksController(this));
    addXmlGuiController(new PrintController(this));
    addXmlGuiController(new ViewConfigController(this));
    addXmlGuiController(new ViewModeController(this));
    addXmlGuiController(new ViewProfileController(byteArrayViewProfileManager, this, this));
    addXmlGuiController(new ViewProfilesManageController(this, byteArrayViewProfileManager, this));

    Kasten::StatusBar* const bottomBar = static_cast<Kasten::StatusBar*>(statusBar());
    addXmlGuiController(new ViewStatusController(bottomBar));
    addXmlGuiController(new ModifiedBarController(bottomBar));
    addXmlGuiController(new ReadOnlyBarController(bottomBar));
    addXmlGuiController(new ZoomBarController(bottomBar));

    addToolFromFactory(DocumentInfoToolViewFactory(), DocumentInfoToolFactory(syncManager));
    addToolFromFactory(ChecksumToolViewFactory(), ChecksumToolFactory());
    addToolFromFactory(FilterToolViewFactory(), FilterToolFactory());
    addToolFromFactory(CharsetConversionToolViewFactory(), CharsetConversionToolFactory());
    addToolFromFactory(StringsExtractToolViewFactory(), StringsExtractToolFactory());
    addToolFromFactory(ByteTableToolViewFactory(), ByteTableToolFactory());
    addToolFromFactory(InfoToolViewFactory(), InfoToolFactory());
    addToolFromFactory(PodDecoderToolViewFactory(), PodDecoderToolFactory());
    addToolFromFactory(StructuresToolViewFactory(), StructuresToolFactory());
    addToolFromFactory(BookmarksToolViewFactory(), BookmarksToolFactory());
}

void OktetaMainWindow::addToolFromFactory(const AbstractToolViewFactory& toolViewFactory,
                                          const AbstractToolFactory& toolFactory)
{
    AbstractTool* tool = toolFactory.create();
    AbstractToolView* toolView = toolViewFactory.create(tool);

    addTool(toolView);
}

bool OktetaMainWindow::queryClose()
{
    // TODO: query the document manager or query the view manager?
    return mProgram->documentManager()->canCloseAll();
}

void OktetaMainWindow::saveProperties(KConfigGroup& configGroup)
{
    DocumentManager* const documentManager = mProgram->documentManager();
    DocumentSyncManager* const syncManager = documentManager->syncManager();
    const QVector<AbstractDocument*> documents = documentManager->documents();

    QStringList urls;
    urls.reserve(documents.size());
    for (AbstractDocument* document : documents) {
        urls.append(syncManager->urlOf(document).url());
    }

    configGroup.writePathEntry(LoadedUrlsKey, urls);
}

void OktetaMainWindow::readProperties(const KConfigGroup& configGroup)
{
    const QStringList urls = configGroup.readPathEntry(LoadedUrlsKey, QStringList());

    DocumentManager* const documentManager = mProgram->documentManager();
    DocumentSyncManager* const syncManager = documentManager->syncManager();
    DocumentCreateManager* const createManager = documentManager->createManager();
    for (const QString& url : urls) {
        if (url.isEmpty()) {
            createManager->createNew();
        } else {
            syncManager->load(QUrl(url, QUrl::TolerantMode));
        }
        // TODO: set view to offset
        // if( offset != -1 )
    }
}

void OktetaMainWindow::onDataOffered(const QMimeData* mimeData, bool& accept)
{
    accept = mimeData->hasUrls()
             || mProgram->documentManager()->createManager()->canCreateNewFromData(mimeData);
}

void OktetaMainWindow::onDataDropped(const QMimeData* mimeData)
{
    const QList<QUrl> urls = mimeData->urls();

    DocumentManager* const documentManager = mProgram->documentManager();
    if (!urls.isEmpty()) {
        DocumentSyncManager* const syncManager = documentManager->syncManager();

        for (const QUrl& url : urls) {
            syncManager->load(url);
        }
    } else {
        documentManager->createManager()->createNewFromData(mimeData, true);
    }
}

void OktetaMainWindow::onCloseRequest(const QVector<Kasten::AbstractView*>& views)
{
    // group views per document
    QHash<AbstractDocument*, QVector<AbstractView*>> viewsToClosePerDocument;
    for (AbstractView* view : views) {
        AbstractDocument* document = view->findBaseModel<AbstractDocument*>();
        viewsToClosePerDocument[document].append(view);
    }

    // find documents which lose all views
    const QVector<AbstractView*> allViews = viewManager()->views();
    for (AbstractView* view : allViews) {
        AbstractDocument* document = view->findBaseModel<AbstractDocument*>();
        QHash<AbstractDocument*, QVector<AbstractView*>>::Iterator it =
            viewsToClosePerDocument.find(document);

        if (it != viewsToClosePerDocument.end()) {
            const QVector<AbstractView*>& viewsOfDocument = it.value();
            const bool isAnotherView = !viewsOfDocument.contains(view);
            if (isAnotherView) {
                viewsToClosePerDocument.erase(it);
            }
        }
    }

    const QVector<AbstractDocument*> documentsWithoutViews = viewsToClosePerDocument.keys().toVector();

    DocumentManager* const documentManager = mProgram->documentManager();
    if (documentManager->canClose(documentsWithoutViews)) {
        viewManager()->removeViews(views);
        documentManager->closeDocuments(documentsWithoutViews);
    }
}

}
