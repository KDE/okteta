/*
    This file is part of the Okteta program, made within the KDE community.

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

#include "program.h"

// program
#include "mainwindow.h"
// Okteta Kasten
#include <bytearraydocumentfactory.h>
#include <bytearrayviewfactory.h>
#include <filesystem/bytearrayrawfilesynchronizerfactory.h>
#include <bytearraystreamencoderconfigeditorfactoryfactory.h>
#include <bytearraydatageneratorconfigeditorfactoryfactory.h>
#include <bytearraystreamencoderfactory.h>
#include <bytearraydatageneratorfactory.h>
// Kasten gui
#include <viewmanager.h>
#include <modelcodecviewmanager.h>
// Kasten core
#include <documentmanager.h>
#include <documentcreatemanager.h>
#include <documentsyncmanager.h>
#include <modelcodecmanager.h>
// KDE
#include <KLocale>
#include <KUrl>
#include <KCmdLineArgs>
#include <KApplication>
// Qt
#include <QtCore/QList>


namespace Kasten
{

// static const char OffsetOptionId[] = "offset";
// static const char OffsetOptionShortId[] = "o";


OktetaProgram::OktetaProgram( int argc, char *argv[] )
 : mDocumentManager( new DocumentManager() ),
   mViewManager( new ViewManager() )
{
    KCmdLineOptions programOptions;
//     programOptions.add( OffsetOptionShortId );
//     programOptions.add( OffsetOptionId, ki18n("Offset to set the cursor to"), 0 );
    programOptions.add( "+[URL(s)]", ki18n("File(s) to load"), 0 );

    KCmdLineArgs::init( argc, argv, &mAboutData );
    KCmdLineArgs::addCmdLineOptions( programOptions );
}


int OktetaProgram::execute()
{
    KApplication programCore;

    KGlobal::locale()->insertCatalog( "liboktetacore" );
    KGlobal::locale()->insertCatalog( "libkasten" );
    KGlobal::locale()->insertCatalog( "liboktetakasten" );

    const QList<AbstractModelStreamEncoder*> encoderList =
        ByteArrayStreamEncoderFactory::createStreamEncoders();

    const QList<AbstractModelDataGenerator*> generatorList =
        ByteArrayDataGeneratorFactory::createDataGenerators();

    const QList<AbstractModelStreamEncoderConfigEditorFactory*> encoderConfigEditorFactoryList =
        ByteArrayStreamEncoderConfigEditorFactoryFactory::createFactorys();

    const QList<AbstractModelDataGeneratorConfigEditorFactory*> generatorConfigEditorFactoryList =
        ByteArrayDataGeneratorConfigEditorFactoryFactory::createFactorys();

    mDocumentManager->codecManager()->setEncoders( encoderList );
    mDocumentManager->codecManager()->setGenerators( generatorList );
    mDocumentManager->createManager()->setDocumentFactory( new ByteArrayDocumentFactory() );
    mDocumentManager->syncManager()->setDocumentSynchronizerFactory( new ByteArrayRawFileSynchronizerFactory() );

    mViewManager->setViewFactory( new ByteArrayViewFactory() );
    mViewManager->codecViewManager()->setEncoderConfigEditorFactories( encoderConfigEditorFactoryList );
    mViewManager->codecViewManager()->setGeneratorConfigEditorFactories( generatorConfigEditorFactoryList );

    // started by session management?
    if( programCore.isSessionRestored() )
    {
        RESTORE( OktetaMainWindow(this) );
    }
    else
    {
        // no session.. just start up normally
        OktetaMainWindow *mainWindow = new OktetaMainWindow( this );

        KCmdLineArgs *arguments = KCmdLineArgs::parsedArgs();

        // take arguments
        if( arguments->count() > 0 )
        {
//             int offset = -1;
//             if( arguments->isSet(OffsetOptionId) )
//             {
//                 const QString offsetOptionArgument = arguments->getOption( OffsetOptionId );
//                 offset = readOut( offsetOptionArgument );
//             }

            DocumentSyncManager* syncManager = mDocumentManager->syncManager();
            for( int i=0; i<arguments->count(); ++i )
            {
                syncManager->load( arguments->url(i) );
                // TODO: set view to offset
                // if( offset != -1 )
            }
        }

        mDocumentManager->setWidget( mainWindow );
        mainWindow->show();

        arguments->clear();
    }

    return programCore.exec();
}


void OktetaProgram::quit()
{
    kapp->quit();
}


OktetaProgram::~OktetaProgram()
{
    delete mDocumentManager;
    delete mViewManager;
}

}
