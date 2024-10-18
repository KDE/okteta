/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTUSERMESSAGESHANDLER_HPP
#define KASTEN_ABSTRACTUSERMESSAGESHANDLER_HPP

// lib
#include "kastencore_export.hpp"
// Std
#include <memory>

namespace Kasten {

class UserErrorReport;
class UserNotification;

class KASTENCORE_EXPORT AbstractUserMessagesHandler
{
public:
    virtual ~AbstractUserMessagesHandler();

public: // API to be implemented
    virtual void postErrorReport(std::unique_ptr<UserErrorReport>&& errorReport) = 0;
    virtual void postNotification(std::unique_ptr<UserNotification>&& notification) = 0;

};

inline AbstractUserMessagesHandler::~AbstractUserMessagesHandler() = default;

}

#endif
