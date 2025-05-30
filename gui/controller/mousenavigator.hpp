/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_MOUSENAVIGATOR_HPP
#define OKTETA_MOUSENAVIGATOR_HPP

// lib
#include "abstractmousecontroller.hpp"
// Qt
#include <QObject>
#include <QPoint>
#include <QTimer>


namespace Okteta {

class MouseNavigator : public QObject
                     , public AbstractMouseController
{
    Q_OBJECT

public:
    MouseNavigator(AbstractByteArrayViewPrivate* view, AbstractMouseController* parent);

    ~MouseNavigator() override;

public: // AbstractMouseController API
    [[nodiscard]]
    bool handleMousePressEvent(QMouseEvent* mouseEvent) override;
    [[nodiscard]]
    bool handleMouseMoveEvent(QMouseEvent* mouseEvent) override;
    [[nodiscard]]
    bool handleMouseReleaseEvent(QMouseEvent* mouseEvent) override;
    [[nodiscard]]
    bool handleMouseDoubleClickEvent(QMouseEvent* mouseEvent) override;

private:
    /** handles the move of the mouse with pressed buttons */
    void handleMouseMove(QPoint point);

private Q_SLOTS:
    /** gets called by the scroll timer (for mouse selection) */
    void autoScrollTimerDone();
    /** */
    void startDrag();

private:
    /** Timer that triggers ensureCursorVisible function calls */
    QTimer mScrollTimer;
/*     QTimer *ChangeIntervalTimer, */
    /** Timer to start a drag */
    QTimer mDragStartTimer;
    /** timer to measure whether the time between a double click and the following counts for a tripleclick */
    QTimer mTrippleClickTimer;

private:
    /** point at which the current double click happended (used by TrippleClick) */
    QPoint mDoubleClickPoint;
    /** line in which the current double click happended (used by TrippleClick) */
    int mDoubleClickLine;
    /** point at which the current dragging started */
    QPoint mDragStartPoint;

private: // parameters
    /** flag if the left mouse button is pressed */
    bool mLMBPressed : 1;
    /** flag if a double click is happening */
    bool mInLMBDoubleClick : 1;
    /** flag if a drag might have started */
    bool mDragStartPossible : 1;
};

}

#endif
