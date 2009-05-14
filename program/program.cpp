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

#include "program.h"

// program
#include "mainwindow.h"
// Okteta Kakao
#include <kbytearraydocumentfactory.h>
#include <kbytearraydisplayfactory.h>
#include <filesystem/kbytearrayrawfilesynchronizerfactory.h>
#include <bytearraysourcecodestreamencoderconfigeditorfactory.h>
#include <bytearrayvaluesstreamencoderconfigeditorfactory.h>
#include <kbytearraytextstreamencoder.h>
#include <kbytearrayvaluestreamencoder.h>
#include <kbytearraysourcecodestreamencoder.h>
#include <bytearrayviewtextstreamencoder.h>
// Kakao gui
#include <kviewmanager.h>
#include <modelcodecviewmanager.h>
// Kakao core
#include <kdocumentmanager.h>
#include <kdocumentcreatemanager.h>
#include <kdocumentsyncmanager.h>
#include <modelcodecmanager.h>
// KDE
#include <KLocale>
#include <KUrl>
#include <KCmdLineArgs>
#include <KApplication>
// Qt
#include <QtCore/QList>


// static const char OffsetOptionId[] = "offset";
// static const char OffsetOptionShortId[] = "o";


OktetaProgram::OktetaProgram( int argc, char *argv[] )
 : mDocumentManager( new KDocumentManager() ),
   mViewManager( new KViewManager(mDocumentManager) )
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

    QList<AbstractModelStreamEncoder*> encoderList;
    encoderList << new KByteArrayValueStreamEncoder()
                << new KByteArrayTextStreamEncoder()
                << new KByteArraySourceCodeStreamEncoder()
                << new ByteArrayViewTextStreamEncoder();

    QList<AbstractModelStreamEncoderConfigEditorFactory*> encoderConfigEditorFactoryList;
    encoderConfigEditorFactoryList << new ByteArraySourceCodeStreamEncoderConfigEditorFactory()
                                   << new ByteArrayValuesStreamEncoderConfigEditorFactory();

    mDocumentManager->codecManager()->setEncoders( encoderList );
    mDocumentManager->createManager()->setDocumentFactory( new KByteArrayDocumentFactory() );
    mDocumentManager->syncManager()->setDocumentSynchronizerFactory( new KByteArrayRawFileSynchronizerFactory() );

    mViewManager->setViewFactory( new KByteArrayDisplayFactory() );
    mViewManager->codecViewManager()->setEncoderConfigEditorFactories( encoderConfigEditorFactoryList );

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

            KDocumentSyncManager *syncManager = mDocumentManager->syncManager();
            for( int i=0; i<arguments->count(); ++i )
            {
                syncManager->load( arguments->url(i) );
                // TODO: set view to offset
                // if( offset != -1 )
            }
        }

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
