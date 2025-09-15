/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "userquery.hpp"
#include "userquery_p.hpp"

namespace Kasten {

UserQuery::UserQuery(AbstractModel* targetModel, const QString& text, const QString& title,
                     UserQuerySeverity severity)
    : d_ptr(std::make_unique<UserQueryPrivate>(targetModel, text, title, severity))
{
}

UserQuery::~UserQuery() = default;

AbstractModel* UserQuery::targetModel() const
{
    Q_D(const UserQuery);

    return d->targetModel();
}

QString UserQuery::text() const
{
    Q_D(const UserQuery);

    return d->text();
}

QString UserQuery::title() const
{
    Q_D(const UserQuery);

    return d->title();
}

UserQuerySeverity UserQuery::severity() const
{
    Q_D(const UserQuery);

    return d->severity();
}

const std::vector<std::unique_ptr<UserResponseOption>>& UserQuery::responseOptions() const
{
    Q_D(const UserQuery);

    return d->responseOptions();
}

void UserQuery::addResponseOption(const KGuiItem& guiItem, const QString& id, UserResponseOptionHints hints)
{
    Q_D(UserQuery);

    d->addResponseOption(guiItem, id, hints);
}

}
