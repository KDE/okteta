/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "usermessageshandler_p.hpp"

// Kasten Core
#include <Kasten/AbstractModel>
#include <Kasten/UserNotification>
#include <Kasten/UserNotificationsInlineable>
// KF
#include <KMessageBox>

namespace Kasten {

void UserMessagesHandlerPrivate::postNotification(std::unique_ptr<UserNotification>&& notification)
{
    AbstractModel* targetModel = notification->targetModel();
    AbstractModel* model = targetModel ? targetModel->findBaseModelWithInterface<If::UserNotificationsInlineable*>() : nullptr;
    If::UserNotificationsInlineable* userNotificationsInlineable = model ? qobject_cast<If::UserNotificationsInlineable*>(model) : nullptr;

    // TODO: is non-view model, search a visible view (with most priority)
    if (userNotificationsInlineable) {
        userNotificationsInlineable->showNotification(notification.get());
    } else {
        KMessageBox::information(m_widget,
                                notification->text(),
                                notification->title());
    }
}

void UserMessagesHandlerPrivate::setWidget(QWidget* widget)
{
    m_widget = widget;
}

}
