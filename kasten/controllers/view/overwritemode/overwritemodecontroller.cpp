/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "overwritemodecontroller.hpp"

// lib
#include <kasten/okteta/bytearrayview.hpp>
// KF5
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
