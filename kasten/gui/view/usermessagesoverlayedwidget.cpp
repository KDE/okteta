/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "usermessagesoverlayedwidget.hpp"

// lib
#include "usermessagesoverlaylayout.hpp"
// Kasten Core
#include <Kasten/UserNotification>
// KF
#include <KMessageWidget>

namespace Kasten {

static constexpr int DefaultMessageAutoHideTimeout = 6 * 1000;

UserMessagesOverlayedWidget::UserMessagesOverlayedWidget(QWidget* parent)
    : QWidget(parent)
{
    m_notificationAutoHideTimer.setSingleShot(true);
    m_notificationAutoHideTimer.setInterval(DefaultMessageAutoHideTimeout);

    m_layout = new UserMessagesOverlayLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
}

UserMessagesOverlayedWidget::~UserMessagesOverlayedWidget() = default;

void UserMessagesOverlayedWidget::createNotificationWidget()
{
    m_notificationWidget = new KMessageWidget(this);
    m_notificationWidget->setCloseButtonVisible(false);
    m_notificationWidget->setMessageType(KMessageWidget::Information);
    m_notificationWidget->hide();

    m_layout->setNotificationWidget(m_notificationWidget);

    connect(&m_notificationAutoHideTimer, &QTimer::timeout,
            m_notificationWidget, &KMessageWidget::animatedHide);
}

void UserMessagesOverlayedWidget::setMainWidget(QWidget* widget)
{
    m_layout->addWidget(widget);
    setFocusProxy(widget);

    // ensure z-order of message widgets
    if (m_notificationWidget) {
        m_notificationWidget->raise();
    }
}

void UserMessagesOverlayedWidget::showNotification(UserNotification* notification)
{
    if (! m_notificationWidget) {
        createNotificationWidget();
    }

    m_notificationWidget->setText(notification->text());
    m_notificationWidget->animatedShow();
    m_notificationAutoHideTimer.start();
}

}

#include "moc_usermessagesoverlayedwidget.cpp"
