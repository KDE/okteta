/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "clipboardcontroller.hpp"

// Kasten gui
#include <kasten/dataselectable.hpp>
#include <kasten/selecteddatawriteable.hpp>
// Kasten core
#include <kasten/abstractmodel.hpp>
// KF5
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
    } else {
        mMimeDataControl = nullptr;
    }

    const QMimeData* mimeData = QApplication::clipboard()->mimeData(QClipboard::Clipboard);

    const bool hasSelectedData = mSelectionControl ? mSelectionControl->hasSelectedData() : false;
    const bool isWriteable = (mMimeDataControl && !mModel->isReadOnly());
    const bool isPastable = isWriteable && !mimeData->formats().isEmpty() && mMimeDataControl->canReadData(mimeData);

    mCopyAction->setEnabled(hasSelectedData);
    mCutAction->setEnabled(hasSelectedData && isWriteable);
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
    const bool isPastable = isWriteable && !mimeData->formats().isEmpty() && mMimeDataControl->canReadData(mimeData);

    mCutAction->setEnabled(hasSelectedData && isWriteable);
    mPasteAction->setEnabled(isPastable);
}

void ClipboardController::onHasSelectedDataChanged(bool hasSelectedData)
{
    const bool isWriteable = (mMimeDataControl && !mModel->isReadOnly());

    mCopyAction->setEnabled(hasSelectedData);
    mCutAction->setEnabled(hasSelectedData && isWriteable);
}

void ClipboardController::onClipboardDataChanged()
{
    const QMimeData* mimeData = QApplication::clipboard()->mimeData(QClipboard::Clipboard);

    const bool isWriteable = (mMimeDataControl && !mModel->isReadOnly());
    const bool isPastable = isWriteable && !mimeData->formats().isEmpty() && mMimeDataControl->canReadData(mimeData);

    mPasteAction->setEnabled(isPastable);
}

}
