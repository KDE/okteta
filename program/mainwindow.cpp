/*
    This file is part of the Okteta program, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
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
#if KASTEN_BUILD_TERMINALTOOL
#include <Kasten/TerminalToolViewFactory>
#include <Kasten/TerminalToolFactory>
#endif
// controllers
// #include <Kasten/Okteta/OverwriteOnlyControllerFactory>
#include <Kasten/Okteta/CropControllerFactory>
#include <Kasten/Okteta/OverwriteModeControllerFactory>
#include <Kasten/Okteta/GotoOffsetControllerFactory>
#include <Kasten/Okteta/SelectRangeControllerFactory>
#include <Kasten/Okteta/SearchControllerFactory>
#include <Kasten/Okteta/ReplaceControllerFactory>
#include <Kasten/Okteta/BookmarksControllerFactory>
#include <Kasten/Okteta/PrintControllerFactory>
#include <Kasten/Okteta/ViewConfigControllerFactory>
#include <Kasten/Okteta/ViewModeControllerFactory>
#include <Kasten/Okteta/ViewContextMenuControllerFactory>
#include <Kasten/Okteta/ViewStatusControllerFactory>
#include <Kasten/Okteta/ViewProfileControllerFactory>
#include <Kasten/Okteta/ViewProfilesManageControllerFactory>
// Kasten controllers
#include <Kasten/ModifiedBarControllerFactory>
#include <Kasten/ReadOnlyControllerFactory>
#include <Kasten/ReadOnlyBarControllerFactory>
#include <Kasten/CreatorControllerFactory>
#include <Kasten/LoaderControllerFactory>
#include <Kasten/CloseControllerFactory>
#include <Kasten/SetRemoteControllerFactory>
#include <Kasten/SynchronizeControllerFactory>
#include <Kasten/ClipboardControllerFactory>
#include <Kasten/InsertControllerFactory>
#include <Kasten/CopyAsControllerFactory>
#include <Kasten/ExportControllerFactory>
#include <Kasten/VersionControllerFactory>
#include <Kasten/ZoomControllerFactory>
#include <Kasten/ZoomBarControllerFactory>
#include <Kasten/SelectControllerFactory>
#include <Kasten/ViewAreaContextMenuControllerFactory>
#include <Kasten/SwitchViewControllerFactory>
#include <Kasten/ViewListMenuControllerFactory>
#include <Kasten/ViewAreaSplitControllerFactory>
#include <Kasten/ToolListMenuControllerFactory>
#include <Kasten/FullScreenControllerFactory>
#include <Kasten/UiColorSchemeControllerFactory>
#include <Kasten/QuitControllerFactory>
// Kasten gui
#include <Kasten/AbstractToolView>
#include <Kasten/AbstractXmlGuiController>
#include <Kasten/UserMessagesHandler>
#include <Kasten/MultiDocumentStrategy>
#include <Kasten/ModelCodecViewManager>
#include <Kasten/ViewManager>
#include <Kasten/MultiViewAreas>
#include <Kasten/StatusBar>
// Kasten core
#include <Kasten/AbstractTool>
#include <Kasten/ModelCodecManager>
#include <Kasten/DocumentCreateManager>
#include <Kasten/DocumentSyncManager>
#include <Kasten/DocumentManager>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
// Qt
#include <QUrl>
#include <QMimeData>
// Std
#include <algorithm>
#include <unordered_map>
#include <vector>

namespace Kasten {

static constexpr char LoadedUrlsKey[] = "LoadedUrls";

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
    connect(viewArea(), &MultiViewAreas::newDocumentRequested,
            this, &OktetaMainWindow::onNewDocumentRequested);
    connect(viewArea(), &AbstractGroupedViews::closeRequest,
            this, &OktetaMainWindow::onCloseRequest);

// Using State value as generated with Qt 5.15.11-KDE
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 11)
    // XXX: Workaround for Qt 4.4's lacking of proper handling of the initial layout of dock widgets
    //      This state is taken from an oktetarc where the docker constellation was configured by hand.
    //      Setting this state if none is present seems to work, but there's
    //      still the versioning problem to be accounted for.
    //      Hack borrowed from trunk/koffice/krita/ui/kis_view2.cpp:
    const QString mainWindowState = QStringLiteral(
        "AAAA/wAAAAD9AAAAAwAAAAAAAAEVAAACLvwCAAAAAfwAAABLAAACLgAAAI8BAAAd+gAAAAABAAAAAvsAAAAiAEYAaQBsAGUAUwB5AHMAdABlAG0AQgByAG8AdwBzAGUAcgEAAAAA/////wAAAGQA////+wAAABIARABvAGMAdQBtAGUAbgB0AHMBAAAAAP////8AAABcAP///wAAAAEAAAHhAAACLvwCAAAABfsAAAAUAFMAdAByAHUAYwB0AFQAbwBvAGwDAAACjQAAAPwAAAIyAAABT/sAAAAQAFYAZQByAHMAaQBvAG4AcwAAAAA2AAAAiQAAAAAAAAAA/AAAAEsAAAEqAAABKgEAAB36AAAAAQEAAAAH+wAAABgARABvAGMAdQBtAGUAbgB0AEkAbgBmAG8BAAAAAP////8AAACMAP////sAAAASAEIAeQB0AGUAVABhAGIAbABlAQAAAAD/////AAAAXAD////7AAAADgBTAHQAcgBpAG4AZwBzAQAAAAD/////AAAAiQD////7AAAAEABDAGgAZQBjAGsAcwB1AG0BAAAAAP////8AAAEFAP////sAAAAcAFMAdAByAHUAYwB0AHUAcgBlAHMAVABvAG8AbAEAAAAA/////wAAAJkA////+wAAABQAUwB0AHIAdQBjAHQAVABvAG8AbAMAAAKNAAAA/AAAAjIAAAFP+wAAABQAUABPAEQARABlAGMAbwBkAGUAcgEAAAAA/////wAAAJQA/////AAAAQsAAADsAAAAAAD////6AAAAAAEAAAAB+wAAABAAQwBoAGUAYwBrAFMAdQBtAQAAAAD/////AAAAAAAAAAD8AAABdgAAAQMAAAEDAQAAHfoAAAAAAQAAAAT7AAAAGABCAGkAbgBhAHIAeQBGAGkAbAB0AGUAcgEAAAAA/////wAAAVMA////+wAAACIAQwBoAGEAcgBzAGUAdABDAG8AbgB2AGUAcgBzAGkAbwBuAQAAAAD/////AAABRAD////7AAAACABJAG4AZgBvAQAAAAD/////AAAAiQD////7AAAAEgBCAG8AbwBrAG0AYQByAGsAcwEAAAAA/////wAAAFwA////AAAAAwAABVYAAABC/AEAAAAB+wAAABAAVABlAHIAbQBpAG4AYQBsAAAAAAAAAAVWAAAAXAD///8AAADlAAACLgAAAAQAAAAEAAAACAAAAAj8AAAAAQAAAAIAAAABAAAAFgBtAGEAaQBuAFQAbwBvAGwAQgBhAHIBAAAAAAAABAYAAAAAAAAAAA==");
    const char mainWindowStateKey[] = "State";
    KConfigGroup group(KSharedConfig::openConfig(), QStringLiteral("MainWindow"));
    if (!group.hasKey(mainWindowStateKey)) {
        group.writeEntry(mainWindowStateKey, mainWindowState);
    }
#endif

    setStatusBar(new Kasten::StatusBar(this));

    setupControllers();
    setupGUI();

    // all controllers which use plugActionList have to do so after(!) setupGUI() or their entries will be removed
    // TODO: why is this so?
    // tmp
    addXmlGuiControllerFromFactory(ToolListMenuControllerFactory(this));
    addXmlGuiControllerFromFactory(ViewListMenuControllerFactory(viewManager(), viewArea()));
}

OktetaMainWindow::~OktetaMainWindow() = default;

void OktetaMainWindow::setupControllers()
{
    AbstractUserMessagesHandler* const userMessagesHandler = mProgram->userMessagesHandler();
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
    addXmlGuiControllerFromFactory(CreatorControllerFactory(codecManager, documentStrategy));
    addXmlGuiControllerFromFactory(LoaderControllerFactory(documentStrategy));
    addXmlGuiControllerFromFactory(SetRemoteControllerFactory(syncManager));
    addXmlGuiControllerFromFactory(SynchronizeControllerFactory(syncManager));
    addXmlGuiControllerFromFactory(ExportControllerFactory(codecViewManager, codecManager));
    addXmlGuiControllerFromFactory(CloseControllerFactory(documentStrategy));
    addXmlGuiControllerFromFactory(VersionControllerFactory());
    addXmlGuiControllerFromFactory(ReadOnlyControllerFactory());
    addXmlGuiControllerFromFactory(ViewAreaContextMenuControllerFactory(viewArea, syncManager));
    addXmlGuiControllerFromFactory(SwitchViewControllerFactory(viewArea));
    addXmlGuiControllerFromFactory(ViewAreaSplitControllerFactory(viewManager, viewArea));
    addXmlGuiControllerFromFactory(FullScreenControllerFactory(this));
    addXmlGuiControllerFromFactory(UiColorSchemeControllerFactory(this));
    addXmlGuiControllerFromFactory(QuitControllerFactory(this));

    addXmlGuiControllerFromFactory(ZoomControllerFactory());
    addXmlGuiControllerFromFactory(SelectControllerFactory());
    addXmlGuiControllerFromFactory(ClipboardControllerFactory());
    addXmlGuiControllerFromFactory(InsertControllerFactory(codecViewManager, codecManager));
    addXmlGuiControllerFromFactory(CopyAsControllerFactory(codecViewManager, codecManager));

    addToolFromFactory(FileSystemBrowserToolViewFactory(), FileSystemBrowserToolFactory(syncManager));
    addToolFromFactory(DocumentsToolViewFactory(), DocumentsToolFactory(documentManager));
#if KASTEN_BUILD_TERMINALTOOL
    addToolFromFactory(TerminalToolViewFactory(), TerminalToolFactory(syncManager));
#endif
#ifndef NDEBUG
    addToolFromFactory(VersionViewToolViewFactory(), VersionViewToolFactory());
#endif

    // Okteta specific
//     addXmlGuiControllerFromFactory(OverwriteOnlyControllerFactory() );
    addXmlGuiControllerFromFactory(CropControllerFactory());
    addXmlGuiControllerFromFactory(OverwriteModeControllerFactory());
    addXmlGuiControllerFromFactory(SearchControllerFactory(userMessagesHandler, this));
    addXmlGuiControllerFromFactory(ReplaceControllerFactory(userMessagesHandler, this));
    addXmlGuiControllerFromFactory(GotoOffsetControllerFactory(viewArea));
    addXmlGuiControllerFromFactory(SelectRangeControllerFactory(viewArea));
    addXmlGuiControllerFromFactory(BookmarksControllerFactory());
    addXmlGuiControllerFromFactory(PrintControllerFactory());
    addXmlGuiControllerFromFactory(ViewConfigControllerFactory());
    addXmlGuiControllerFromFactory(ViewModeControllerFactory());
    addXmlGuiControllerFromFactory(ViewContextMenuControllerFactory());
    addXmlGuiControllerFromFactory(ViewProfileControllerFactory(byteArrayViewProfileManager, this));
    addXmlGuiControllerFromFactory(ViewProfilesManageControllerFactory(byteArrayViewProfileManager, this));

    auto* const bottomBar = static_cast<Kasten::StatusBar*>(statusBar());
    addXmlGuiControllerFromFactory(ViewStatusControllerFactory(bottomBar));
    addXmlGuiControllerFromFactory(ModifiedBarControllerFactory(bottomBar));
    addXmlGuiControllerFromFactory(ReadOnlyBarControllerFactory(bottomBar));
    addXmlGuiControllerFromFactory(ZoomBarControllerFactory(bottomBar));

    addToolFromFactory(DocumentInfoToolViewFactory(), DocumentInfoToolFactory(syncManager));
    addToolFromFactory(ChecksumToolViewFactory(), ChecksumToolFactory());
    addToolFromFactory(FilterToolViewFactory(), FilterToolFactory());
    addToolFromFactory(CharsetConversionToolViewFactory(userMessagesHandler), CharsetConversionToolFactory());
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
    auto tool = toolFactory.create();
    auto toolView = toolViewFactory.create(tool.get());

    addTool(std::move(tool), std::move(toolView));
}

void OktetaMainWindow::addXmlGuiControllerFromFactory(const AbstractXmlGuiControllerFactory& factory)
{
    auto controller = factory.create(this);
    addXmlGuiController(std::move(controller));
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

void OktetaMainWindow::onNewDocumentRequested()
{
    mProgram->documentManager()->createManager()->createNew();
}

void OktetaMainWindow::onCloseRequest(const QVector<Kasten::AbstractView*>& views)
{
    // group views per document
    std::unordered_map<AbstractDocument*, std::vector<AbstractView*>> viewsToClosePerDocument;
    for (AbstractView* view : views) {
        auto* document = view->findBaseModel<AbstractDocument*>();
        viewsToClosePerDocument[document].emplace_back(view);
    }

    // find documents which lose all views
    const QVector<AbstractView*> allViews = viewManager()->views();
    for (AbstractView* view : allViews) {
        auto* document = view->findBaseModel<AbstractDocument*>();
        auto it = viewsToClosePerDocument.find(document);

        if (it != viewsToClosePerDocument.end()) {
            const std::vector<AbstractView*>& viewsOfDocument = it->second;
            const bool isAnotherView = (std::find(viewsOfDocument.cbegin(), viewsOfDocument.cend(), view) == viewsOfDocument.cend());
            if (isAnotherView) {
                viewsToClosePerDocument.erase(it);
            }
        }
    }

    QVector<AbstractDocument*> documentsWithoutViews;
    documentsWithoutViews.reserve(viewsToClosePerDocument.size());
    std::transform(viewsToClosePerDocument.cbegin(), viewsToClosePerDocument.cend(),
                   std::back_inserter(documentsWithoutViews),
                   [](const std::pair<AbstractDocument*, std::vector<AbstractView*>>& entry){ return entry.first; });

    DocumentManager* const documentManager = mProgram->documentManager();
    if (documentManager->canClose(documentsWithoutViews)) {
        viewManager()->removeViews(views);
        documentManager->closeDocuments(documentsWithoutViews);
    }
}

}

#include "moc_mainwindow.cpp"
