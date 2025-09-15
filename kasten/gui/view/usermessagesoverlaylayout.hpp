/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERMESSAGESOVERLAYLAYOUT_HPP
#define KASTEN_USERMESSAGESOVERLAYLAYOUT_HPP

// Qt
#include <QVBoxLayout>

class QWidget;

namespace Kasten {

class UserMessagesOverlayLayout : public QVBoxLayout
{
    Q_OBJECT

public:
    explicit UserMessagesOverlayLayout(QWidget* parent = nullptr);
    ~UserMessagesOverlayLayout() override;

public: // QLayout API
    void setGeometry(const QRect& rect) override;

public:
    void setErrorReportWidget(QWidget* errorReportWidget);
    void setNotificationWidget(QWidget* notificationWidget);

private:
    QWidget* m_errorReportWidget = nullptr;
    QWidget* m_notificationWidget = nullptr;
};

}

#endif
