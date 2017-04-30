/*
    This file is part of the Okteta program, made within the KDE community.

    Copyright 2006-2009,2011,2014 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include "about.h"
#include "mainwindow.h"
// Okteta Kasten
#include <kasten/okteta/bytearraydocumentfactory.h>
#include <kasten/okteta/bytearrayviewfactory.h>
#include <kasten/okteta/bytearrayrawfilesynchronizerfactory.h>
#include <kasten/okteta/bytearraystreamencoderconfigeditorfactoryfactory.h>
#include <kasten/okteta/bytearraydatageneratorconfigeditorfactoryfactory.h>
#include <kasten/okteta/bytearraystreamencoderfactory.h>
#include <kasten/okteta/bytearraydatageneratorfactory.h>
// tmp
#include <kasten/okteta/bytearrayviewprofilemanager.h>
// Kasten gui
#include <kasten/multidocumentstrategy.h>
#include <kasten/dialoghandler.h>
#include <kasten/viewmanager.h>
#include <kasten/modelcodecviewmanager.h>
// Kasten core
#include <kasten/documentmanager.h>
#include <kasten/documentcreatemanager.h>
#include <kasten/documentsyncmanager.h>
#include <kasten/modelcodecmanager.h>
// KF5
#include <KDBusService>
#include <KLocalizedString>
// Qt
#include <QCommandLineParser>
#include <QList>
#include <QUrl>
#include <QDir>
#include <QLoggingCategory>

namespace Kasten
{

// static const char OffsetOptionId[] = "offset";
// static const char OffsetOptionShortId[] = "o";

OktetaProgram::OktetaProgram( int &argc, char* argv[] )
  : mApp( argc, argv )
{
#if QT_VERSION < 0x050500
#ifndef QT_NO_DEBUG
    // MSVC complains about mismatched strings... (wide vs normal)
    // fix it using QT_UNICODE_LITERAL
    QLoggingCategory::setFilterRules(QStringLiteral(
        "okteta.core.debug = false\n"
        QT_UNICODE_LITERAL("okteta.gui.debug = false\n")
        QT_UNICODE_LITERAL("kasten.core.debug = false\n")
        QT_UNICODE_LITERAL("kasten.gui.debug = false\n")
        QT_UNICODE_LITERAL("kasten.okteta.core = false\n")
        QT_UNICODE_LITERAL("kasten.okteta.gui = false\n")
        QT_UNICODE_LITERAL("kasten.okteta.controllers.structures.debug = false\n")));
#endif
#endif

    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    KLocalizedString::setApplicationDomain( "okteta" );
}


int OktetaProgram::execute()
{
    mDocumentManager = new DocumentManager();
    mViewManager = new ViewManager();
    mDocumentStrategy = new MultiDocumentStrategy( mDocumentManager, mViewManager );
    mDialogHandler = new DialogHandler();

    OktetaAboutData aboutData;
    KAboutData::setApplicationData( aboutData );
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("okteta")));

    KDBusService programDBusService(KDBusService::Multiple | KDBusService::NoExitOnFailure);

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.addHelpOption();
    parser.addVersionOption();

    // urls to open
    parser.addPositionalArgument( QStringLiteral("urls"), i18n("File(s) to load."), QStringLiteral("[urls...]") );

    // offset option
//     programOptions.add( OffsetOptionShortId );
//     programOptions.add( OffsetOptionId, ki18n("Offset to set the cursor to"), 0 );

    parser.process(mApp);

    aboutData.processCommandLine(&parser);

    // TODO:
    mByteArrayViewProfileManager = new ByteArrayViewProfileManager();
    //mModelManagerManager->addModelManager( byteArrayViewProfileManager );

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
    mDocumentManager->codecManager()->setOverwriteDialog( mDialogHandler );
    mDocumentManager->createManager()->setDocumentFactory( new ByteArrayDocumentFactory() );
    mDocumentManager->syncManager()->setDocumentSynchronizerFactory( new ByteArrayRawFileSynchronizerFactory() );
    mDocumentManager->syncManager()->setOverwriteDialog( mDialogHandler );
    mDocumentManager->syncManager()->setSaveDiscardDialog( mDialogHandler );

    mViewManager->setViewFactory( new ByteArrayViewFactory(mByteArrayViewProfileManager) );
    mViewManager->codecViewManager()->setEncoderConfigEditorFactories( encoderConfigEditorFactoryList );
    mViewManager->codecViewManager()->setGeneratorConfigEditorFactories( generatorConfigEditorFactoryList );

    OktetaMainWindow* mainWindow = new OktetaMainWindow( this );
    mDialogHandler->setWidget( mainWindow );

    // started by session management?
    if( mApp.isSessionRestored() && KMainWindow::canBeRestored(1) )
    {
        mainWindow->restore( 1 );
    }
    else
    {
        // no session.. just start up normally
        const QStringList urls = parser.positionalArguments();

        // take arguments
        if( ! urls.isEmpty() )
        {
            const QRegExp withProtocolChecker( QStringLiteral("^[a-zA-Z]+:") );
            foreach (const QString &url, urls) {
                const QUrl u = (withProtocolChecker.indexIn(url) == 0) ?
                    QUrl::fromUserInput( url ) :
                    QUrl::fromLocalFile(QDir::current().absoluteFilePath(url));

                mDocumentStrategy->load( u );
            }
        }

        mainWindow->show();

    }

    return mApp.exec();
}


void OktetaProgram::quit()
{
    qApp->quit();
}


OktetaProgram::~OktetaProgram()
{
    delete mDocumentStrategy;
    delete mDocumentManager;
    delete mViewManager;
    delete mDialogHandler;
    delete mByteArrayViewProfileManager;
}

}
