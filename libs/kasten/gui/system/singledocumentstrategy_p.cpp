/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "singledocumentstrategy_p.hpp"

// lib
#include "createdialog.hpp"
// Kasten gui
#include <abstractmodeldatageneratorconfigeditor.hpp>
#include <modelcodecviewmanager.hpp>
#include <viewmanager.hpp>
// Kasten core
#include <Kasten/ModelDataGenerateThread>
#include <Kasten/DocumentSyncManager>
#include <Kasten/DocumentCreateManager>
#include <Kasten/AbstractModelDataGenerator>
#include <Kasten/AbstractModel>
// KF
#include <KProcess>
// Qt
#include <QMimeData>
#include <QClipboard>
#include <QUrl>
#include <QApplication>

namespace Kasten {

void SingleDocumentStrategyPrivate::init()
{
    Q_Q(SingleDocumentStrategy);
    // setup
    QObject::connect(mDocumentManager, &DocumentManager::added,
                     mViewManager, &ViewManager::createViewsFor);
    QObject::connect(mDocumentManager, &DocumentManager::closing,
                     mViewManager, &ViewManager::removeViewsFor);
    QObject::connect(mDocumentManager, &DocumentManager::added,
                     q, &SingleDocumentStrategy::added);
    QObject::connect(mDocumentManager, &DocumentManager::closing,
                     q, &SingleDocumentStrategy::closing);
    QObject::connect(mDocumentManager->syncManager(), &DocumentSyncManager::urlUsed,
                     q, &SingleDocumentStrategy::urlUsed);
}

void SingleDocumentStrategyPrivate::createNew()
{
    if (mDocumentManager->isEmpty()) {
        mDocumentManager->createManager()->createNew();
    } else {
        const QString executable = QCoreApplication::applicationFilePath();
        // TODO: get parameters from common place with KCmdLineOptions
        // TODO: forward also interesting parameters passed to this program
        const QStringList parameters { QStringLiteral("-c") };
        KProcess::startDetached(executable, parameters);
    }
}

void SingleDocumentStrategyPrivate::createNewFromClipboard()
{
    if (mDocumentManager->isEmpty()) {
        const QMimeData* mimeData =
            QApplication::clipboard()->mimeData(QClipboard::Clipboard);

        mDocumentManager->createManager()->createNewFromData(mimeData, true);
    } else {
        const QString executable = QCoreApplication::applicationFilePath();
        // TODO: get parameters from common place with KCmdLineOptions
        // TODO: forward also interesting parameters passed to this program
        const QStringList parameters {
            QStringLiteral("-c"),
            QStringLiteral("-g"),
            QStringLiteral("FromClipboard"),
        };
        KProcess::startDetached(executable, parameters);
    }
}

void SingleDocumentStrategyPrivate::createNewWithGenerator(AbstractModelDataGenerator* generator)
{
    Q_Q(SingleDocumentStrategy);

    // TODO: show dialog in this process, meanwhile start other process, but hidden,
    // on result of dialog pass on the parameters
    if (!mDocumentManager->isEmpty()) {
        const QString executable = QCoreApplication::applicationFilePath();
        // TODO: get parameters from common place with KCmdLineOptions
        // TODO: forward also interesting parameters passed to this program
        // TODO: add id to AbstractModelDataGenerator, to use instead of className
        const QStringList parameters {
            QStringLiteral("-c"),
            QStringLiteral("-g"),
            QLatin1String(generator->metaObject()->className()),
        };
        KProcess::startDetached(executable, parameters);
        return;
    }

    AbstractModelDataGeneratorConfigEditor* configEditor =
        mViewManager->codecViewManager()->createConfigEditor(generator);

    if (configEditor) {
        // TODO: make dialog abstract for different UIs
        auto* dialog = new CreateDialog(configEditor, generator, QApplication::activeWindow());
//         dialog->setData( mModel, selection ); TODO
        QObject::connect(dialog, &CreateDialog::createAccepted, q, [this](AbstractModelDataGenerator* generator) {
            triggerGeneration(generator);
        });
        dialog->open();
        return;
    }

    triggerGeneration(generator);
}

void SingleDocumentStrategyPrivate::triggerGeneration(AbstractModelDataGenerator* generator)
{
    Q_Q(SingleDocumentStrategy);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    auto* generateThread = new ModelDataGenerateThread(q, generator);
    generateThread->start();
    while (!generateThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    QMimeData* mimeData = generateThread->data();

    delete generateThread;

    const bool setModified = (generator->flags() & AbstractModelDataGenerator::DynamicGeneration);
    mDocumentManager->createManager()->createNewFromData(mimeData, setModified);

    QApplication::restoreOverrideCursor();
}

void SingleDocumentStrategyPrivate::load(const QUrl& url)
{
    if (mDocumentManager->isEmpty()) {
        mDocumentManager->syncManager()->load(url);
    } else {
        const QString executable = QCoreApplication::applicationFilePath();
        // TODO: get parameters from common place with KCmdLineOptions
        // TODO: forward also interesting parameters passed to this program
        const QStringList parameters { url.url() };
        KProcess::startDetached(executable, parameters);
    }
}

}
