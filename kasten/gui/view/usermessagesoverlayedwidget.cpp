/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "usermessagesoverlayedwidget.hpp"

// lib
#include "usermessagesoverlaylayout.hpp"
// Kasten Core
#include <Kasten/UserErrorReport>
#include <Kasten/UserNotification>
// KF
#include <KMessageWidget>
// Qt
#include <QIcon>
// Std
#include <chrono>

using namespace std::chrono_literals;

namespace Kasten {

static constexpr auto DefaultMessageAutoHideTimeout = 6s;

UserMessagesOverlayedWidget::UserMessagesOverlayedWidget(QWidget* parent)
    : QWidget(parent)
{
    m_notificationAutoHideTimer.setSingleShot(true);
    m_notificationAutoHideTimer.setInterval(DefaultMessageAutoHideTimeout);

    m_layout = new UserMessagesOverlayLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
}

UserMessagesOverlayedWidget::~UserMessagesOverlayedWidget() = default;

void UserMessagesOverlayedWidget::createErrorReportWidget()
{
    m_errorReportWidget = new KMessageWidget(this);
    m_errorReportWidget->setMessageType(KMessageWidget::Error);
    m_errorReportWidget->setWordWrap(true);
    m_errorReportWidget->setIcon(QIcon::fromTheme(QStringLiteral("dialog-error")));
    m_errorReportWidget->hide();

    m_layout->setErrorReportWidget(m_errorReportWidget);

    connect(m_errorReportWidget, &KMessageWidget::hideAnimationFinished,
            this, &UserMessagesOverlayedWidget::errorReportHidden);
}

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
    if (m_errorReportWidget) {
        m_errorReportWidget->raise();
    }
}

void UserMessagesOverlayedWidget::showErrorReport(Kasten::UserErrorReport* errorReport)
{
    if (! m_errorReportWidget) {
        createErrorReportWidget();
    }

    m_errorReportWidget->setText(errorReport->text());
    m_errorReportWidget->animatedShow();
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

bool UserMessagesOverlayedWidget::isErrorReportShown() const
{
    return m_errorReportWidget ? m_errorReportWidget->isVisible() : false;
}

}

#include "moc_usermessagesoverlayedwidget.cpp"
