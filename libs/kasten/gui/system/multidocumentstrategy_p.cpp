/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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
        auto* dialog = new CreateDialog(configEditor);
//         dialog->setData( mModel, selection ); TODO
        if (!dialog->exec()) {
            return;
        }
    }

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
