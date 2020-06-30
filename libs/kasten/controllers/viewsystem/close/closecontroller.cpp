/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "closecontroller.hpp"

// Kasten gui
#include <Kasten/ViewManager>
// KF
#include <KActionCollection>
#include <KStandardAction>
#include <KXMLGUIClient>
// Qt
#include <QAction>

namespace Kasten {

CloseController::CloseController(ViewManager* viewManager, KXMLGUIClient* guiClient)
    : mViewManager(viewManager)
{
    mCloseAction = KStandardAction::close(this, &CloseController::close, this);

    guiClient->actionCollection()->addAction(mCloseAction->objectName(), mCloseAction);

    setTargetModel(0);
}

void CloseController::setTargetModel(AbstractModel* model)
{
    mView = model ? model->findBaseModel<AbstractView*>() : 0;

    const bool hasView = (mView != 0);

    mCloseAction->setEnabled(hasView);
}

void CloseController::close()
{
    mViewManager->closeView(mView);
}

}
