/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERQUERY_HPP
#define KASTEN_USERQUERY_HPP

// lib
#include "kastencore_export.hpp"
#include "userresponseoption.hpp"
// Qt
#include <QtGlobal>
#include <QString>
// Std
#include <memory>
#include <vector>

class KGuiItem;

namespace Kasten {

class AbstractModel;

enum UserQuerySeverity {
    UserQueryInformationSeverity,
    UserQueryWarningSeverity,
};

class KASTENCORE_EXPORT UserQuery
{
public:
    explicit UserQuery(AbstractModel* targetModel, const QString& text, const QString& title = QString(),
                       UserQuerySeverity severity = UserQueryInformationSeverity);
    UserQuery(const UserQuery&) = delete;
    UserQuery(UserQuery&&) = delete;

    ~UserQuery();

public:
    UserQuery& operator=(const UserQuery&) = delete;
    UserQuery& operator=(UserQuery&&) = delete;

public:
    [[nodiscard]]
    AbstractModel* targetModel() const;
    [[nodiscard]]
    QString text() const;
    [[nodiscard]]
    QString title() const;
    [[nodiscard]]
    UserQuerySeverity severity() const;
    [[nodiscard]]
    const std::vector<std::unique_ptr<UserResponseOption>>& responseOptions() const;

    void addResponseOption(const KGuiItem& guiItem, const QString& id, UserResponseOptionHints hints = UserResponseNoHints);

private:
    const std::unique_ptr<class UserQueryPrivate> d_ptr;
    Q_DECLARE_PRIVATE(UserQuery)
};

}

#endif
