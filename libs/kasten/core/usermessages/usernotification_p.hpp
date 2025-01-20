/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERNOTIFICATION_P_HPP
#define KASTEN_USERNOTIFICATION_P_HPP

// lib
#include "usernotification.hpp"

namespace Kasten {

class UserNotificationPrivate
{
public:
    explicit UserNotificationPrivate(AbstractModel* targetModel, const QString& text, const QString& title);
    UserNotificationPrivate(const UserNotificationPrivate&) = delete;

    ~UserNotificationPrivate();

    UserNotificationPrivate& operator=(const UserNotificationPrivate&) = delete;

public:
    [[nodiscard]]
    AbstractModel* targetModel() const;
    [[nodiscard]]
    const QString& text() const;
    [[nodiscard]]
    const QString& title() const;

private:
    AbstractModel* const m_targetModel;
    const QString m_text;
    const QString m_title;
};

inline UserNotificationPrivate::UserNotificationPrivate(AbstractModel* targetModel, const QString& text, const QString& title)
    : m_targetModel(targetModel)
    , m_text(text)
    , m_title(title)
{
}

inline UserNotificationPrivate::~UserNotificationPrivate() = default;

inline AbstractModel* UserNotificationPrivate::targetModel() const { return m_targetModel; }
inline const QString& UserNotificationPrivate::text() const { return m_text; }
inline const QString& UserNotificationPrivate::title() const { return m_title; }

}

#endif
