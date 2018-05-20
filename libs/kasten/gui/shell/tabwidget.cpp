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

#include "tabwidget.h"

// lib
#include "tabbar.h"
// Qt
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

namespace Kasten {

TabWidget::TabWidget(QWidget* parent)
    : QTabWidget(parent)
{
    auto* tabBar = new TabBar(this);
    setTabBar(tabBar);
    // slightly fragile as the relative pos of the event is not adapted, but currently not used anyway
    connect(tabBar, &TabBar::testCanDecode,
            this, &TabWidget::testCanDecode);
    connect(tabBar, &TabBar::receivedDropEvent,
            this, &TabWidget::receivedDropEvent);
    connect(tabBar, &TabBar::mouseMiddleClick,
            this, &TabWidget::mouseMiddleClick);

    setTabsClosable(true);
    setDocumentMode(true);
    setAcceptDrops(true);
}

TabWidget::~TabWidget() = default;

void TabWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MidButton) {
        if (count() == 0) {
            emit mouseMiddleClick();
            event->setAccepted(true);
            return;
        }
    }

    QTabWidget::mouseReleaseEvent(event);
}

void TabWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (count() == 0) {
        bool accept = false;
        // The receivers of the testCanDecode() signal has to adjust
        // 'accept' accordingly.
        emit testCanDecode(event, accept);
        event->setAccepted(accept);
        return;
    }

    QTabWidget::dragEnterEvent(event);
}

void TabWidget::dragMoveEvent(QDragMoveEvent* event)
{
    if (count() == 0) {
        bool accept = false;
        // The receivers of the testCanDecode() signal has to adjust
        // 'accept' accordingly.
        emit testCanDecode(event, accept);

        event->setAccepted(accept);
        return;
    }

    QTabWidget::dragMoveEvent(event);
}

void TabWidget::dropEvent(QDropEvent* event)
{
    if (count() == 0) {
        emit receivedDropEvent(event);
        return;
    }

    QTabWidget::dropEvent(event);
}

}
