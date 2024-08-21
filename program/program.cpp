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
#include <Kasten/Okteta/AbstractByteArrayStreamEncoder>
#include <Kasten/Okteta/ByteArrayStreamEncoderFactory>
#include <Kasten/Okteta/ByteArrayDataGeneratorFactory>
// tmp
#include <Kasten/Okteta/ByteArrayViewProfileManager>
// Kasten gui
#include <Kasten/AbstractModelDataGeneratorConfigEditorFactory>
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>
#include <Kasten/MultiDocumentStrategy>
#include <Kasten/DialogHandler>
#include <Kasten/ViewManager>
#include <Kasten/ModelCodecViewManager>
// Kasten core
#include <Kasten/AbstractModelDataGenerator>
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
#include <QUrl>
#include <QDir>

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

    OktetaAboutData aboutData;
    KAboutData::setApplicationData(aboutData);
    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("okteta")));

    KCrash::initialize();
}

OktetaProgram::~OktetaProgram() = default;

int OktetaProgram::execute()
{
    mDocumentManager = std::make_unique<DocumentManager>();
    mViewManager = std::make_unique<ViewManager>();
    mDocumentStrategy = std::make_unique<MultiDocumentStrategy>(mDocumentManager.get(), mViewManager.get());
    mDialogHandler = std::make_unique<DialogHandler>();

    KAboutData aboutData = KAboutData::applicationData();
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
    mByteArrayViewProfileManager = std::make_unique<ByteArrayViewProfileManager>();
    // mModelManagerManager->addModelManager( byteArrayViewProfileManager );

    auto streamEncoderList = ByteArrayStreamEncoderFactory::createStreamEncoders();

    auto dataGeneratorList = ByteArrayDataGeneratorFactory::createDataGenerators();

    auto encoderConfigEditorFactoryList =
        ByteArrayStreamEncoderConfigEditorFactoryFactory::createFactorys();

    auto generatorConfigEditorFactoryList =
        ByteArrayDataGeneratorConfigEditorFactoryFactory::createFactorys();

    mDocumentManager->codecManager()->setStreamEncoders(std::move(streamEncoderList));
    mDocumentManager->codecManager()->setDataGenerators(std::move(dataGeneratorList));
    mDocumentManager->codecManager()->setOverwriteDialog(mDialogHandler.get());
    mDocumentManager->createManager()->setDocumentFactory(new ByteArrayDocumentFactory());
    mDocumentManager->syncManager()->setDocumentSynchronizerFactory(new ByteArrayRawFileSynchronizerFactory());
    mDocumentManager->syncManager()->setOverwriteDialog(mDialogHandler.get());
    mDocumentManager->syncManager()->setSaveDiscardDialog(mDialogHandler.get());

    mViewManager->setViewFactory(new ByteArrayViewFactory(mByteArrayViewProfileManager.get()));
    mViewManager->codecViewManager()->setStreamEncoderConfigEditorFactories(std::move(encoderConfigEditorFactoryList));
    mViewManager->codecViewManager()->setDataGeneratorConfigEditorFactories(std::move(generatorConfigEditorFactoryList));

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
