/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "usermessageshandler.hpp"
#include "usermessageshandler_p.hpp"

// Std
#include <utility>

namespace Kasten {

UserMessagesHandler::UserMessagesHandler(QWidget* widget)
    : d_ptr(std::make_unique<UserMessagesHandlerPrivate>(widget))
{
}

UserMessagesHandler::~UserMessagesHandler() = default;

void UserMessagesHandler::postErrorReport(std::unique_ptr<UserErrorReport>&& errorReport)
{
    Q_D(UserMessagesHandler);

    d->postErrorReport(std::move(errorReport));
}

void UserMessagesHandler::postNotification(std::unique_ptr<UserNotification>&& notification)
{
    Q_D(UserMessagesHandler);

    d->postNotification(std::move(notification));
}

QString UserMessagesHandler::executeQuery(std::unique_ptr<UserQuery>&& userQuery)
{
    Q_D(UserMessagesHandler);

    return d->executeQuery(std::move(userQuery));
}

void UserMessagesHandler::setWidget(QWidget* widget)
{
    Q_D(UserMessagesHandler);

    d->setWidget(widget);
}

}
