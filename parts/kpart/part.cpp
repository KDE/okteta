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

#include "part.h"

// part
#include "partfactory.h"
#include "browserextension.h"
// Okteta Kasten
#include <kasten/okteta/bytearraydocument.h>
#include <kasten/okteta/bytearrayview.h>
#include <kasten/okteta/bytearrayviewprofilesynchronizer.h>
#include <kasten/okteta/bytearrayviewprofilemanager.h>
#include <kasten/okteta/bytearrayrawfilesynchronizerfactory.h>
#include <kasten/okteta/overwriteonlycontroller.h>
#include <kasten/okteta/overwritemodecontroller.h>
#include <kasten/okteta/gotooffsetcontroller.h>
#include <kasten/okteta/selectrangecontroller.h>
#include <kasten/okteta/searchcontroller.h>
#include <kasten/okteta/replacecontroller.h>
#include <kasten/okteta/bookmarkscontroller.h>
#include <kasten/okteta/printcontroller.h>
#include <kasten/okteta/viewconfigcontroller.h>
#include <kasten/okteta/viewmodecontroller.h>
#include <kasten/okteta/viewstatuscontroller.h>
#include <kasten/okteta/viewprofilecontroller.h>
// Kasten
#include <kasten/jobmanager.h>
#include <kasten/abstractloadjob.h>
#include <kasten/abstractsyncwithremotejob.h>
#include <kasten/readonlycontroller.h>
// #include <kasten/readonlybarcontroller.h>
// #include <kasten/synchronizecontroller.h>
#include <kasten/clipboardcontroller.h>
#include <kasten/insertcontroller.h>
#include <kasten/copyascontroller.h>
#include <kasten/exportcontroller.h>
#include <kasten/versioncontroller.h>
#include <kasten/zoomcontroller.h>
#include <kasten/zoombarcontroller.h>
#include <kasten/selectcontroller.h>
// KF5
#include <KActionCollection>
// Qt
#include <QWidget>
#include <QLayout>
#include <QUrl>
#include <QList>


static const char* const UIFileName[] =
{
    "oktetapartreadonlyui.rc",
    "oktetapartbrowserui.rc",
    "oktetapartreadwriteui.rc"
};


OktetaPart::OktetaPart( QObject* parent,
                        const KAboutData& componentData,
                        Modus modus,
                        Kasten::ByteArrayViewProfileManager* viewProfileManager )
  : KParts::ReadWritePart( parent )
  , mModus( modus )
  , mViewProfileManager( viewProfileManager )
{
    setComponentData( componentData );

    QWidget* widget = new QWidget();
    mLayout = new QVBoxLayout( widget );
    mLayout->setMargin( 0 );

    setWidget( widget );

    setXMLFile( QLatin1String(UIFileName[modus]) );

    if( modus == ReadWriteModus )
        mControllers.append( new Kasten::VersionController(this) );
    if( modus == ReadWriteModus )
        mControllers.append( new Kasten::ReadOnlyController(this) );
    // TODO: save_as
//     mControllers.append( new ExportController(mProgram->viewManager(),mProgram->documentManager(),this) );
    mControllers.append( new Kasten::ZoomController(this) );
    mControllers.append( new Kasten::SelectController(this) );
    if( modus != BrowserViewModus )
        mControllers.append( new Kasten::ClipboardController(this) );
//     if( modus != BrowserViewModus )
//         mControllers.append( new Kasten::InsertController(mProgram->viewManager(),mProgram->documentManager(),this) );
//     mControllers.append( new Kasten::CopyAsController(mProgram->viewManager(),mProgram->documentManager(),this) );
    if( modus == ReadWriteModus )
        mControllers.append( new Kasten::OverwriteModeController(this) );
    mControllers.append( new Kasten::SearchController(this,widget) );
    if( modus == ReadWriteModus )
        mControllers.append( new Kasten::ReplaceController(this,widget) );
//     mControllers.append( new Kasten::GotoOffsetController(mGroupedViews,this) );
//     mControllers.append( new Kasten::SelectRangeController(mGroupedViews,this) );
//     mControllers.append( new Kasten::BookmarksController(this) );
    mPrintController = new Kasten::PrintController( this );
    mControllers.append( mPrintController );
    mControllers.append( new Kasten::ViewConfigController(this) );
    mControllers.append( new Kasten::ViewModeController(this) );
    mControllers.append( new Kasten::ViewProfileController(mViewProfileManager, widget, this) );

//     Kasten::StatusBar* bottomBar = static_cast<Kasten::StatusBar*>( statusBar() );
//     mControllers.append( new ViewStatusController(bottomBar) );
//     mControllers.append( new ReadOnlyBarController(bottomBar) );
//     mControllers.append( new ZoomBarController(bottomBar) );

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
    mDocument = new Kasten::ByteArrayDocument( QString() );
    Kasten::ByteArrayViewProfileSynchronizer* viewProfileSynchronizer =
        new Kasten::ByteArrayViewProfileSynchronizer( viewProfileManager );
    mByteArrayView = new Kasten::ByteArrayView( mDocument, viewProfileSynchronizer );

    if( modus == BrowserViewModus )
        new OktetaBrowserExtension( this );
}

