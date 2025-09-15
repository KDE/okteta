/*
    SPDX-FileCopyrightText: 2004, 2008, 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractcontroller.hpp"

namespace Okteta {

AbstractController::AbstractController(AbstractController* parent)
    : mParent(parent)
{
}

AbstractController::~AbstractController() = default;

void AbstractController::handleShortcutOverrideEvent(QKeyEvent* keyEvent) const
{
    if (mParent) {
        mParent->handleShortcutOverrideEvent(keyEvent);
    }
}

bool AbstractController::handleKeyPress(QKeyEvent* keyEvent)
{
    return mParent ? mParent->handleKeyPress(keyEvent) : false;
}

}
