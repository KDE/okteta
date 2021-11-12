/*
    This file is part of the Kasten Framework, made within the KDE community.

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

    // catch non-LMB double clicks
    installEventFilter(this);

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
                emit tabCloseRequested(tabIndex);
            } else {
                emit mouseMiddleClick();
            }
            event->setAccepted(true);
            return;
        }
    }

    QTabBar::mouseReleaseEvent(event);
}

void TabBar::dragEnterEvent(QDragEnterEvent* event)
{
    // accept the entering in general, independent of being over a tab or not
    // as rejecting here seems not revertable in moveevents
    bool accept = false;
    // The receivers of the testCanDecode() signal has to adjust
    // 'accept' accordingly.
    emit testCanDecode(event, accept);

    event->setAccepted(accept);
}

void TabBar::dragMoveEvent(QDragMoveEvent* event)
{
    bool accept = false;
    if (tabAt(event->pos()) == -1) {
        // The receivers of the testCanDecode() signal has to adjust
        // 'accept' accordingly.
        emit testCanDecode(event, accept);
    }

    event->setAccepted(accept);
}

void TabBar::dropEvent(QDropEvent* event)
{
    if (tabAt(event->pos()) == -1) {
        emit receivedDropEvent(event);
        return;
    }

    QTabBar::dropEvent(event);
}


bool TabBar::eventFilter(QObject* object, QEvent* event)
{
    if (object == this) {
        // TODO Qt6: Move to mouseDoubleClickEvent when fixme in qttabbar.cpp is resolved
        // see "fixme Qt 6: move to mouseDoubleClickEvent(), here for BC reasons." in qtabbar.cpp
        if (event->type() == QEvent::MouseButtonDblClick) {
            // block tabBarDoubleClicked signals with RMB, see https://bugs.kde.org/show_bug.cgi?id=356016
            auto* mouseEvent = static_cast<const QMouseEvent*>(event);
            if (mouseEvent->button() != Qt::LeftButton) {
                return true;
            }
        }
    }
    return QObject::eventFilter(object, event);
}

void TabBar::onTabBarDoubleClicked(int index)
{
    if (index == -1) {
        emit emptySpaceMouseDoubleClicked();
    }
}

}
