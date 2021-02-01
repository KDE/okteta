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
#include <Kasten/TerminalToolViewFactory>
#include <Kasten/TerminalToolFactory>
// controllers
// #include <Kasten/Okteta/OverwriteOnlyControllerFactory>
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
#include <Kasten/SwitchViewControllerFactory>
#include <Kasten/ViewListMenuControllerFactory>
#include <Kasten/ViewAreaSplitControllerFactory>
#include <Kasten/ToolListMenuControllerFactory>
#include <Kasten/FullScreenControllerFactory>
#ifndef KASTEN_NO_UICOLORSCHEMECONTROLLER
#include <Kasten/UiColorSchemeControllerFactory>
#endif
#include <Kasten/QuitControllerFactory>
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
// KF
#include <KConfigGroup>
#include <KSharedConfig>
// Qt
#include <QUrl>
#include <QMimeData>

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
    addXmlGuiControllerFromFactory(ToolListMenuControllerFactory(this));
    addXmlGuiControllerFromFactory(ViewListMenuControllerFactory(viewManager(), viewArea()));
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
    addXmlGuiControllerFromFactory(CreatorControllerFactory(codecManager, documentStrategy));
    addXmlGuiControllerFromFactory(LoaderControllerFactory(documentStrategy));
    addXmlGuiControllerFromFactory(SetRemoteControllerFactory(syncManager));
    addXmlGuiControllerFromFactory(SynchronizeControllerFactory(syncManager));
    addXmlGuiControllerFromFactory(ExportControllerFactory(codecViewManager, codecManager));
    addXmlGuiControllerFromFactory(CloseControllerFactory(documentStrategy));
    addXmlGuiControllerFromFactory(VersionControllerFactory());
    addXmlGuiControllerFromFactory(ReadOnlyControllerFactory());
    addXmlGuiControllerFromFactory(SwitchViewControllerFactory(viewArea));
    addXmlGuiControllerFromFactory(ViewAreaSplitControllerFactory(viewManager, viewArea));
    addXmlGuiControllerFromFactory(FullScreenControllerFactory(this));
#ifndef KASTEN_NO_UICOLORSCHEMECONTROLLER
    addXmlGuiControllerFromFactory(UiColorSchemeControllerFactory(this));
#endif
    addXmlGuiControllerFromFactory(QuitControllerFactory(this));

    addXmlGuiControllerFromFactory(ZoomControllerFactory());
    addXmlGuiControllerFromFactory(SelectControllerFactory());
    addXmlGuiControllerFromFactory(ClipboardControllerFactory());
    addXmlGuiControllerFromFactory(InsertControllerFactory(codecViewManager, codecManager));
    addXmlGuiControllerFromFactory(CopyAsControllerFactory(codecViewManager, codecManager));

    addToolFromFactory(FileSystemBrowserToolViewFactory(), FileSystemBrowserToolFactory(syncManager));
    addToolFromFactory(DocumentsToolViewFactory(), DocumentsToolFactory(documentManager));
    addToolFromFactory(TerminalToolViewFactory(), TerminalToolFactory(syncManager));
#ifndef NDEBUG
    addToolFromFactory(VersionViewToolViewFactory(), VersionViewToolFactory());
#endif

    // Okteta specific
//     addXmlGuiControllerFromFactory(OverwriteOnlyControllerFactory() );
    addXmlGuiControllerFromFactory(OverwriteModeControllerFactory());
    addXmlGuiControllerFromFactory(SearchControllerFactory(this));
    addXmlGuiControllerFromFactory(ReplaceControllerFactory(this));
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

void OktetaMainWindow::addXmlGuiControllerFromFactory(const AbstractXmlGuiControllerFactory& factory)
{
    AbstractXmlGuiController* controller = factory.create(this);
    addXmlGuiController(controller);
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
        auto* document = view->findBaseModel<AbstractDocument*>();
        viewsToClosePerDocument[document].append(view);
    }

    // find documents which lose all views
    const QVector<AbstractView*> allViews = viewManager()->views();
    for (AbstractView* view : allViews) {
        auto* document = view->findBaseModel<AbstractDocument*>();
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
