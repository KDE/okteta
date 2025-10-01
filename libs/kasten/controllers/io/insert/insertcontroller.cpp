/*
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
#include <KActionCollection>
#include <KLocalizedString>
#include <KSelectAction>
// Qt
#include <QMimeData>
#include <QApplication>
// Std
#include <utility>

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
    connect(mInsertSelectAction, qOverload<QAction*>(&KSelectAction::triggered),
            this, &InsertController::onActionTriggered);

    // TODO: find better id
    guiClient->actionCollection()->addAction(QStringLiteral("insert"), mInsertSelectAction);

    updateActions();
}

void InsertController::setTargetModel(AbstractModel* model)
{
    AbstractModel* const selectedDataWriteableModel = model ? model->findBaseModelWithInterface<If::SelectedDataWriteable*>() : nullptr;
    if (selectedDataWriteableModel == mModel) {
        return;
    }

    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = selectedDataWriteableModel;
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
    // mSelectedDataWriteableControl->canInsertData( QMimeData() ) needs already data
    // TODO: it this depend on the current selection/focus? So it needs to be updated on every change?
    const auto generatorList = mModelCodecManager->dataGenerators();
    const bool hasGenerators = (!generatorList.empty());

    if (hasGenerators) {
        for (AbstractModelDataGenerator* generator : generatorList) {
            const QString title = generator->typeName();
            auto* const action = new QAction(title, mInsertSelectAction);

            action->setData(QVariant::fromValue(generator));
            mInsertSelectAction->addAction(action);
        }
    } else {
        auto* const noneAction = new QAction(i18nc("@item There are no generators.", "Not available."), mInsertSelectAction);
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

    auto configEditor = mModelCodecViewManager->createConfigEditor(generator);

    if (configEditor) {
        auto* const dialog = new InsertDialog(std::move(configEditor), generator, QApplication::activeWindow());
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

    auto generateThread = std::make_unique<ModelDataGenerateThread>(this, generator);
    generateThread->start();
    while (!generateThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    auto mimeData = generateThread->releaseData();

    generateThread.reset();

    mSelectedDataWriteableControl->insertData(mimeData.get());

    QApplication::restoreOverrideCursor();
}

void InsertController::onReadOnlyChanged(bool isReadOnly)
{
    const bool isWriteable = (!isReadOnly);

    mInsertSelectAction->setEnabled(isWriteable);
}

}

#include "moc_insertcontroller.cpp"
