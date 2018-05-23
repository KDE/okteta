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

#include "viewmodecontroller.h"

// lib
#include <kasten/okteta/bytearrayview.h>
// KF5
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KSelectAction>

namespace Kasten {

ViewModeController::ViewModeController(KXMLGUIClient* guiClient)
{
    // view style
    mViewModeAction = new KSelectAction(i18nc("@title:menu", "&View Mode"), this);
    mViewModeAction->setItems(QStringList {
        i18nc("@item:inmenu", "&Columns"),
        i18nc("@item:inmenu", "&Rows"),
    });
    connect(mViewModeAction, QOverload<int>::of(&KSelectAction::triggered),
            this, &ViewModeController::setViewMode);

    guiClient->actionCollection()->addAction(QStringLiteral("viewmode"), mViewModeAction);

    setTargetModel(nullptr);
}

void ViewModeController::setTargetModel(AbstractModel* model)
{
//     if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    const bool hasView = (mByteArrayView != nullptr);
    if (hasView) {
        mViewModeAction->setCurrentItem((int)mByteArrayView->viewModus());
    }
    mViewModeAction->setEnabled(hasView);
}

void ViewModeController::setViewMode(int viewMode)
{
    mByteArrayView->setViewModus(viewMode);
}

}
