/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "selectcontroller.hpp"

// Kasten gui
#include <Kasten/DataSelectable>
#include <Kasten/AbstractModel>
// KF
#include <KXMLGUIClient>
#include <KActionCollection>
#include <KStandardAction>
// Qt
#include <QAction>

namespace Kasten {

SelectController::SelectController(KXMLGUIClient* guiClient)
{
    mSelectAllAction = KStandardAction::selectAll(this, &SelectController::selectAll, this);
    mSelectAllAction->setEnabled(false);
    mDeselectAction =  KStandardAction::deselect( this, &SelectController::unselect,  this);
    mDeselectAction->setEnabled(false);

    KActionCollection* actionCollection = guiClient->actionCollection();
    actionCollection->addAction(mSelectAllAction->objectName(), mSelectAllAction);
    actionCollection->addAction(mDeselectAction->objectName(), mDeselectAction);
}

void SelectController::setTargetModel(AbstractModel* model)
{
    AbstractModel* const dataSelectableModel = model ? model->findBaseModelWithInterface<If::DataSelectable*>() : nullptr;
    if (dataSelectableModel == mModel) {
        return;
    }

    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = dataSelectableModel;
    mSelectControl = mModel ? qobject_cast<If::DataSelectable*>(mModel) : nullptr;

    const bool hasSelectionControl = (mSelectControl != nullptr);
    if (hasSelectionControl) {
        connect(mModel, SIGNAL(hasSelectedDataChanged(bool)), SLOT(onHasSelectedDataChanged(bool)));
    }

    mSelectAllAction->setEnabled(hasSelectionControl);
    mDeselectAction->setEnabled(hasSelectionControl ? mSelectControl->hasSelectedData() : false);
}

void SelectController::onHasSelectedDataChanged(bool hasSelectedData)
{
    mDeselectAction->setEnabled(hasSelectedData);
}

void SelectController::selectAll()
{
    mSelectControl->selectAllData(true);
}

void SelectController::unselect()
{
    mSelectControl->selectAllData(false);
}

}

#include "moc_selectcontroller.cpp"
