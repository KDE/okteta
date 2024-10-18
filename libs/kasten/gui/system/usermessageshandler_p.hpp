/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VUSERMESSAGESHANDLER_P_HPP
#define KASTEN_VUSERMESSAGESHANDLER_P_HPP

// lib
#include "usermessageshandler.hpp"
// Kasten Core
#include <Kasten/UserErrorReport>
// Qt
#include <QObject>
// Std
#include <unordered_map>
#include <vector>

namespace Kasten {

class AbstractModel;

class UserMessagesHandlerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit UserMessagesHandlerPrivate(QWidget* widget);
    UserMessagesHandlerPrivate(const UserMessagesHandlerPrivate&) = delete;

    ~UserMessagesHandlerPrivate();

    UserMessagesHandlerPrivate& operator=(const UserMessagesHandlerPrivate&) = delete;

public:
    void postErrorReport(std::unique_ptr<UserErrorReport>&& errorReport);
    void postNotification(std::unique_ptr<UserNotification>&& notification);

public:
    void setWidget(QWidget* widget);

private:
    void enqueueErrorReport(AbstractModel* model, std::unique_ptr<UserErrorReport>&& errorReport);

private Q_SLOTS:
    void onErrorReportHidden();
    void onModelDestroyed(QObject* object);

private:
    QWidget* m_widget;

    std::unordered_map<QObject*, std::vector<std::unique_ptr<UserErrorReport>>> m_enqueuedErrorReports;
};

inline UserMessagesHandlerPrivate::UserMessagesHandlerPrivate(QWidget* widget)
    : m_widget(widget)
{
}

inline UserMessagesHandlerPrivate::~UserMessagesHandlerPrivate() = default;

}

#endif
