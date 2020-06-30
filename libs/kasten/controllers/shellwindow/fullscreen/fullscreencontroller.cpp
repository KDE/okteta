/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "fullscreencontroller.hpp"

// KF
#include <KToggleFullScreenAction>
#include <KXmlGuiWindow>
#include <KActionCollection>
#include <KStandardAction>

namespace Kasten {

FullScreenController::FullScreenController(KXmlGuiWindow* window)
    : mMainWindow(window)
{
    auto* fullScreenAction = KStandardAction::fullScreen(this, &FullScreenController::switchFullScreen,
                                                         window, this);

    mMainWindow->actionCollection()->addAction(fullScreenAction->objectName(), fullScreenAction);
}

void FullScreenController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

void FullScreenController::switchFullScreen(bool toFullScreen)
{
    KToggleFullScreenAction::setFullScreen(mMainWindow, toFullScreen);
}

}
