/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "usererrorreport.hpp"
#include "usererrorreport_p.hpp"

namespace Kasten {

UserErrorReport::UserErrorReport(AbstractModel* targetModel, const QString& text, const QString& title)
    : d_ptr(std::make_unique<UserErrorReportPrivate>(targetModel, text, title))
{
}

UserErrorReport::~UserErrorReport() = default;

AbstractModel* UserErrorReport::targetModel() const
{
    Q_D(const UserErrorReport);

    return d->targetModel();
}

QString UserErrorReport::text() const
{
    Q_D(const UserErrorReport);

    return d->text();
}

QString UserErrorReport::title() const
{
    Q_D(const UserErrorReport);

    return d->title();
}

}
