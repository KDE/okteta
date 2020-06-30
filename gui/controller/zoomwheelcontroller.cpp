/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "zoomwheelcontroller.hpp"

// lib
#include <abstractbytearrayview.hpp>
// Qt
#include <QWheelEvent>

namespace Okteta {

ZoomWheelController::ZoomWheelController(AbstractByteArrayView* view, AbstractWheelController* parent)
    : AbstractWheelController(parent)
    , mView(view)
{
}

ZoomWheelController::~ZoomWheelController() = default;

bool ZoomWheelController::handleWheelEvent(QWheelEvent* wheelEvent)
{
    bool eventUsed = false;

    if (wheelEvent->modifiers() & Qt::CTRL) {
        const int delta = wheelEvent->angleDelta().y();
        if (delta > 0) {
            mView->zoomIn();
        } else if (delta < 0) {
            mView->zoomOut();
        }

        eventUsed = true;
    }

    return eventUsed ? true : AbstractWheelController::handleWheelEvent(wheelEvent);
}

}
