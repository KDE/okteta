/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "zoompinchcontroller.hpp"

// lib
#include <abstractbytearrayview.hpp>
// Qt
#include <QPinchGesture>

namespace Okteta {

ZoomPinchController::ZoomPinchController(AbstractByteArrayView* view)
    : mView(view)
{
}

bool ZoomPinchController::handlePinchGesture(QPinchGesture* pinchGesture)
{
    switch (pinchGesture->state()) {
        case Qt::GestureStarted :
            mOriginalZoomLevel = mView->zoomLevel();
            break;
        case Qt::GestureCanceled :
            mView->setZoomLevel(mOriginalZoomLevel);
            break;
        case Qt::GestureUpdated:
        case Qt::GestureFinished:
            if (pinchGesture->changeFlags() & QPinchGesture::ScaleFactorChanged) {
                mView->setZoomLevel(pinchGesture->totalScaleFactor() * mOriginalZoomLevel);
            }
            break;
        default:
            break;
    }

    return true;
}

}
