/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OKTETA_MOUSENAVIGATOR_H
#define OKTETA_MOUSENAVIGATOR_H

// lib
#include "abstractmousecontroller.h"
// Qt
#include <QtCore/QObject>
#include <QtCore/QPoint>

class QMouseEvent;
class QTimer;


namespace Okteta
{

class MouseNavigator : public QObject, public AbstractMouseController
{
  Q_OBJECT

  public:
    MouseNavigator( AbstractByteArrayView* view, AbstractMouseController* parent );

    virtual ~MouseNavigator();

  public: // AbstractMouseController API
    virtual bool handleMousePressEvent( QMouseEvent* mouseEvent );
    virtual bool handleMouseMoveEvent( QMouseEvent* mouseEvent );
    virtual bool handleMouseReleaseEvent( QMouseEvent* mouseEvent );
    virtual bool handleMouseDoubleClickEvent( QMouseEvent* mouseEvent );

  protected:
     /** handles the move of the mouse with pressed buttons */
    void handleMouseMove( const QPoint& point );

  protected Q_SLOTS:
    /** gets called by the scroll timer (for mouse selection) */
    void autoScrollTimerDone();
    /** */
    void startDrag();

  protected:
    /** Timer that triggers ensureCursorVisible function calls */
    QTimer* mScrollTimer;
/*     QTimer *ChangeIntervalTimer, */
    /** Timer to start a drag */
    QTimer* mDragStartTimer;
    /** timer to measure whether the time between a double click and the following counts for a tripleclick */
    QTimer* mTrippleClickTimer;

  protected:
    /** point at which the current double click happended (used by TrippleClick) */
    QPoint mDoubleClickPoint;
    /** line in which the current double click happended (used by TrippleClick) */
    int mDoubleClickLine;
    /** point at which the current dragging started */
    QPoint mDragStartPoint;

  protected: // parameters
    /** flag if the left mouse button is pressed */
    bool mLMBPressed:1;
    /** flag if a double click is happening */
    bool mInLMBDoubleClick:1;
    /** flag if a Drag'n'Drop is happening */
    bool mInDnD:1;
    /** flag if a drag might have started */
    bool mDragStartPossible:1;
};

}

#endif
