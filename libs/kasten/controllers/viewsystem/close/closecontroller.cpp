/*
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
    mCloseAction->setEnabled(false);

    guiClient->actionCollection()->addAction(mCloseAction->objectName(), mCloseAction);
}

void CloseController::setTargetModel(AbstractModel* model)
{
    AbstractView* const view = model ? model->findBaseModel<AbstractView*>() : nullptr;
    if (view == mView) {
        return;
    }

    mView = view;

    const bool hasView = (mView != 0);

    mCloseAction->setEnabled(hasView);
}

void CloseController::close()
{
    mViewManager->closeView(mView);
}

}

#include "moc_closecontroller.cpp"
