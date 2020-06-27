/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "quitcontroller.hpp"

// KF
#include <KXmlGuiWindow>
#include <KActionCollection>
#include <KStandardAction>

namespace Kasten {

QuitController::QuitController(KXmlGuiWindow* window)
    : mMainWindow(window)
{
    auto* quitAction = KStandardAction::quit(this, &QuitController::quit, this);

    mMainWindow->actionCollection()->addAction(quitAction->objectName(), quitAction);
}

void QuitController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

void QuitController::quit()
{
    mMainWindow->close();
//   Program->quit(); // TODO: think about a base program
}

}
