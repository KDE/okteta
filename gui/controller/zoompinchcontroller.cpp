/*
    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "zoompinchcontroller.hpp"

// lib
#include <abstractbytearrayview_p.hpp>
// Qt
#include <QPinchGesture>

namespace Okteta {

ZoomPinchController::ZoomPinchController(AbstractByteArrayViewPrivate* view)
    : mView(view)
{
}

bool ZoomPinchController::handlePinchGesture(QPinchGesture* pinchGesture)
{
    switch (pinchGesture->state()) {
        case Qt::GestureStarted :
            m_originalZoomScale = mView->zoomScale();
            break;
        case Qt::GestureCanceled :
            mView->setZoomScale(m_originalZoomScale);
            break;
        case Qt::GestureUpdated:
        case Qt::GestureFinished:
            if (pinchGesture->changeFlags() & QPinchGesture::ScaleFactorChanged) {
                mView->setZoomScale(pinchGesture->totalScaleFactor() * m_originalZoomScale);
            }
            break;
        default:
            break;
    }

    return true;
}

}
