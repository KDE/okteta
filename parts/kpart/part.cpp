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
#include <kbytearraydocument.h>
#include <kbytearraydisplay.h>
#include <filesystem/kbytearrayrawfilesynchronizerfactory.h>
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
// Kasten
#include <jobmanager.h>
#include <abstractloadjob.h>
#include <document/readonly/readonlycontroller.h>
// #include <document/readonly/readonlybarcontroller.h>
// #include <io/synchronize/synchronizecontroller.h>
#include <io/clipboard/clipboardcontroller.h>
#include <io/copyas/copyascontroller.h>
#include <io/export/exportcontroller.h>
#include <view/zoom/zoomcontroller.h>
#include <view/zoom/zoombarcontroller.h>
#include <view/select/selectcontroller.h>
// KDE
#include <KLocale>
#include <KActionCollection>
// Qt
#include <QtGui/QWidget>
#include <QtGui/QLayout>
#include <QtCore/QList>


static const char ReadOnlyUIFileName[] =  "oktetapartreadonlyui.rc";
static const char BrowserUIFileName[] =   "oktetapartbrowserui.rc";


OktetaPart::OktetaPart( QWidget* parentWidget,
                        QObject* parent,
                        bool browserViewWanted )
  : KParts::ReadOnlyPart( parent )
{
    setComponentData( OktetaPartFactory::componentData() );

    KGlobal::locale()->insertCatalog( "liboktetacore" );
    KGlobal::locale()->insertCatalog( "libkastencore" );
    KGlobal::locale()->insertCatalog( "libkastengui" );
    KGlobal::locale()->insertCatalog( "libkastencontroller" );

//     QList<AbstractModelStreamEncoder*> encoderList;
//     encoderList << new ByteArrayValueStreamEncoder()
//                 << new ByteArrayTextStreamEncoder()
//                 << new ByteArraySourceCodeStreamEncoder()
//                 << new ByteArrayViewTextStreamEncoder();

//     QList<AbstractModelStreamEncoderConfigEditorFactory*> encoderConfigEditorFactoryList;
//     encoderConfigEditorFactoryList << new ByteArraySourceCodeStreamEncoderConfigEditorFactory()
//                                    << new ByteArrayValuesStreamEncoderConfigEditorFactory();

//     mDocumentManager->codecManager()->setEncoders( encoderList );
//     mDocumentManager->syncManager()->setDocumentSynchronizerFactory( new KByteArrayRawFileSynchronizerFactory() );

//     mViewManager->codecViewManager()->setEncoderConfigEditorFactories( encoderConfigEditorFactoryList );


    QWidget* widget = new QWidget();
    mLayout = new QVBoxLayout( widget );
    mLayout->setMargin( 0 );

    setWidget( widget );
    setAutoDeleteWidget( true );

    setXMLFile( browserViewWanted ? BrowserUIFileName : ReadOnlyUIFileName );

    // TODO: save_as
//     mControllers.append( new ExportController(mProgram->viewManager(),mProgram->documentManager(),this) );
    mControllers.append( new Kasten::ZoomController(this) );
    mControllers.append( new Kasten::SelectController(this) );
    if( ! browserViewWanted )
        mControllers.append( new Kasten::ClipboardController(this) );
//     mControllers.append( new Kasten::CopyAsController(mProgram->viewManager(),mProgram->documentManager(),this) );
//     mControllers.append( new Kasten::OverwriteModeController(this) );
    mControllers.append( new Kasten::SearchController(this,widget) );
//     mControllers.append( new Kasten::ReplaceController(this,widget) );
//     mControllers.append( new Kasten::GotoOffsetController(mGroupedViews,this) );
//     mControllers.append( new Kasten::SelectRangeController(mGroupedViews,this) );
//     mControllers.append( new Kasten::InsertPatternController(this) );
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
    mDocument = new Kasten::KByteArrayDocument( QString() );
    mDisplay = new Kasten::KByteArrayDisplay( mDocument );

    if( browserViewWanted )
        new OktetaBrowserExtension( this );
}

Kasten::PrintController* OktetaPart::printController() const { return mPrintController; }

bool OktetaPart::openFile()
{
    Kasten::KByteArrayRawFileSynchronizerFactory* synchronizerFactory = new Kasten::KByteArrayRawFileSynchronizerFactory();
    Kasten::AbstractModelSynchronizer* synchronizer = synchronizerFactory->createSynchronizer();

    Kasten::AbstractLoadJob* loadJob = synchronizer->startLoad( localFilePath() );
    connect( loadJob, SIGNAL(documentLoaded( Kasten::AbstractDocument* )), SLOT(onDocumentLoaded( Kasten::AbstractDocument* )) );
    Kasten::JobManager::executeJob( loadJob, widget() );

    delete synchronizerFactory;

    return true;
}

void OktetaPart::onDocumentLoaded( Kasten::AbstractDocument* document )
{
    if( document )
    {
        delete mDisplay;
        delete mDocument;

        mDocument = static_cast<Kasten::KByteArrayDocument*>( document );
        mDocument->setReadOnly( true );

        mDisplay = new Kasten::KByteArrayDisplay( mDocument );
//     mDisplay->setNoOfBytesPerLine( 16 );
        mDisplay->setShowsNonprinting( false );
        connect( mDisplay, SIGNAL(hasSelectedDataChanged( bool )), SIGNAL(hasSelectedDataChanged( bool )) );

        mLayout->addWidget( mDisplay->widget() );

        foreach( Kasten::AbstractXmlGuiController* controller, mControllers )
            controller->setTargetModel( mDisplay );
    }
}

OktetaPart::~OktetaPart()
{
    qDeleteAll( mControllers );
    delete mDisplay;
    delete mDocument;
}
