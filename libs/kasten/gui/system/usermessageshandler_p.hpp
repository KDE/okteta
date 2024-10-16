/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VUSERMESSAGESHANDLER_P_HPP
#define KASTEN_VUSERMESSAGESHANDLER_P_HPP

// lib
#include "usermessageshandler.hpp"

namespace Kasten {

class UserMessagesHandlerPrivate
{
public:
    explicit UserMessagesHandlerPrivate(QWidget* widget);
    UserMessagesHandlerPrivate(const UserMessagesHandlerPrivate&) = delete;

    ~UserMessagesHandlerPrivate();

    UserMessagesHandlerPrivate& operator=(const UserMessagesHandlerPrivate&) = delete;

public:
    void postNotification(std::unique_ptr<UserNotification>&& notification);

public:
    void setWidget(QWidget* widget);

private:
    QWidget* m_widget;
};

inline UserMessagesHandlerPrivate::UserMessagesHandlerPrivate(QWidget* widget)
    : m_widget(widget)
{
}

inline UserMessagesHandlerPrivate::~UserMessagesHandlerPrivate() = default;

}

#endif
