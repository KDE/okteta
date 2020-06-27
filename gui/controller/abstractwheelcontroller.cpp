/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractwheelcontroller.hpp"

namespace Okteta {

AbstractWheelController::AbstractWheelController(AbstractWheelController* parent)
    : mParent(parent)
{
}

AbstractWheelController::~AbstractWheelController() = default;

bool AbstractWheelController::handleWheelEvent(QWheelEvent* wheelEvent)
{
    return mParent ? mParent->handleWheelEvent(wheelEvent) : false;
}

}
