/*
    This file is part of the Okteta KPart module, made within the KDE community.

    Copyright 2003,2007,2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "part.hpp"

// part
#include "partfactory.hpp"
#include "browserextension.hpp"
// Okteta Kasten controllers
#include <Kasten/Okteta/OverwriteOnlyControllerFactory>
#include <Kasten/Okteta/OverwriteModeControllerFactory>
#include <Kasten/Okteta/GotoOffsetControllerFactory>
#include <Kasten/Okteta/SelectRangeControllerFactory>
#include <Kasten/Okteta/SearchControllerFactory>
#include <Kasten/Okteta/ReplaceControllerFactory>
#include <Kasten/Okteta/BookmarksControllerFactory>
#include <Kasten/Okteta/PrintControllerFactory>
#include <Kasten/Okteta/ViewConfigControllerFactory>
#include <Kasten/Okteta/ViewModeControllerFactory>
#include <Kasten/Okteta/ViewStatusControllerFactory>
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
// #include <Kasten/InsertControllerFactory>
// #include <Kasten/CopyAsControllerFactory>
// #include <Kasten/ExportControllerFactory>
#include <Kasten/VersionControllerFactory>
#include <Kasten/ZoomControllerFactory>
// #include <Kasten/ZoomBarControllerFactory>
#include <Kasten/SelectControllerFactory>
// Kasten
#include <Kasten/AbstractXmlGuiController>
#include <Kasten/JobManager>
#include <Kasten/AbstractLoadJob>
#include <Kasten/AbstractSyncWithRemoteJob>
// Qt
#include <QWidget>
#include <QLayout>
#include <QUrl>

static constexpr const char* UIFileName[] =
{
    "oktetapartreadonlyui.rc",
    "oktetapartbrowserui.rc",
    "oktetapartreadwriteui.rc"
};

OktetaPart::OktetaPart(QObject* parent,
                       const KAboutData& componentData,
                       Modus modus,
                       Kasten::ByteArrayViewProfileManager* viewProfileManager)
    : KParts::ReadWritePart(parent)
    , mModus(modus)
    , mViewProfileManager(viewProfileManager)
{
    setComponentData(componentData);

    QWidget* widget = new QWidget();
    mLayout = new QVBoxLayout(widget);
    mLayout->setMargin(0);

    setWidget(widget);

    setXMLFile(QLatin1String(UIFileName[modus]));

    if (modus == Modus::ReadWrite) {
        addController(Kasten::VersionControllerFactory());
    }
    if (modus == Modus::ReadWrite) {
        addController(Kasten::ReadOnlyControllerFactory());
    }
    // TODO: save_as
//     addController(ExportControllerFactory(mProgram->viewManager(),mProgram->documentManager()));
    addController(Kasten::ZoomControllerFactory());
    addController(Kasten::SelectControllerFactory());
    if (modus != Modus::BrowserView) {
        addController(Kasten::ClipboardControllerFactory());
    }
//     if( modus != BrowserViewModus )
//         addController(Kasten::InsertControllerFactory(mProgram->viewManager(),mProgram->documentManager()));
//     addController(Kasten::CopyAsControllerFactory(mProgram->viewManager(),mProgram->documentManager()));
    if (modus == Modus::ReadWrite) {
        addController(Kasten::OverwriteModeControllerFactory());
    }
    addController(Kasten::SearchControllerFactory(widget));
    if (modus == Modus::ReadWrite) {
        addController(Kasten::ReplaceControllerFactory(widget));
    }
//     addController(Kasten::GotoOffsetControllerFactory(mGroupedViews));
//     addController(Kasten::SelectRangeControllerFactory(mGroupedViews));
//     addController(Kasten::BookmarksControllerFactory());
    addController(Kasten::PrintControllerFactory());
    addController(Kasten::ViewConfigControllerFactory());
    addController(Kasten::ViewModeControllerFactory());
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
    mDocument = new Kasten::ByteArrayDocument(QString());
    Kasten::ByteArrayViewProfileSynchronizer* viewProfileSynchronizer =
        new Kasten::ByteArrayViewProfileSynchronizer(viewProfileManager);
    mByteArrayView = new Kasten::ByteArrayView(mDocument, viewProfileSynchronizer);

    if (modus == Modus::BrowserView) {
        new OktetaBrowserExtension(this);
    }
}

OktetaPart::~OktetaPart()
{
    qDeleteAll(mControllers);
    delete mByteArrayView;
    delete mDocument;
}

void OktetaPart::addController(const Kasten::AbstractXmlGuiControllerFactory& factory)
{
    Kasten::AbstractXmlGuiController* controller = factory.create(this);
    mControllers.append(controller);
}

void OktetaPart::setReadWrite(bool readWrite)
{
    mDocument->setReadOnly(!readWrite);

    KParts::ReadWritePart::setReadWrite(readWrite);   // TODO: call to super needed?
}

bool OktetaPart::openFile()
{
    Kasten::ByteArrayRawFileSynchronizerFactory* synchronizerFactory = new Kasten::ByteArrayRawFileSynchronizerFactory();
    Kasten::AbstractModelSynchronizer* synchronizer = synchronizerFactory->createSynchronizer();

    Kasten::AbstractLoadJob* loadJob = synchronizer->startLoad(QUrl::fromLocalFile(localFilePath()));
    connect(loadJob, &Kasten::AbstractLoadJob::documentLoaded,
            this, &OktetaPart::onDocumentLoaded);
    Kasten::JobManager::executeJob(loadJob);

    delete synchronizerFactory;

    return true;
}

bool OktetaPart::saveFile()
{
    Kasten::AbstractModelSynchronizer* synchronizer = mDocument->synchronizer();

    Kasten::AbstractSyncWithRemoteJob* syncJob =
        synchronizer->startSyncWithRemote(QUrl::fromLocalFile(localFilePath()), Kasten::AbstractModelSynchronizer::ReplaceRemote);
    const bool syncSucceeded = Kasten::JobManager::executeJob(syncJob);

    return syncSucceeded;
}

void OktetaPart::onDocumentLoaded(Kasten::AbstractDocument* document)
{
    if (document) {
        for (Kasten::AbstractXmlGuiController* controller : qAsConst(mControllers)) {
            controller->setTargetModel(nullptr);
        }

        delete mByteArrayView;
        delete mDocument;

        mDocument = static_cast<Kasten::ByteArrayDocument*>(document);
        mDocument->setReadOnly(mModus != Modus::ReadWrite);
        connect(mDocument->synchronizer(), &Kasten::AbstractModelSynchronizer::localSyncStateChanged,
                this, &OktetaPart::onModified);

        Kasten::ByteArrayViewProfileSynchronizer* viewProfileSynchronizer =
            new Kasten::ByteArrayViewProfileSynchronizer(mViewProfileManager);
        viewProfileSynchronizer->setViewProfileId(mViewProfileManager->defaultViewProfileId());

        mByteArrayView = new Kasten::ByteArrayView(mDocument, viewProfileSynchronizer);
        connect(mByteArrayView, SIGNAL(hasSelectedDataChanged(bool)), SIGNAL(hasSelectedDataChanged(bool)));

        QWidget* displayWidget = mByteArrayView->widget();
        mLayout->addWidget(displayWidget);
        mLayout->parentWidget()->setFocusProxy(displayWidget);

        for (Kasten::AbstractXmlGuiController* controller : qAsConst(mControllers)) {
            controller->setTargetModel(mByteArrayView);
        }

        setModified(false);
    }
}

void OktetaPart::onModified(Kasten::LocalSyncState state)
{
    const bool isModified = (state != Kasten::LocalInSync);
    setModified(isModified);
}
