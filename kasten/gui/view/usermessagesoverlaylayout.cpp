/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

    if (m_notificationWidget) {
        // using style margins for now, perhaps should use separate matrics?
        QWidget* const widget = parentWidget();
        QStyle* const style = widget ? widget->style() : qApp->style();
        const int leftMargin = style->pixelMetric(QStyle::PM_LayoutLeftMargin, nullptr, widget);
        const int topMargin = style->pixelMetric(QStyle::PM_LayoutTopMargin, nullptr, widget);
        const int rightMargin = style->pixelMetric(QStyle::PM_LayoutRightMargin, nullptr, widget);
        const int bottomMargin = style->pixelMetric(QStyle::PM_LayoutBottomMargin, nullptr, widget);
        const QRect adjustedRect = rect.adjusted(leftMargin, topMargin, -rightMargin, -bottomMargin);
        const QSize notificationSize = m_notificationWidget->sizeHint();
        const QRect notificationRect(adjustedRect.x(),
                                     adjustedRect.height() - notificationSize.height(),
                                     notificationSize.width(),
                                     notificationSize.height());
        m_notificationWidget->setGeometry(notificationRect);
    }
}

void UserMessagesOverlayLayout::setNotificationWidget(QWidget* notificationWidget)
{
    m_notificationWidget = notificationWidget;
    invalidate();
}

}
