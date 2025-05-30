/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "copyascontroller.hpp"

// lib
#include "copyasdialog.hpp"
// Kasten gui
#include <Kasten/ModelCodecViewManager>
#include <Kasten/DataSelectable>
#include <Kasten/AbstractModelStreamEncoderConfigEditor>
// Kasten core
#include <Kasten/ModelStreamEncodeThread>
#include <Kasten/ModelCodecManager>
#include <Kasten/AbstractModelStreamEncoder>
#include <Kasten/AbstractModelSelection>
#include <Kasten/AbstractModel>
// KF
#include <KXMLGUIClient>
#include <KActionCollection>
#include <KLocalizedString>
#include <KSelectAction>
// Qt
#include <QBuffer>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>

namespace Kasten {

CopyAsController::CopyAsController(ModelCodecViewManager* modelCodecViewManager,
                                   ModelCodecManager* modelCodecManager,
                                   KXMLGUIClient* guiClient)
    : mModelCodecViewManager(modelCodecViewManager)
    , mModelCodecManager(modelCodecManager)
{
    mCopyAsSelectAction = new KSelectAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                                            i18nc("@title:menu", "Copy As"),
                                            this);
    mCopyAsSelectAction->setToolBarMode(KSelectAction::MenuMode);
    connect(mCopyAsSelectAction, qOverload<QAction*>(&KSelectAction::triggered),
            this, &CopyAsController::onActionTriggered);

    guiClient->actionCollection()->addAction(QStringLiteral("copy_as"), mCopyAsSelectAction);

    updateActions();
}

void CopyAsController::setTargetModel(AbstractModel* model)
{
    AbstractModel* const dataSelectableModel = model ? model->findBaseModelWithInterface<If::DataSelectable*>() : nullptr;
    if (dataSelectableModel == mModel) {
        return;
    }

    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = dataSelectableModel;
    mSelectionControl = mModel ? qobject_cast<If::DataSelectable*>(mModel) : nullptr;

    if (mSelectionControl) {
        // TODO: only fill the list on menu call...
        connect(mModel, SIGNAL(hasSelectedDataChanged(bool)), SLOT(updateActions()));
    }

    updateActions();
}

void CopyAsController::updateActions()
{
    mCopyAsSelectAction->removeAllActions();

    const AbstractModelSelection* selection = mSelectionControl ? mSelectionControl->modelSelection() : nullptr;

    const auto encoderList = mModelCodecManager->streamEncoders(mModel, selection);
    const bool hasEncoders = (!encoderList.empty());

    if (hasEncoders) {
        for (auto* encoder : encoderList) {
            const QString title = encoder->remoteTypeName();
            auto* action = new QAction(title, mCopyAsSelectAction);

            action->setData(QVariant::fromValue(encoder));
            mCopyAsSelectAction->addAction(action);
        }
    } else {
        auto* noneAction = new QAction(i18nc("@item There are no encoders.", "Not available."), mCopyAsSelectAction);
        noneAction->setEnabled(false);
        mCopyAsSelectAction->addAction(noneAction);
    }

    // TODO: need a call AbstractModelSelection::isEmpty
    mCopyAsSelectAction->setEnabled(mSelectionControl && mSelectionControl->hasSelectedData());
}

void CopyAsController::onActionTriggered(QAction* action)
{
    auto* encoder = action->data().value<AbstractModelStreamEncoder*>();

    const AbstractModelSelection* selection = mSelectionControl->modelSelection();

    auto configEditor = mModelCodecViewManager->createConfigEditor(encoder);

    if (configEditor) {
        auto* dialog = new CopyAsDialog(encoder->remoteTypeName(), std::move(configEditor), encoder,
                                        QApplication::activeWindow());
        dialog->setData(mModel, selection);
        connect(dialog, &CopyAsDialog::copyAccepted, this, &CopyAsController::triggerExecution);
        dialog->open();
        return;
    }

    triggerExecution(encoder, selection);
}

void CopyAsController::triggerExecution(AbstractModelStreamEncoder* encoder,
                                        const AbstractModelSelection* selection)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QByteArray exportData;
    QBuffer exportDataBuffer(&exportData);
    exportDataBuffer.open(QIODevice::WriteOnly);

    auto encodeThread = std::make_unique<ModelStreamEncodeThread>(this, &exportDataBuffer, mModel, selection, encoder);
    encodeThread->start();
    while (!encodeThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    encodeThread.reset();

    exportDataBuffer.close();

    auto mimeData = std::make_unique<QMimeData>();
    mimeData->setData(encoder->remoteClipboardMimeType(), exportData);
    QApplication::clipboard()->setMimeData(mimeData.release(), QClipboard::Clipboard);

    QApplication::restoreOverrideCursor();
}

}

#include "moc_copyascontroller.cpp"
