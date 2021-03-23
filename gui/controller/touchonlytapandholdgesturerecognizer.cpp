/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "touchonlytapandholdgesturerecognizer.hpp"

// lib
#include "touchonlytapandholdgesture.hpp"
// Qt
#include <QWidget>
#include <QTouchEvent>

namespace Okteta {

constexpr int tapRadius = 40; // value used by Qt's tap gestures

TouchOnlyTapAndHoldGestureRecognizer::TouchOnlyTapAndHoldGestureRecognizer() = default;

QGesture *TouchOnlyTapAndHoldGestureRecognizer::create(QObject* target)
{
    Q_UNUSED(target);

    return new TouchOnlyTapAndHoldGesture;
}

QGestureRecognizer::Result TouchOnlyTapAndHoldGestureRecognizer::recognize(QGesture* state, QObject* object,
                                                                           QEvent* event)
{
    auto* tapAndHoldGesture = static_cast<TouchOnlyTapAndHoldGesture *>(state);

    if ((object == state) && (event->type() == QEvent::Timer)) {
        tapAndHoldGesture->cancelHoldTimer();
        return QGestureRecognizer::FinishGesture | QGestureRecognizer::ConsumeEventHint;
    }

    switch (event->type()) {
    case QEvent::TouchBegin: {
        auto* touchEvent = static_cast<const QTouchEvent*>(event);
        const QTouchEvent::TouchPoint touchPoint = touchEvent->touchPoints().at(0);
        const QPointF screenPosition = touchPoint.screenPos();
        const QPointF widgetPosition = touchPoint.pos();
        tapAndHoldGesture->setHotSpot(screenPosition);
        tapAndHoldGesture->setPosition(widgetPosition);
        tapAndHoldGesture->beginHoldTimeer();
        return QGestureRecognizer::MayBeGesture;
    }
    case QEvent::TouchUpdate: {
        if (tapAndHoldGesture->isHoldTimerActive()) {
            auto* touchEvent = static_cast<const QTouchEvent*>(event);
            if (touchEvent->touchPoints().size() == 1) {
                const QTouchEvent::TouchPoint touchPoint = touchEvent->touchPoints().at(0);
                const QPoint delta = touchPoint.pos().toPoint() - touchPoint.startPos().toPoint();
                if (delta.manhattanLength() <= tapRadius) {
                    return QGestureRecognizer::MayBeGesture;
                }
            }
        }
        return QGestureRecognizer::CancelGesture;
    }
    case QEvent::TouchEnd:
        return QGestureRecognizer::CancelGesture;
    default:
        return QGestureRecognizer::Ignore;
    }
}

void TouchOnlyTapAndHoldGestureRecognizer::reset(QGesture* state)
{
    auto* tapAndHoldGesture = static_cast<TouchOnlyTapAndHoldGesture*>(state);
    tapAndHoldGesture->reset();

    QGestureRecognizer::reset(state);
}

}
