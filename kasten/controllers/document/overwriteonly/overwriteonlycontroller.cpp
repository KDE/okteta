/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "overwriteonlycontroller.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KToggleAction>

namespace Kasten {

OverwriteOnlyController::OverwriteOnlyController(KXMLGUIClient* guiClient)
{
    mSetOverwriteOnlyAction = new KToggleAction(i18nc("@option:check", "Overwrite only"), this);
    connect(mSetOverwriteOnlyAction, &QAction::triggered, this, &OverwriteOnlyController::setOverwriteOnly);

    guiClient->actionCollection()->addAction(QStringLiteral("isoverwriteonly"), mSetOverwriteOnlyAction);

    setTargetModel(nullptr);
}

void OverwriteOnlyController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
//     if( mByteArrayModel ) mByteArrayModel->disconnect( mSetOverwriteOnlyAction );

    mByteArrayModel = nullptr; // view ? view->baseModel() : nullptr;

    if (mByteArrayModel) {
//         mSetOverwriteOnlyAction->setChecked( mByteArrayModel->isOverwriteOnly() );

//         connect( mByteArrayModel, SIGNAL(overwriteOnlyChanged(bool)),
//                  mSetOverwriteOnlyAction, SLOT(setChecked(bool)) );
//         connect( mByteArrayModel, SIGNAL(modifiableChanged(bool)),
//                  mSetOverwriteOnlyAction, SLOT(setEnabled(bool)) );
    }

    mSetOverwriteOnlyAction->setEnabled(false);  // mByteArrayModel ? mByteArrayModel->isModifiable() : false );
}

void OverwriteOnlyController::setOverwriteOnly(bool isOverwriteOnly)
{
    Q_UNUSED(isOverwriteOnly)
//     mByteArrayModel->setOverwriteOnly( isOverwriteOnly );
}

}
