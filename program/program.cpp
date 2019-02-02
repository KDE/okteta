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

#include "program.hpp"

// program
#include "about.hpp"
#include "mainwindow.hpp"
// Okteta Kasten
#include <kasten/okteta/bytearraydocumentfactory.hpp>
#include <kasten/okteta/bytearrayviewfactory.hpp>
#include <kasten/okteta/bytearrayrawfilesynchronizerfactory.hpp>
#include <kasten/okteta/bytearraystreamencoderconfigeditorfactoryfactory.hpp>
#include <kasten/okteta/bytearraydatageneratorconfigeditorfactoryfactory.hpp>
#include <kasten/okteta/bytearraystreamencoderfactory.hpp>
#include <kasten/okteta/bytearraydatageneratorfactory.hpp>
// tmp
#include <kasten/okteta/bytearrayviewprofilemanager.hpp>
// Kasten gui
#include <kasten/multidocumentstrategy.hpp>
#include <kasten/dialoghandler.hpp>
#include <kasten/viewmanager.hpp>
#include <kasten/modelcodecviewmanager.hpp>
// Kasten core
#include <kasten/documentmanager.hpp>
#include <kasten/documentcreatemanager.hpp>
#include <kasten/documentsyncmanager.hpp>
#include <kasten/modelcodecmanager.hpp>
// KF5
#include <KDBusService>
#include <KLocalizedString>
#include <KCrash>
// Qt
#include <QCommandLineParser>
#include <QList>
#include <QUrl>
#include <QDir>
#include <QLoggingCategory>

namespace Kasten {

// static const char OffsetOptionId[] = "offset";
// static const char OffsetOptionShortId[] = "o";

static
int& preConstructionHookHack(int& argc)
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    return argc;
}


OktetaProgram::OktetaProgram(int& argc, char* argv[])
    : mApp(preConstructionHookHack(argc), argv)
{
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    KLocalizedString::setApplicationDomain("okteta");
    KCrash::initialize();
}

OktetaProgram::~OktetaProgram()
{
    delete mDocumentStrategy;
    delete mDocumentManager;
    delete mViewManager;
    delete mDialogHandler;
    delete mByteArrayViewProfileManager;
}

int OktetaProgram::execute()
{
    mDocumentManager = new DocumentManager();
    mViewManager = new ViewManager();
    mDocumentStrategy = new MultiDocumentStrategy(mDocumentManager, mViewManager);
    mDialogHandler = new DialogHandler();

    OktetaAboutData aboutData;
    KAboutData::setApplicationData(aboutData);
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("okteta")));

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);

    // urls to open
    parser.addPositionalArgument(QStringLiteral("urls"), i18n("File(s) to load."), QStringLiteral("[urls...]"));

    // offset option
//     programOptions.add( OffsetOptionShortId );
//     programOptions.add( OffsetOptionId, ki18n("Offset to set the cursor to"), 0 );

    parser.process(mApp);

    aboutData.processCommandLine(&parser);

    KDBusService programDBusService(KDBusService::Multiple | KDBusService::NoExitOnFailure);

    // TODO:
    mByteArrayViewProfileManager = new ByteArrayViewProfileManager();
    // mModelManagerManager->addModelManager( byteArrayViewProfileManager );

    const QList<AbstractModelStreamEncoder*> encoderList =
        ByteArrayStreamEncoderFactory::createStreamEncoders();

    const QList<AbstractModelDataGenerator*> generatorList =
        ByteArrayDataGeneratorFactory::createDataGenerators();

    const QList<AbstractModelStreamEncoderConfigEditorFactory*> encoderConfigEditorFactoryList =
        ByteArrayStreamEncoderConfigEditorFactoryFactory::createFactorys();

    const QList<AbstractModelDataGeneratorConfigEditorFactory*> generatorConfigEditorFactoryList =
        ByteArrayDataGeneratorConfigEditorFactoryFactory::createFactorys();

    mDocumentManager->codecManager()->setEncoders(encoderList);
    mDocumentManager->codecManager()->setGenerators(generatorList);
    mDocumentManager->codecManager()->setOverwriteDialog(mDialogHandler);
    mDocumentManager->createManager()->setDocumentFactory(new ByteArrayDocumentFactory());
    mDocumentManager->syncManager()->setDocumentSynchronizerFactory(new ByteArrayRawFileSynchronizerFactory());
    mDocumentManager->syncManager()->setOverwriteDialog(mDialogHandler);
    mDocumentManager->syncManager()->setSaveDiscardDialog(mDialogHandler);

    mViewManager->setViewFactory(new ByteArrayViewFactory(mByteArrayViewProfileManager));
    mViewManager->codecViewManager()->setEncoderConfigEditorFactories(encoderConfigEditorFactoryList);
    mViewManager->codecViewManager()->setGeneratorConfigEditorFactories(generatorConfigEditorFactoryList);

    OktetaMainWindow* mainWindow = new OktetaMainWindow(this);
    mDialogHandler->setWidget(mainWindow);

    // started by session management?
    if (mApp.isSessionRestored() && KMainWindow::canBeRestored(1)) {
        mainWindow->restore(1);
    } else {
        // no session.. just start up normally
        const QStringList urls = parser.positionalArguments();

        // take arguments
        if (!urls.isEmpty()) {
            const QString currentPath = QDir::currentPath();
            for (const QString& url : urls) {
                mDocumentStrategy->load(QUrl::fromUserInput(url, currentPath, QUrl::AssumeLocalFile));
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

}
