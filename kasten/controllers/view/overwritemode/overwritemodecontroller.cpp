/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "overwritemodecontroller.hpp"

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KToggleAction>

namespace Kasten {

OverwriteModeController::OverwriteModeController(KXMLGUIClient* guiClient)
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mSetOverWriteAction = new KToggleAction(i18nc("@option:check set the view into overwrite mode",
                                                  "Overwr&ite Mode"), this);
    mSetOverWriteAction->setWhatsThis(
        i18nc("@info:whatsthis",
              "Choose whether you want the input to be inserted or to overwrite existing data."));
    // TODO: or should we catch the signal from the view (needs to be added)
    actionCollection->setDefaultShortcut(mSetOverWriteAction, QKeySequence(Qt::Key_Insert));
    connect(mSetOverWriteAction, &KToggleAction::triggered, this, &OverwriteModeController::setOverWrite);

    actionCollection->addAction(QStringLiteral("set_overwrite"), mSetOverWriteAction);

    setTargetModel(nullptr);
}

void OverwriteModeController::setTargetModel(AbstractModel* model)
{
    if (mByteArrayView) {
        mByteArrayView->disconnect(mSetOverWriteAction);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    if (mByteArrayView) {
        mSetOverWriteAction->setChecked(mByteArrayView->isOverwriteMode());

        connect(mByteArrayView, &ByteArrayView::overwriteModeChanged,
                mSetOverWriteAction, &KToggleAction::setChecked);
        // TODO: catch if isOverwriteOnly changes
    }

    const bool canInsert = mByteArrayView && !mByteArrayView->isOverwriteOnly();
    mSetOverWriteAction->setEnabled(canInsert);
}

void OverwriteModeController::setOverWrite(bool isOverWrite)
{
    mByteArrayView->setOverwriteMode(isOverWrite);
}

}
