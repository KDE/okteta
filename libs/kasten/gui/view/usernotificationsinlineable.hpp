/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_USERNOTIFICATIONSINLINEABLE_HPP
#define KASTEN_IF_USERNOTIFICATIONSINLINEABLE_HPP

// Qt
#include <QtPlugin>
// Std
#include <memory>

namespace Kasten {

class UserNotification;

namespace If {

class UserNotificationsInlineable
{
public:
    virtual ~UserNotificationsInlineable();

public: // actions
    virtual void showNotification(UserNotification* notification) = 0;
};

inline UserNotificationsInlineable::~UserNotificationsInlineable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::UserNotificationsInlineable, "org.kde.kasten.if.usernotificationsinlineable/1.0")

#endif
