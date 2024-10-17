/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERMESSAGESOVERLAYEDWIDGET_HPP
#define KASTEN_USERMESSAGESOVERLAYEDWIDGET_HPP

// Qt
#include <QTimer>
#include <QWidget>

class KMessageWidget;

namespace Kasten {

class UserNotification;
class UserMessagesOverlayLayout;

class UserMessagesOverlayedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserMessagesOverlayedWidget(QWidget* parent = nullptr);
    ~UserMessagesOverlayedWidget() override;

public:
    void showNotification(Kasten::UserNotification* notification);
    // TODO: specify dealing with old main widget, right now relying on being killed
    void setMainWidget(QWidget* widget);

private:
    void createNotificationWidget();

private:
    UserMessagesOverlayLayout* m_layout;

    KMessageWidget* m_notificationWidget = nullptr;
    QTimer m_notificationAutoHideTimer;
};

}

#endif
