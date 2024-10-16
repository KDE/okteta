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
    m_messageAutoHideTimer.setSingleShot(true);
    m_messageAutoHideTimer.setInterval(DefaultMessageAutoHideTimeout);

    m_layout = new UserMessagesOverlayLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_messageWidget = new KMessageWidget(this);
    m_messageWidget->setCloseButtonVisible(false);
    m_messageWidget->setMessageType(KMessageWidget::Information);
    m_messageWidget->hide();

    m_layout->setMessageWidget(m_messageWidget);

    connect(&m_messageAutoHideTimer, &QTimer::timeout,
            m_messageWidget, &KMessageWidget::animatedHide);
}

UserMessagesOverlayedWidget::~UserMessagesOverlayedWidget() = default;

void UserMessagesOverlayedWidget::setMainWidget(QWidget* widget)
{
    m_layout->addWidget(widget);
    setFocusProxy(widget);

    // ensure z-order of message widgets
    m_messageWidget->raise();
}

void UserMessagesOverlayedWidget::showNotification(UserNotification* notification)
{
    m_messageWidget->setText(notification->text());
    m_messageWidget->animatedShow();
    m_messageAutoHideTimer.start();
}

}

#include "moc_usermessagesoverlayedwidget.cpp"
