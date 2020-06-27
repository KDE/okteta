/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2004, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractcontroller.hpp"

namespace Okteta {

AbstractController::AbstractController(AbstractController* parent)
    : mParent(parent)
{
}

AbstractController::~AbstractController() = default;

bool AbstractController::handleKeyPress(QKeyEvent* keyEvent)
{
    return mParent ? mParent->handleKeyPress(keyEvent) : false;
}

}
