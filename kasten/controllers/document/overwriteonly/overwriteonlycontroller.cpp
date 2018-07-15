/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "overwriteonlycontroller.h"

// Okteta core
#include <okteta/abstractbytearraymodel.h>
// KF5
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
