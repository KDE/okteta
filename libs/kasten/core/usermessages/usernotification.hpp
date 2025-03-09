/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERNOTIFICATION_HPP
#define KASTEN_USERNOTIFICATION_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QtGlobal>
#include <QString>
// Std
#include <memory>

namespace Kasten {

class AbstractModel;

class KASTENCORE_EXPORT UserNotification
{
public:
    explicit UserNotification(AbstractModel* targetModel, const QString& text, const QString& title = QString());
    UserNotification(const UserNotification&) = delete;
    UserNotification(UserNotification&&) = delete;

    ~UserNotification();

public:
    UserNotification& operator=(const UserNotification&) = delete;
    UserNotification& operator=(UserNotification&&) = delete;

public:
    [[nodiscard]]
    AbstractModel* targetModel() const;
    [[nodiscard]]
    QString text() const;
    [[nodiscard]]
    QString title() const;

private:
    const std::unique_ptr<class UserNotificationPrivate> d_ptr;
    Q_DECLARE_PRIVATE(UserNotification)
};

}

#endif
