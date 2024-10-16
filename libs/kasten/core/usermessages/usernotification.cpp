/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "usernotification.hpp"
#include "usernotification_p.hpp"

namespace Kasten {

UserNotification::UserNotification(AbstractModel* targetModel, const QString& text, const QString& title)
    : d_ptr(std::make_unique<UserNotificationPrivate>(targetModel, text, title))
{
}

UserNotification::~UserNotification() = default;

AbstractModel* UserNotification::targetModel() const
{
    Q_D(const UserNotification);

    return d->targetModel();
}

QString UserNotification::text() const
{
    Q_D(const UserNotification);

    return d->text();
}

QString UserNotification::title() const
{
    Q_D(const UserNotification);

    return d->title();
}

}
