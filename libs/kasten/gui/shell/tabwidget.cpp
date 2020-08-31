/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2018 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "tabwidget.hpp"

// lib
#include "tabbar.hpp"
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
    if (event->button() == Qt::MiddleButton) {
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
