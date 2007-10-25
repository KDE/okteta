/*
    This file is part of the Okteta program, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/


#include "program.h"

// program
#include "kbytearraydocumentfactory.h"
#include "kbytearraydisplayfactory.h"
#include "mainwindow.h"
// kakao
#include <kdocumentmanager.h>
#include <kdocumentcreator.h>
#include <kdocumentsynchronizer.h>
#include <kviewmanager.h>
// KDE
#include <KLocale>
#include <KCmdLineArgs>
#include <KApplication>


OktetaProgram::OktetaProgram( int argc, char *argv[] )
 : mDocumentManager( new KDocumentManager() ),
   mViewManager( new KViewManager(mDocumentManager) )
{
    KByteArrayDocumentFactory *factory = new KByteArrayDocumentFactory();
    mDocumentManager->creator()->setDocumentFactory( factory );
    mDocumentManager->synchronizer()->setDocumentFactory( factory );

    mViewManager->setViewFactory( new KByteArrayDisplayFactory() );

    KCmdLineOptions programOptions;
    programOptions.add( "+[URL(s)]", ki18n("File(s) to open"), 0 );

    KCmdLineArgs::init( argc, argv, &mAboutData );
    KCmdLineArgs::addCmdLineOptions( programOptions );
}


int OktetaProgram::execute()
{
    KApplication programCore;
    // started by session management?
//     if( isSessionRestored() )
//     {
//       RESTORE( OktetaMainWindow );
//     }
//     else
    {
        // no session.. just start up normally
        OktetaMainWindow *mainWindow = new OktetaMainWindow( this );
        mainWindow->show();

        KCmdLineArgs *arguments = KCmdLineArgs::parsedArgs();
#if 0
    // take arguments
    KCmdLineArgs *arguments = KCmdLineArgs::parsedArgs();
    if( Args->count() > 0 )
    {
        for( int i=0; i<Args->count(); ++i )
        ;
    }
#endif
//    if( arguments->isSet("offset") )
//    {
//        Q3CString offsetStr = args->getOption( "offset" );
//        uint _offset = parseDecimalOrHexadecimal(offsetStr.data() );
//        hexEdit->setStartupOffset( _offset );
//   }

    //for( int i = 0; i < args->count(); ++i )
    //  hexEdit->addStartupFile( args->url(i).url() );

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
//     delete mDocumentManager;
//     delete mViewManager;
}
