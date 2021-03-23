/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "touchonlytapandholdgesture.hpp"

// Qt
#include <QTapAndHoldGesture>

namespace Okteta {

void TouchOnlyTapAndHoldGesture::beginHoldTimeer()
{
    if (m_timerId != 0) {
        killTimer(m_timerId);
    }
    m_timerId = startTimer(QTapAndHoldGesture::timeout());
}

void TouchOnlyTapAndHoldGesture::cancelHoldTimer()
{
    if (m_timerId != 0) {
        killTimer(m_timerId);
    }
    m_timerId = 0;
}

void TouchOnlyTapAndHoldGesture::reset()
{
    m_position = QPointF();
    cancelHoldTimer();
}

}
