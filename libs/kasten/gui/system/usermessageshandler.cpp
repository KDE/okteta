/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "usermessageshandler.hpp"
#include "usermessageshandler_p.hpp"

namespace Kasten {

UserMessagesHandler::UserMessagesHandler(QWidget* widget)
    : d_ptr(std::make_unique<UserMessagesHandlerPrivate>(widget))
{
}

UserMessagesHandler::~UserMessagesHandler() = default;

void UserMessagesHandler::postNotification(std::unique_ptr<UserNotification>&& notification)
{
    Q_D(UserMessagesHandler);

    d->postNotification(std::move(notification));
}

void UserMessagesHandler::setWidget(QWidget* widget)
{
    Q_D(UserMessagesHandler);

    d->setWidget(widget);
}

}
