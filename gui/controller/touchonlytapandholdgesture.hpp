/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_TOUCHONLYTAPANDHOLDGESTURE_HPP
#define OKTETA_TOUCHONLYTAPANDHOLDGESTURE_HPP

// Qt
#include <QGesture>

namespace Okteta {

class TouchOnlyTapAndHoldGesture : public QGesture
{
    Q_OBJECT
    /// in widget coords
    Q_PROPERTY(QPointF position READ position WRITE setPosition)

public:
    TouchOnlyTapAndHoldGesture() = default;

public:
    QPointF position() const { return m_position; }
    void setPosition(const QPointF &pos) { m_position = pos; }
    void beginHoldTimeer();
    void cancelHoldTimer();
    bool isHoldTimerActive() const { return (m_timerId != 0); }
    void reset();

private:
    int m_timerId = 0;
    QPointF m_position;
};

}

#endif
