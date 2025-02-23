/*
    This file is part of the Okteta KPart module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2009, 2011, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "part.hpp"

// part
#include "browserextension.hpp"
// Okteta Kasten controllers
// #include <Kasten/Okteta/OverwriteOnlyControllerFactory>
#include <Kasten/Okteta/CropControllerFactory>
#include <Kasten/Okteta/OverwriteModeControllerFactory>
#include <Kasten/Okteta/GotoOffsetControllerFactory>
#include <Kasten/Okteta/SelectRangeControllerFactory>
#include <Kasten/Okteta/SearchControllerFactory>
#include <Kasten/Okteta/ReplaceControllerFactory>
// #include <Kasten/Okteta/BookmarksControllerFactory>
#include <Kasten/Okteta/PrintControllerFactory>
#include <Kasten/Okteta/ViewConfigControllerFactory>
#include <Kasten/Okteta/ViewModeControllerFactory>
#include <Kasten/Okteta/ViewContextMenuControllerFactory>
// #include <Kasten/Okteta/ViewStatusControllerFactory>
#include <Kasten/Okteta/ViewProfileControllerFactory>
// Okteta Kasten
#include <Kasten/Okteta/ByteArrayDocument>
#include <Kasten/Okteta/ByteArrayView>
#include <Kasten/Okteta/ByteArrayViewProfileSynchronizer>
#include <Kasten/Okteta/ByteArrayViewProfileManager>
#include <Kasten/Okteta/ByteArrayRawFileSynchronizerFactory>
// Kasten controllers
#include <Kasten/ReadOnlyControllerFactory>
// #include <Kasten/ReadOnlyBarController>
// #include <Kasten/SynchronizeController>
#include <Kasten/ClipboardControllerFactory>
#include <Kasten/InsertControllerFactory>
#include <Kasten/CopyAsControllerFactory>
#include <Kasten/ExportControllerFactory>
#include <Kasten/VersionControllerFactory>
#include <Kasten/ZoomControllerFactory>
// #include <Kasten/ZoomBarControllerFactory>
#include <Kasten/SelectControllerFactory>
// Kasten
#include <Kasten/SingleViewArea>
#include <Kasten/AbstractXmlGuiController>
#include <Kasten/JobManager>
#include <Kasten/AbstractLoadJob>
#include <Kasten/AbstractSyncWithRemoteJob>
#include <Kasten/UserMessagesHandler>
// KF
#include <KPluginMetaData>
// Qt
#include <QWidget>
#include <QVBoxLayout>
#include <QUrl>
// Std
#include <utility>

static constexpr const char* UIFileName[] =
{
    "oktetapartreadonlyui.rc",
    "oktetapartbrowserui.rc",
    "oktetapartreadwriteui.rc"
};

OktetaPart::OktetaPart(QObject* parent,
                       const KPluginMetaData& metaData,
                       Modus modus,
                       Kasten::ByteArrayViewProfileManager* viewProfileManager,
                       Kasten::UserMessagesHandler* userMessagesHandler,
                       Kasten::ModelCodecManager* modelCodecManager,
                       Kasten::ModelCodecViewManager* modelCodecViewManager)
    : KParts::ReadWritePart(parent)
    , mModus(modus)
    , mViewProfileManager(viewProfileManager)
{
    setMetaData(metaData);

    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);

    setWidget(widget);

    setXMLFile(QLatin1String(UIFileName[modus]));

    mSingleViewArea = std::make_unique<Kasten::SingleViewArea>();
    QWidget* areaWidget = mSingleViewArea->widget();
    layout->addWidget(areaWidget);
    layout->parentWidget()->setFocusProxy(areaWidget);

    if (modus == Modus::ReadWrite) {
        addController(Kasten::VersionControllerFactory());
    }
    if (modus == Modus::ReadWrite) {
        addController(Kasten::ReadOnlyControllerFactory());
    }
    // TODO: save_as
    addController(Kasten::ExportControllerFactory(modelCodecViewManager, modelCodecManager));
    addController(Kasten::ZoomControllerFactory());
    addController(Kasten::SelectControllerFactory());
    if (modus != Modus::BrowserView) {
        addController(Kasten::ClipboardControllerFactory());
    }
    if (modus == Modus::ReadWrite) {
        addController(Kasten::InsertControllerFactory(modelCodecViewManager, modelCodecManager));
    }
    addController(Kasten::CopyAsControllerFactory(modelCodecViewManager, modelCodecManager));
    if (modus == Modus::ReadWrite) {
        addController(Kasten::CropControllerFactory());
    }
    if (modus == Modus::ReadWrite) {
        addController(Kasten::OverwriteModeControllerFactory());
    }
    addController(Kasten::SearchControllerFactory(userMessagesHandler, widget));
    if (modus == Modus::ReadWrite) {
        addController(Kasten::ReplaceControllerFactory(userMessagesHandler, widget));
    }
    addController(Kasten::GotoOffsetControllerFactory(mSingleViewArea.get()));
    addController(Kasten::SelectRangeControllerFactory(mSingleViewArea.get()));
//     addController(Kasten::BookmarksControllerFactory());
    addController(Kasten::PrintControllerFactory(userMessagesHandler));
    addController(Kasten::ViewConfigControllerFactory());
    addController(Kasten::ViewModeControllerFactory());
    addController(Kasten::ViewContextMenuControllerFactory());
    addController(Kasten::ViewProfileControllerFactory(mViewProfileManager, widget));

//     Kasten::StatusBar* bottomBar = static_cast<Kasten::StatusBar*>( statusBar() );
//     addController(ViewStatusControllerFactory(bottomBar) );
//     addController(ReadOnlyBarControllerFactory(bottomBar) );
//     addController(ZoomBarControllerFactory(bottomBar) );

//     addTool( new DocumentInfoToolView(new DocumentInfoTool(mProgram->documentManager()->syncManager())) );
//     addTool( new ChecksumToolView(new ChecksumTool()) );
//     addTool( new FilterToolView(new FilterTool()) );
//     addTool( new StringsExtractToolView(new StringsExtractTool()) );
//     addTool( new ByteTableToolView(new ByteTableTool()) );
//     addTool( new InfoToolView(new InfoTool()) );
//     addTool( new PODDecoderToolView(new PODDecoderTool()) );
//     addTool( new BookmarksToolView(new BookmarksTool()) );

    // TODO: BrowserExtension might rely on existing objects (session snap while loadJob),
    // so this hack just creates some dummies
    mDocument = std::make_unique<Kasten::ByteArrayDocument>(QString());
    auto viewProfileSynchronizer = std::make_unique<Kasten::ByteArrayViewProfileSynchronizer>(viewProfileManager);
    mByteArrayView = std::make_unique<Kasten::ByteArrayView>(mDocument.get(), std::move(viewProfileSynchronizer));

    if (modus == Modus::BrowserView) {
        new OktetaBrowserExtension(this);
    }
}

OktetaPart::~OktetaPart() = default;

void OktetaPart::addController(const Kasten::AbstractXmlGuiControllerFactory& factory)
{
    auto controller = factory.create(this);
    mControllers.emplace_back(std::move(controller));
}

void OktetaPart::setReadWrite(bool readWrite)
{
    mDocument->setReadOnly(!readWrite);

    KParts::ReadWritePart::setReadWrite(readWrite);   // TODO: call to super needed?
}

bool OktetaPart::openFile()
{
    const auto synchronizerFactory = std::make_unique<Kasten::ByteArrayRawFileSynchronizerFactory>();

    Kasten::AbstractLoadJob* loadJob = synchronizerFactory->startLoad(QUrl::fromLocalFile(localFilePath()));
    connect(loadJob, &KJob::result,
            this, &OktetaPart::onDocumentLoadJobResult);
    const bool loadSuccess = Kasten::JobManager::executeJob(loadJob);

    return loadSuccess;
}

bool OktetaPart::saveFile()
{
    Kasten::AbstractModelSynchronizer* synchronizer = mDocument->synchronizer();

    Kasten::AbstractSyncWithRemoteJob* syncJob =
        synchronizer->startSyncWithRemote(QUrl::fromLocalFile(localFilePath()), Kasten::AbstractModelSynchronizer::ReplaceRemote);
    const bool syncSucceeded = Kasten::JobManager::executeJob(syncJob);

    return syncSucceeded;
}

void OktetaPart::onDocumentLoadJobResult(KJob* job)
{
    auto* loadJob = qobject_cast<Kasten::AbstractLoadJob*>(job);
    auto document = loadJob->releaseDocument();

    if (document) {
        for (const auto& controller : mControllers) {
            controller->setTargetModel(nullptr);
        }
        mSingleViewArea->setView(nullptr);
        mByteArrayView.reset();

        // is there a nicer way to upcycle a unique_ptr properly?
        mDocument.reset(static_cast<Kasten::ByteArrayDocument*>(document.release()));
        mDocument->setReadOnly(mModus != Modus::ReadWrite);
        connect(mDocument->synchronizer(), &Kasten::AbstractModelSynchronizer::localSyncStateChanged,
                this, &OktetaPart::onModified);

        auto viewProfileSynchronizer = std::make_unique<Kasten::ByteArrayViewProfileSynchronizer>(mViewProfileManager);
        viewProfileSynchronizer->setViewProfileId(mViewProfileManager->defaultViewProfileId());

        mByteArrayView = std::make_unique<Kasten::ByteArrayView>(mDocument.get(), std::move(viewProfileSynchronizer));
        connect(mByteArrayView.get(), SIGNAL(hasSelectedDataChanged(bool)), SIGNAL(hasSelectedDataChanged(bool)));

        mSingleViewArea->setView(mByteArrayView.get());

        for (const auto& controller : mControllers) {
            controller->setTargetModel(mByteArrayView.get());
        }

        setModified(false);
    }
}

void OktetaPart::onModified(Kasten::LocalSyncState state)
{
    const bool isModified = (state != Kasten::LocalInSync);
    setModified(isModified);
}

#include "moc_part.cpp"
