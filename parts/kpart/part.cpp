/*
    This file is part of the Okteta KPart module, part of the KDE project.

    Copyright 2003,2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <bytearraydocument.h>
#include <bytearrayview.h>
#include <filesystem/bytearrayrawfilesynchronizerfactory.h>
#include <overwriteonly/overwriteonlycontroller.h>
#include <overwritemode/overwritemodecontroller.h>
#include <insertpattern/insertpatterncontroller.h>
#include <insertpattern/insertsequencecontroller.h>
#include <gotooffset/gotooffsetcontroller.h>
#include <selectrange/selectrangecontroller.h>
#include <search/searchcontroller.h>
#include <replace/replacecontroller.h>
#include <bookmarks/bookmarkscontroller.h>
#include <print/printcontroller.h>
#include <viewconfig/viewconfigcontroller.h>
#include <viewmode/viewmodecontroller.h>
#include <viewstatus/viewstatuscontroller.h>
// Kasten
#include <jobmanager.h>
#include <abstractloadjob.h>
#include <abstractsyncwithremotejob.h>
#include <document/readonly/readonlycontroller.h>
// #include <document/readonly/readonlybarcontroller.h>
// #include <io/synchronize/synchronizecontroller.h>
#include <io/clipboard/clipboardcontroller.h>
#include <io/copyas/copyascontroller.h>
#include <io/export/exportcontroller.h>
#include <view/version/versioncontroller.h>
#include <view/zoom/zoomcontroller.h>
#include <view/zoom/zoombarcontroller.h>
#include <view/select/selectcontroller.h>
// KDE
#include <KActionCollection>
// Qt
#include <QtGui/QWidget>
#include <QtGui/QLayout>
#include <QtCore/QList>


static const char* UIFileName[] =
{
    "oktetapartreadonlyui.rc",
    "oktetapartbrowserui.rc",
    "oktetapartreadwriteui.rc"
};


OktetaPart::OktetaPart( QObject* parent,
                        const KComponentData& componentData,
                        Modus modus )
  : KParts::ReadWritePart( parent ),
    mModus( modus )
{
    setComponentData( componentData );

    QWidget* widget = new QWidget();
    mLayout = new QVBoxLayout( widget );
    mLayout->setMargin( 0 );

    setWidget( widget );
    setAutoDeleteWidget( true );

    setXMLFile( UIFileName[modus] );

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
//     mControllers.append( new Kasten::CopyAsController(mProgram->viewManager(),mProgram->documentManager(),this) );
    if( modus == ReadWriteModus )
        mControllers.append( new Kasten::OverwriteModeController(this) );
    mControllers.append( new Kasten::SearchController(this,widget) );
    if( modus == ReadWriteModus )
        mControllers.append( new Kasten::ReplaceController(this,widget) );
//     mControllers.append( new Kasten::GotoOffsetController(mGroupedViews,this) );
//     mControllers.append( new Kasten::SelectRangeController(mGroupedViews,this) );
    if( modus == ReadWriteModus )
        mControllers.append( new Kasten::InsertPatternController(this) );
    if( modus == ReadWriteModus )
        mControllers.append( new Kasten::InsertSequenceController(this) );
//     mControllers.append( new Kasten::BookmarksController(this) );
    mPrintController = new Kasten::PrintController( this );
    mControllers.append( mPrintController );
    mControllers.append( new Kasten::ViewConfigController(this) );
    mControllers.append( new Kasten::ViewModeController(this) );

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
    mByteArrayView = new Kasten::ByteArrayView( mDocument );

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

    Kasten::AbstractLoadJob* loadJob = synchronizer->startLoad( localFilePath() );
    connect( loadJob, SIGNAL(documentLoaded( Kasten::AbstractDocument* )), SLOT(onDocumentLoaded( Kasten::AbstractDocument* )) );
    Kasten::JobManager::executeJob( loadJob, widget() );

    delete synchronizerFactory;

    return true;
}

bool OktetaPart::saveFile()
{
    Kasten::AbstractModelSynchronizer* synchronizer = mDocument->synchronizer();

    Kasten::AbstractSyncWithRemoteJob *syncJob =
        synchronizer->startSyncWithRemote( localFilePath(), Kasten::AbstractModelSynchronizer::ReplaceRemote );
    const bool syncSucceeded = Kasten::JobManager::executeJob( syncJob, widget() );

    return syncSucceeded;
}


void OktetaPart::onDocumentLoaded( Kasten::AbstractDocument* document )
{
    if( document )
    {
        delete mByteArrayView;
        delete mDocument;

        mDocument = static_cast<Kasten::ByteArrayDocument*>( document );
        mDocument->setReadOnly( mModus != ReadWriteModus );
        connect( mDocument, SIGNAL(syncStatesChanged( Kasten::AbstractDocument::SyncStates )),
                 SLOT(onModified( int )) );

        mByteArrayView = new Kasten::ByteArrayView( mDocument );
//     mByteArrayView->setNoOfBytesPerLine( 16 );
        mByteArrayView->setShowsNonprinting( false );
        connect( mByteArrayView, SIGNAL(hasSelectedDataChanged( bool )), SIGNAL(hasSelectedDataChanged( bool )) );

        QWidget* displayWidget = mByteArrayView->widget();
        mLayout->addWidget( displayWidget );
        mLayout->parentWidget()->setFocusProxy( displayWidget );

        foreach( Kasten::AbstractXmlGuiController* controller, mControllers )
            controller->setTargetModel( mByteArrayView );

        setModified( false );
    }
}


void OktetaPart::onModified( int states )
{
    Kasten::AbstractDocument::SyncStates syncStates = (Kasten::AbstractDocument::SyncStates)( states );

    const bool isModified = ( syncStates != Kasten::AbstractDocument::InSync );
    setModified( isModified );
}

OktetaPart::~OktetaPart()
{
    qDeleteAll( mControllers );
    delete mByteArrayView;
    delete mDocument;
}
