/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "usermessagesoverlaylayout.hpp"

// Qt
#include <QApplication>
#include <QStyle>
#include <QWidget>

namespace Kasten {

UserMessagesOverlayLayout::UserMessagesOverlayLayout(QWidget* parentWidget)
    : QVBoxLayout(parentWidget)
{
}

UserMessagesOverlayLayout::~UserMessagesOverlayLayout() = default;

void UserMessagesOverlayLayout::setGeometry(const QRect& rect)
{
    QVBoxLayout::setGeometry(rect);

    if (!m_errorReportWidget && !m_notificationWidget) {
        return;
    }

    // using style margins for now, perhaps should use separate metrics?
    QWidget* const widget = parentWidget();
    QStyle* const style = widget ? widget->style() : qApp->style();
    const int leftMargin = style->pixelMetric(QStyle::PM_LayoutLeftMargin, nullptr, widget);
    const int topMargin = style->pixelMetric(QStyle::PM_LayoutTopMargin, nullptr, widget);
    const int rightMargin = style->pixelMetric(QStyle::PM_LayoutRightMargin, nullptr, widget);
    const int bottomMargin = style->pixelMetric(QStyle::PM_LayoutBottomMargin, nullptr, widget);
    const QRect adjustedRect = rect.adjusted(leftMargin, topMargin, -rightMargin, -bottomMargin);

    if (m_errorReportWidget) {
        // TODO: heightForWidth runs 8nto the issue that KMessageBox currently uses setFixedSize
        // once showanimation is done. Which screws things in width->height affecting resizes.
        // Also seen e.g. in KDevelop's main message display.
        const QSize errorReportSize =
            QSize(adjustedRect.width(),
                  m_errorReportWidget->hasHeightForWidth() ?
                      m_errorReportWidget->heightForWidth(adjustedRect.width()) :
                      m_errorReportWidget->sizeHint().height());
        const QRect errorReportRect(adjustedRect.x(),
                                    adjustedRect.y(),
                                    errorReportSize.width(),
                                    errorReportSize.height());
        m_errorReportWidget->setGeometry(errorReportRect);
    }

    if (m_notificationWidget) {
        const QSize notificationSize = m_notificationWidget->sizeHint();
        const QRect notificationRect(adjustedRect.x(),
                                     adjustedRect.height() - notificationSize.height(),
                                     notificationSize.width(),
                                     notificationSize.height());
        m_notificationWidget->setGeometry(notificationRect);
    }
}

void UserMessagesOverlayLayout::setErrorReportWidget(QWidget* errorReportWidget)
{
    m_errorReportWidget = errorReportWidget;
    invalidate();
}

void UserMessagesOverlayLayout::setNotificationWidget(QWidget* notificationWidget)
{
    m_notificationWidget = notificationWidget;
    invalidate();
}

}

#include "moc_usermessagesoverlaylayout.cpp"
