/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "userresponseoption.hpp"
#include "userresponseoption_p.hpp"

namespace Kasten {

UserResponseOption::UserResponseOption(const KGuiItem& guiItem, const QString& id, UserResponseOptionHints hints)
    : d_ptr(std::make_unique<UserResponseOptionPrivate>(guiItem, id, hints))
{
}

UserResponseOption::~UserResponseOption() = default;

KGuiItem UserResponseOption::guiItem() const
{
    Q_D(const UserResponseOption);

    return d->guiItem();
}

QString UserResponseOption::id() const
{
    Q_D(const UserResponseOption);

    return d->id();
}

UserResponseOptionHints UserResponseOption::hints() const
{
    Q_D(const UserResponseOption);

    return d->hints();
}

}
