/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "multidocumentstrategy_p.hpp"

// lib
#include "createdialog.hpp"
// Kasten gui
#include <abstractmodeldatageneratorconfigeditor.hpp>
#include <modelcodecviewmanager.hpp>
#include <viewmanager.hpp>
// Kasten core
#include <Kasten/ModelDataGenerateThread>
#include <Kasten/AbstractModelDataGenerator>
#include <Kasten/AbstractModel>
// Qt
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QUrl>

namespace Kasten {

void MultiDocumentStrategyPrivate::init()
{
    Q_Q(MultiDocumentStrategy);

    // setup
    QObject::connect(mDocumentManager, &DocumentManager::added,
                     mViewManager, &ViewManager::createViewsFor);
    QObject::connect(mDocumentManager, &DocumentManager::closing,
                     mViewManager, &ViewManager::removeViewsFor);
    QObject::connect(mDocumentManager, &DocumentManager::added,
                     q, &MultiDocumentStrategy::added);
    QObject::connect(mDocumentManager, &DocumentManager::closing,
                     q, &MultiDocumentStrategy::closing);
    QObject::connect(mDocumentManager->syncManager(), &DocumentSyncManager::urlUsed,
                     q, &MultiDocumentStrategy::urlUsed);
}

void MultiDocumentStrategyPrivate::createNewFromClipboard()
{
    const QMimeData* mimeData = QApplication::clipboard()->mimeData(QClipboard::Clipboard);

    mDocumentManager->createManager()->createNewFromData(mimeData, true);
}

void MultiDocumentStrategyPrivate::createNewWithGenerator(AbstractModelDataGenerator* generator)
{
    Q_Q(MultiDocumentStrategy);

    AbstractModelDataGeneratorConfigEditor* configEditor =
        mViewManager->codecViewManager()->createConfigEditor(generator);

    if (configEditor) {
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

void MultiDocumentStrategyPrivate::triggerGeneration(AbstractModelDataGenerator* generator)
{
    Q_Q(MultiDocumentStrategy);

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

}
