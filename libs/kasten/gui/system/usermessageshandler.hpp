/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERMESSAGESHANDLER_HPP
#define KASTEN_USERMESSAGESHANDLER_HPP

// lib
#include "kastengui_export.hpp"
// Kasten core
#include <Kasten/AbstractUserMessagesHandler>
// Qt
#include <QtGlobal>

class QWidget;

namespace Kasten {

class KASTENGUI_EXPORT UserMessagesHandler : public AbstractUserMessagesHandler
{
public:
    explicit UserMessagesHandler(QWidget* widget = nullptr);
    UserMessagesHandler(const UserMessagesHandler&) = delete;
    UserMessagesHandler(UserMessagesHandler&&) = delete;

    ~UserMessagesHandler() override;

    UserMessagesHandler& operator=(const UserMessagesHandler&) = delete;
    UserMessagesHandler& operator=(UserMessagesHandler&&) = delete;

public: // AbstractUserMessagesHandler API
    void postErrorReport(std::unique_ptr<UserErrorReport>&& errorReport) override;
    void postNotification(std::unique_ptr<UserNotification>&& notification) override;

    [[nodiscard]]
    QString executeQuery(std::unique_ptr<UserQuery>&& userQuery) override;

public:
    void setWidget(QWidget* widget);

private:
    const std::unique_ptr<class UserMessagesHandlerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(UserMessagesHandler)
};

}

#endif
