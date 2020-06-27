/*
    This file is part of the Okteta program, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009, 2011, 2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "program.hpp"

// program
#include "about.hpp"
#include "mainwindow.hpp"
// Okteta Kasten
#include <Kasten/Okteta/ByteArrayDocumentFactory>
#include <Kasten/Okteta/ByteArrayViewFactory>
#include <Kasten/Okteta/ByteArrayRawFileSynchronizerFactory>
#include <Kasten/Okteta/ByteArrayStreamEncoderConfigEditorFactoryFactory>
#include <Kasten/Okteta/ByteArrayDataGeneratorConfigEditorFactoryFactory>
#include <Kasten/Okteta/ByteArrayStreamEncoderFactory>
#include <Kasten/Okteta/ByteArrayDataGeneratorFactory>
// tmp
#include <Kasten/Okteta/ByteArrayViewProfileManager>
// Kasten gui
#include <Kasten/MultiDocumentStrategy>
#include <Kasten/DialogHandler>
#include <Kasten/ViewManager>
#include <Kasten/ModelCodecViewManager>
// Kasten core
#include <Kasten/DocumentManager>
#include <Kasten/DocumentCreateManager>
#include <Kasten/DocumentSyncManager>
#include <Kasten/ModelCodecManager>
// KF
#include <KDBusService>
#include <KLocalizedString>
#include <KCrash>
// Qt
#include <QCommandLineParser>
#include <QVector>
#include <QUrl>
#include <QDir>
#include <QLoggingCategory>

namespace Kasten {

// static constexpr char OffsetOptionId[] = "offset";
// static constexpr char OffsetOptionShortId[] = "o";

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

    const QVector<AbstractModelStreamEncoder*> encoderList =
        ByteArrayStreamEncoderFactory::createStreamEncoders();

    const QVector<AbstractModelDataGenerator*> generatorList =
        ByteArrayDataGeneratorFactory::createDataGenerators();

    const QVector<AbstractModelStreamEncoderConfigEditorFactory*> encoderConfigEditorFactoryList =
        ByteArrayStreamEncoderConfigEditorFactoryFactory::createFactorys();

    const QVector<AbstractModelDataGeneratorConfigEditorFactory*> generatorConfigEditorFactoryList =
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

    auto* mainWindow = new OktetaMainWindow(this);
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

    return QApplication::exec();
}

void OktetaProgram::quit()
{
    qApp->quit();
}

}
