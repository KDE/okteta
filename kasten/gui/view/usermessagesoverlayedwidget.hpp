/*
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

class UserErrorReport;
class UserNotification;
class UserMessagesOverlayLayout;

class UserMessagesOverlayedWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserMessagesOverlayedWidget(QWidget* parent = nullptr);
    ~UserMessagesOverlayedWidget() override;

public:
    void showErrorReport(Kasten::UserErrorReport* errorReport);
    void showNotification(Kasten::UserNotification* notification);
    // TODO: specify dealing with old main widget, right now relying on being killed
    void setMainWidget(QWidget* widget);

    [[nodiscard]]
    bool isErrorReportShown() const;

Q_SIGNALS:
    void errorReportHidden();

private:
    void createErrorReportWidget();
    void createNotificationWidget();

private:
    UserMessagesOverlayLayout* m_layout;

    KMessageWidget* m_errorReportWidget = nullptr;

    KMessageWidget* m_notificationWidget = nullptr;
    QTimer m_notificationAutoHideTimer;
};

}

#endif
