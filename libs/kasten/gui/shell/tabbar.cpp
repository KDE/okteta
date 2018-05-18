/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2018 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "tabbar.h"

// Qt
#include <QDragEnterEvent>
#include <QDragMoveEvent>


namespace Kasten
{

TabBar::TabBar( QWidget* parent )
  : QTabBar( parent )
{
    setAcceptDrops( true );
}

void TabBar::dragEnterEvent( QDragEnterEvent* event )
{
    // accept the entering in general, independent of being over a tab or not
    // as rejecting here seems not revertable in moveevents
    bool accept = false;
    // The receivers of the testCanDecode() signal has to adjust
    // 'accept' accordingly.
    emit testCanDecode(event, accept);

    event->setAccepted(accept);
}

void TabBar::dragMoveEvent( QDragMoveEvent* event )
{
    bool accept = false;
    if (tabAt(event->pos()) == -1) {
        // The receivers of the testCanDecode() signal has to adjust
        // 'accept' accordingly.
        emit testCanDecode(event, accept);
    }

    event->setAccepted(accept);
}

void TabBar::dropEvent( QDropEvent* event )
{
   if (tabAt(event->pos()) == -1) {
        emit receivedDropEvent(event);
        return;
    }

    QTabBar::dropEvent( event );
}

TabBar::~TabBar()
{
}

}