Kasten::PrintController* OktetaPart::printController() const { return mPrintController; }


void OktetaPart::setReadWrite( bool readWrite )
{
    mDocument->setReadOnly( ! readWrite );

    KParts::ReadWritePart::setReadWrite( readWrite ); // TODO: call to super needed?
}

bool OktetaPart::openFile()
{
    Kasten::ByteArrayRawFileSynchronizerFactory* synchronizerFactory = new Kasten::ByteArrayRawFileSynchronizerFactory();
    Kasten::AbstractModelSynchronizer* synchronizer = synchronizerFactory->createSynchronizer();

    Kasten::AbstractLoadJob* loadJob = synchronizer->startLoad( QUrl::fromLocalFile( localFilePath() ) );
    connect( loadJob, &Kasten::AbstractLoadJob::documentLoaded,
             this, &OktetaPart::onDocumentLoaded );
    Kasten::JobManager::executeJob( loadJob );

    delete synchronizerFactory;

    return true;
}

bool OktetaPart::saveFile()
{
    Kasten::AbstractModelSynchronizer* synchronizer = mDocument->synchronizer();

    Kasten::AbstractSyncWithRemoteJob *syncJob =
        synchronizer->startSyncWithRemote( QUrl::fromLocalFile( localFilePath() ), Kasten::AbstractModelSynchronizer::ReplaceRemote );
    const bool syncSucceeded = Kasten::JobManager::executeJob( syncJob );

    return syncSucceeded;
}


void OktetaPart::onDocumentLoaded( Kasten::AbstractDocument* document )
{
    if( document )
    {
        foreach( Kasten::AbstractXmlGuiController* controller, mControllers )
            controller->setTargetModel( 0 );

        delete mByteArrayView;
        delete mDocument;

        mDocument = static_cast<Kasten::ByteArrayDocument*>( document );
        mDocument->setReadOnly( mModus != ReadWriteModus );
        connect( mDocument->synchronizer(), &Kasten::AbstractModelSynchronizer::localSyncStateChanged,
                 this, &OktetaPart::onModified );

        Kasten::ByteArrayViewProfileSynchronizer* viewProfileSynchronizer =
            new Kasten::ByteArrayViewProfileSynchronizer( mViewProfileManager );
        viewProfileSynchronizer->setViewProfileId( mViewProfileManager->defaultViewProfileId() );

        mByteArrayView = new Kasten::ByteArrayView( mDocument, viewProfileSynchronizer );
        connect( mByteArrayView, SIGNAL(hasSelectedDataChanged(bool)), SIGNAL(hasSelectedDataChanged(bool)) );

        QWidget* displayWidget = mByteArrayView->widget();
        mLayout->addWidget( displayWidget );
        mLayout->parentWidget()->setFocusProxy( displayWidget );

        foreach( Kasten::AbstractXmlGuiController* controller, mControllers )
            controller->setTargetModel( mByteArrayView );

        setModified( false );
    }
}


void OktetaPart::onModified( Kasten::LocalSyncState state )
{
    const bool isModified = ( state != Kasten::LocalInSync );
    setModified( isModified );
}

OktetaPart::~OktetaPart()
{
    qDeleteAll( mControllers );
    delete mByteArrayView;
    delete mDocument;
}
