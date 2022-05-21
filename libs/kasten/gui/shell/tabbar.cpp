/*
    SPDX-FileCopyrightText: 2018 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "tabbar.hpp"

// Qt
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>

namespace Kasten {

TabBar::TabBar(QWidget* parent)
    : QTabBar(parent)
{
    setAcceptDrops(true);
    setChangeCurrentOnDrag(true);

    connect(this, &QTabBar::tabBarDoubleClicked,
            this, &TabBar::onTabBarDoubleClicked);
}

TabBar::~TabBar() = default;

void TabBar::mouseReleaseEvent(QMouseEvent* event)
{
    if (tabsClosable()) {
        if (event->button() == Qt::MiddleButton) {
            const int tabIndex = tabAt(event->pos());
            if (tabIndex != -1) {
                Q_EMIT tabCloseRequested(tabIndex);
            } else {
                Q_EMIT mouseMiddleClick();
            }
            event->setAccepted(true);
            return;
        }
    }

    QTabBar::mouseReleaseEvent(event);
}

void TabBar::mouseDoubleClickEvent(QMouseEvent* event)
{
   // block tabBarDoubleClicked signals with RMB, see https://bugs.kde.org/show_bug.cgi?id=356016
    if (event->button() != Qt::LeftButton) {
        mousePressEvent(event);
        return;
    }

    QTabBar::mouseDoubleClickEvent(event);
}

void TabBar::dragEnterEvent(QDragEnterEvent* event)
{
    // accept the entering in general, independent of being over a tab or not
    // as rejecting here seems not revertable in moveevents
    bool accept = false;
    // The receivers of the testCanDecode() signal has to adjust
    // 'accept' accordingly.
    Q_EMIT testCanDecode(event, accept);

    if (accept) {
        event->setAccepted(true);
        return;
    }

    QTabBar::dragEnterEvent(event);
}

void TabBar::dragMoveEvent(QDragMoveEvent* event)
{
    if (tabAt(event->position().toPoint()) == -1) {
        bool accept = false;
        // The receivers of the testCanDecode() signal has to adjust
        // 'accept' accordingly.
        Q_EMIT testCanDecode(event, accept);

        if (accept) {
            event->setAccepted(true);
            return;
        }
    }

    QTabBar::dragMoveEvent(event);
}

void TabBar::dropEvent(QDropEvent* event)
{
    if (tabAt(event->position().toPoint()) == -1) {
        Q_EMIT receivedDropEvent(event);
        return;
    }

    QTabBar::dropEvent(event);
}

void TabBar::onTabBarDoubleClicked(int index)
{
    if (index == -1) {
        Q_EMIT emptySpaceMouseDoubleClicked();
    }
}

}

#include "moc_tabbar.cpp"
