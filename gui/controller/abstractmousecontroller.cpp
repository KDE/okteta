/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmousecontroller.hpp"

namespace Okteta {

AbstractMouseController::AbstractMouseController(AbstractByteArrayView* view, AbstractMouseController* parent)
    : mParent(parent)
    , mView(view)
{
}

AbstractMouseController::~AbstractMouseController() = default;

bool AbstractMouseController::handleMousePressEvent(QMouseEvent* mouseEvent)
{
    return mParent ? mParent->handleMousePressEvent(mouseEvent) : false;
}
bool AbstractMouseController::handleMouseMoveEvent(QMouseEvent* mouseEvent)
{
    return mParent ? mParent->handleMouseMoveEvent(mouseEvent) : false;
}
bool AbstractMouseController::handleMouseReleaseEvent(QMouseEvent* mouseEvent)
{
    return mParent ? mParent->handleMouseReleaseEvent(mouseEvent) : false;
}
bool AbstractMouseController::handleMouseDoubleClickEvent(QMouseEvent* mouseEvent)
{
    return mParent ? mParent->handleMouseDoubleClickEvent(mouseEvent) : false;
}

}
