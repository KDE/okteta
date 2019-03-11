/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2008,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include <Kasten/AbstractModel>
// KF
#include <KXMLGUIClient>
#include <KXMLGUIFactory>
#include <KActionCollection>
#include <KLocalizedString>
#include <KSelectAction>
// Qt
#include <QBuffer>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>

Q_DECLARE_METATYPE(Kasten::AbstractModelStreamEncoder*)

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
    connect(mCopyAsSelectAction, QOverload<QAction*>::of(&KSelectAction::triggered),
            this, &CopyAsController::onActionTriggered);

    guiClient->actionCollection()->addAction(QStringLiteral("copy_as"), mCopyAsSelectAction);

    setTargetModel(nullptr);
}

void CopyAsController::setTargetModel(AbstractModel* model)
{
    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = model ? model->findBaseModelWithInterface<If::DataSelectable*>() : nullptr;
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

    const QVector<AbstractModelStreamEncoder*> encoderList =
        mModelCodecManager->encoderList(mModel, selection);
    const bool hasEncoders = (!encoderList.isEmpty());

    if (hasEncoders) {
        for (auto* encoder : encoderList) {
            const QString title = encoder->remoteTypeName();
            auto* action = new QAction(title, mCopyAsSelectAction);

            action->setData(QVariant::fromValue(encoder));
            mCopyAsSelectAction->addAction(action);
        }
    } else {
        QAction* noneAction = new QAction(i18nc("@item There are no encoders.", "Not available."), mCopyAsSelectAction);
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

    AbstractModelStreamEncoderConfigEditor* configEditor =
        mModelCodecViewManager->createConfigEditor(encoder);

    if (configEditor) {
        CopyAsDialog* dialog = new CopyAsDialog(encoder->remoteTypeName(), configEditor);
        dialog->setData(mModel, selection);
        if (dialog->exec() == 0) {
            return;
        }
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    QByteArray exportData;
    QBuffer exportDataBuffer(&exportData);
    exportDataBuffer.open(QIODevice::WriteOnly);

    auto* encodeThread = new ModelStreamEncodeThread(this, &exportDataBuffer, mModel, selection, encoder);
    encodeThread->start();
    while (!encodeThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    delete encodeThread;

    exportDataBuffer.close();

    auto* mimeData = new QMimeData;
    mimeData->setData(encoder->remoteClipboardMimeType(), exportData);
    QApplication::clipboard()->setMimeData(mimeData, QClipboard::Clipboard);

    QApplication::restoreOverrideCursor();
}

}
