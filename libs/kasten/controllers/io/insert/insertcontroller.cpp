/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "insertcontroller.hpp"

// lib
#include "insertdialog.hpp"
// Kasten gui
#include <Kasten/ModelCodecViewManager>
#include <Kasten/SelectedDataWriteable>
#include <Kasten/AbstractModelDataGeneratorConfigEditor>
// Kasten core
#include <Kasten/ModelDataGenerateThread>
#include <Kasten/ModelCodecManager>
#include <Kasten/AbstractModelDataGenerator>
#include <Kasten/AbstractModel>
// KF
#include <KXMLGUIClient>
#include <KXMLGUIFactory>
#include <KActionCollection>
#include <KLocalizedString>
#include <KSelectAction>
// Qt
#include <QMimeData>
#include <QApplication>

namespace Kasten {

InsertController::InsertController(ModelCodecViewManager* modelCodecViewManager,
                                   ModelCodecManager* modelCodecManager,
                                   KXMLGUIClient* guiClient)
    : mModelCodecViewManager(modelCodecViewManager)
    , mModelCodecManager(modelCodecManager)
{
    mInsertSelectAction = new KSelectAction(i18nc("@title:menu", "Insert"), this);
//     mInsertSelectAction->setIcon( QIcon::fromTheme( QStringLiteral("insert-text") ) );
    mInsertSelectAction->setToolBarMode(KSelectAction::MenuMode);
    connect(mInsertSelectAction, QOverload<QAction*>::of(&KSelectAction::triggered),
            this, &InsertController::onActionTriggered);

    // TODO: find better id
    guiClient->actionCollection()->addAction(QStringLiteral("insert"), mInsertSelectAction);

    setTargetModel(nullptr);
}

void InsertController::setTargetModel(AbstractModel* model)
{
    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = model ? model->findBaseModelWithInterface<If::SelectedDataWriteable*>() : nullptr;
    mSelectedDataWriteableControl = mModel ? qobject_cast<If::SelectedDataWriteable*>(mModel) : nullptr;

    if (mSelectedDataWriteableControl) {
        // TODO: only fill the list on menu call...
        connect(mModel, &AbstractModel::readOnlyChanged,
                this, &InsertController::onReadOnlyChanged);
    }

    updateActions();
}

void InsertController::updateActions()
{
    mInsertSelectAction->removeAllActions();

    // TODO: pass model to find which mimetypes it can read
    // mSelectedDataWriteableControl->canReadData( QMimeData() ) needs already data
    // TODO: it this depend on the current selection/focus? So it needs to be updated on every change?
    const QVector<AbstractModelDataGenerator*> generatorList =
        mModelCodecManager->generatorList();
    const bool hasGenerators = (!generatorList.isEmpty());

    if (hasGenerators) {
        for (AbstractModelDataGenerator* generator : generatorList) {
            const QString title = generator->typeName();
            auto* action = new QAction(title, mInsertSelectAction);

            action->setData(QVariant::fromValue(generator));
            mInsertSelectAction->addAction(action);
        }
    } else {
        QAction* noneAction = new QAction(i18nc("@item There are no generators.", "Not available."), mInsertSelectAction);
        noneAction->setEnabled(false);
        mInsertSelectAction->addAction(noneAction);
    }

    // TODO: need a call AbstractModelSelection::isEmpty
    const bool isWriteable = (mSelectedDataWriteableControl && !mModel->isReadOnly());
    mInsertSelectAction->setEnabled(isWriteable);
}

void InsertController::onActionTriggered(QAction* action)
{
    auto* generator = action->data().value<AbstractModelDataGenerator*>();

    AbstractModelDataGeneratorConfigEditor* configEditor =
        mModelCodecViewManager->createConfigEditor(generator);

    if (configEditor) {
        auto* dialog = new InsertDialog(configEditor, generator, QApplication::activeWindow());
//         dialog->setData( mModel, selection ); TODO
        connect(dialog, &InsertDialog::insertAccepted, this, &InsertController::triggerExecution);
        dialog->open();
        return;
    }

    triggerExecution(generator);
}

void InsertController::triggerExecution(AbstractModelDataGenerator* generator)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    auto* generateThread = new ModelDataGenerateThread(this, generator);
    generateThread->start();
    while (!generateThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    QMimeData* mimeData = generateThread->data();

    delete generateThread;

    mSelectedDataWriteableControl->insertData(mimeData);

    QApplication::restoreOverrideCursor();
}

void InsertController::onReadOnlyChanged(bool isReadOnly)
{
    const bool isWriteable = (!isReadOnly);

    mInsertSelectAction->setEnabled(isWriteable);
}

}
