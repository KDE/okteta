/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "clipboardcontroller.hpp"

// Kasten gui
#include <Kasten/DataSelectable>
#include <Kasten/SelectedDataWriteable>
// Kasten core
#include <Kasten/AbstractModel>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
// Qt
#include <QClipboard>
#include <QApplication>
#include <QMimeData>
#include <QAction>

namespace Kasten {

ClipboardController::ClipboardController(KXMLGUIClient* guiClient)
{
    mCutAction =   KStandardAction::cut(  this, &ClipboardController::cut,   this);
    mCopyAction =  KStandardAction::copy( this, &ClipboardController::copy,  this);
    mPasteAction = KStandardAction::paste(this, &ClipboardController::paste, this);

    guiClient->actionCollection()->addActions({
        mCutAction,
        mCopyAction,
        mPasteAction,
    });

    connect(QApplication::clipboard(), &QClipboard::dataChanged,
            this, &ClipboardController::onClipboardDataChanged);

    setTargetModel(nullptr);
}

void ClipboardController::setTargetModel(AbstractModel* model)
{
    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = model ? model->findBaseModelWithInterface<If::DataSelectable*>() : nullptr;
    mSelectionControl = mModel ? qobject_cast<If::DataSelectable*>(mModel) : nullptr;

    if (mSelectionControl) {
        connect(mModel, SIGNAL(hasSelectedDataChanged(bool)), SLOT(onHasSelectedDataChanged(bool)));

        mMimeDataControl = qobject_cast<If::SelectedDataWriteable*>(mModel);
        if (mMimeDataControl) {
            connect(mModel, &AbstractModel::readOnlyChanged,
                    this, &ClipboardController::onReadOnlyChanged);
        }

        mCutControl = nullptr;
        const auto children = mModel->findChildren<QObject*>(QString(), Qt::FindDirectChildrenOnly);
        for (auto* child : children) {
            mCutControl = qobject_cast<If::SelectedDataCutable*>(child);
            if (mCutControl) {
                connect(child, SIGNAL(canCutSelectedDataChanged(bool)),
                        this, SLOT(onCanCutSelectedDataChanged(bool)));
                break;
            }
        }
    } else {
        mMimeDataControl = nullptr;
        mCutControl = nullptr;
    }

    const QMimeData* mimeData = QApplication::clipboard()->mimeData(QClipboard::Clipboard);

    const bool hasSelectedData = mSelectionControl ? mSelectionControl->hasSelectedData() : false;
    const bool isWriteable = (mMimeDataControl && !mModel->isReadOnly());
    const bool isCutable = (hasSelectedData && isWriteable && (!mCutControl || mCutControl->canCutSelectedData()));
    const bool isPastable = isWriteable && !mimeData->formats().isEmpty() && mMimeDataControl->canReadData(mimeData);

    mCopyAction->setEnabled(hasSelectedData);
    mCutAction->setEnabled(isCutable);
    mPasteAction->setEnabled(isPastable);
}

void ClipboardController::cut()
{
    QMimeData* data = mMimeDataControl->cutSelectedData();
    if (!data) {
        return;
    }

    QApplication::clipboard()->setMimeData(data, QClipboard::Clipboard);
}

void ClipboardController::copy()
{
    QMimeData* data = mSelectionControl->copySelectedData();
    if (!data) {
        return;
    }

    QApplication::clipboard()->setMimeData(data, QClipboard::Clipboard);
}

void ClipboardController::paste()
{
    const QMimeData* data = QApplication::clipboard()->mimeData(QClipboard::Clipboard);

    mMimeDataControl->insertData(data);
}

void ClipboardController::onReadOnlyChanged(bool isReadOnly)
{
    const QMimeData* mimeData = QApplication::clipboard()->mimeData(QClipboard::Clipboard);

    const bool hasSelectedData = mSelectionControl ? mSelectionControl->hasSelectedData() : false;
    const bool isWriteable = !isReadOnly;
    // backward compatibility: assume true by default
    const bool canCutSelectedData = mCutControl ? mCutControl->canCutSelectedData() : true;
    const bool isCutable = (hasSelectedData && isWriteable && canCutSelectedData);
    const bool isPastable = isWriteable && !mimeData->formats().isEmpty() && mMimeDataControl->canReadData(mimeData);

    mCutAction->setEnabled(isCutable);
    mPasteAction->setEnabled(isPastable);
}

void ClipboardController::onHasSelectedDataChanged(bool hasSelectedData)
{
    const bool isWriteable = (mMimeDataControl && !mModel->isReadOnly());
    // backward compatibility: assume true by default
    const bool canCutSelectedData = mCutControl ? mCutControl->canCutSelectedData() : true;
    const bool isCutable = (hasSelectedData && isWriteable && canCutSelectedData);

    mCopyAction->setEnabled(hasSelectedData);
    mCutAction->setEnabled(isCutable);
}

void ClipboardController::onCanCutSelectedDataChanged(bool canCutSelectedData)
{
    const bool hasSelectedData = mSelectionControl ? mSelectionControl->hasSelectedData() : false;
    const bool isWriteable = (mMimeDataControl && !mModel->isReadOnly());
    const bool isCutable = (hasSelectedData && isWriteable && canCutSelectedData);

    mCutAction->setEnabled(isCutable);
}

void ClipboardController::onClipboardDataChanged()
{
    const QMimeData* mimeData = QApplication::clipboard()->mimeData(QClipboard::Clipboard);

    const bool isWriteable = (mMimeDataControl && !mModel->isReadOnly());
    const bool isPastable = isWriteable && !mimeData->formats().isEmpty() && mMimeDataControl->canReadData(mimeData);

    mPasteAction->setEnabled(isPastable);
}

}
