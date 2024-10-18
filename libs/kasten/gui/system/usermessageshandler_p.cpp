/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "usermessageshandler_p.hpp"

// Kasten Core
#include <Kasten/AbstractModel>
#include <Kasten/UserNotification>
#include <Kasten/UserErrorReportsInlineable>
#include <Kasten/UserNotificationsInlineable>
// KF
#include <KMessageBox>

namespace Kasten {

void UserMessagesHandlerPrivate::postErrorReport(std::unique_ptr<UserErrorReport>&& errorReport)
{
    AbstractModel* targetModel = errorReport->targetModel();
    AbstractModel* model = targetModel ? targetModel->findBaseModelWithInterface<If::UserErrorReportsInlineable*>() : nullptr;
    If::UserErrorReportsInlineable* userErrorReportsInlineable = model ? qobject_cast<If::UserErrorReportsInlineable*>(model) : nullptr;

    // TODO: is non-view model, search a visible view (with most priority)
    if (userErrorReportsInlineable) {
        if (!userErrorReportsInlineable->isErrorReportShown()) {
            userErrorReportsInlineable->showErrorReport(errorReport.get());
        } else {
            enqueueErrorReport(model, std::move(errorReport));
        }
    } else {
        KMessageBox::error(m_widget,
                           errorReport->text(),
                           errorReport->title());
    }
}

void UserMessagesHandlerPrivate::postNotification(std::unique_ptr<UserNotification>&& notification)
{
    AbstractModel* targetModel = notification->targetModel();
    AbstractModel* model = targetModel ? targetModel->findBaseModelWithInterface<If::UserNotificationsInlineable*>() : nullptr;
    If::UserNotificationsInlineable* userNotificationsInlineable = model ? qobject_cast<If::UserNotificationsInlineable*>(model) : nullptr;

    // TODO: is non-view model, search a visible view (with most priority)
    if (userNotificationsInlineable) {
        userNotificationsInlineable->showNotification(notification.get());
    } else {
        KMessageBox::information(m_widget,
                                notification->text(),
                                notification->title());
    }
}

void UserMessagesHandlerPrivate::setWidget(QWidget* widget)
{
    m_widget = widget;
}

void UserMessagesHandlerPrivate::enqueueErrorReport(AbstractModel* model,
                                                    std::unique_ptr<UserErrorReport>&& errorReport)
{
    auto it = m_enqueuedErrorReports.find(model);
    if (it == m_enqueuedErrorReports.end()) {
        it = m_enqueuedErrorReports.emplace(model, std::vector<std::unique_ptr<UserErrorReport>>()).first;
        connect(model, SIGNAL(errorReportHidden()),
                this, SLOT(onErrorReportHidden()));
        connect(model, &QObject::destroyed,
                this, &UserMessagesHandlerPrivate::onModelDestroyed);

    }
    it->second.emplace_back(std::move(errorReport));
}

void UserMessagesHandlerPrivate::onErrorReportHidden()
{
    auto* model = qobject_cast<AbstractModel*>(sender());
    auto it = m_enqueuedErrorReports.find(model);
    if (it == m_enqueuedErrorReports.end()) {
        return;
    }

    auto& errorReports = it->second;
    auto firstReportIt = errorReports.begin();
    auto errorReport = std::move(*firstReportIt);

    if (errorReports.size() == 1) {
        m_enqueuedErrorReports.erase(it);
        disconnect(model, nullptr, this, nullptr);
    } else {
        errorReports.erase(firstReportIt);
    }

    // TODO: ideally it is ensured this handler is the only one
    // using the API, to not get in conflict
    If::UserErrorReportsInlineable* userErrorReportsInlineable =
        qobject_cast<If::UserErrorReportsInlineable*>(model);

    userErrorReportsInlineable->showErrorReport(errorReport.get());
}

void UserMessagesHandlerPrivate::onModelDestroyed(QObject* object)
{
    auto* const model = static_cast<AbstractModel*>(object);
    m_enqueuedErrorReports.erase(model);
}

}

#include "moc_usermessageshandler_p.cpp"
