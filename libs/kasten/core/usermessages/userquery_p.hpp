/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERQUERY_P_HPP
#define KASTEN_USERQUERY_P_HPP

// lib
#include "userquery.hpp"

namespace Kasten {

class UserQueryPrivate
{
public:
    explicit UserQueryPrivate(AbstractModel* targetModel, const QString& text, const QString& title,
                              UserQuerySeverity severity);
    UserQueryPrivate(const UserQueryPrivate&) = delete;

    ~UserQueryPrivate();

    UserQueryPrivate& operator=(const UserQueryPrivate&) = delete;

public:
    [[nodiscard]]
    AbstractModel* targetModel() const;
    [[nodiscard]]
    const QString& text() const;
    [[nodiscard]]
    const QString& title() const;
    [[nodiscard]]
    UserQuerySeverity severity() const;
    [[nodiscard]]
    const std::vector<std::unique_ptr<UserResponseOption>>& responseOptions() const;

    void addResponseOption(const KGuiItem& guiItem, const QString& id, UserResponseOptionHints hints);

private:
    AbstractModel* const m_targetModel;
    const QString m_text;
    const QString m_title;
    const UserQuerySeverity m_severity;

    std::vector<std::unique_ptr<UserResponseOption>> m_responseOptions;
};

inline UserQueryPrivate::UserQueryPrivate(AbstractModel* targetModel, const QString& text, const QString& title,
                                          UserQuerySeverity severity)
    : m_targetModel(targetModel)
    , m_text(text)
    , m_title(title)
    , m_severity(severity)
{
}

inline UserQueryPrivate::~UserQueryPrivate() = default;

inline AbstractModel* UserQueryPrivate::targetModel() const { return m_targetModel; }
inline const QString& UserQueryPrivate::text() const { return m_text; }
inline const QString& UserQueryPrivate::title() const { return m_title; }
inline UserQuerySeverity UserQueryPrivate::severity() const { return m_severity; }

inline const std::vector<std::unique_ptr<UserResponseOption>>& UserQueryPrivate::responseOptions() const
{
    return m_responseOptions;
}

inline void UserQueryPrivate::addResponseOption(const KGuiItem& guiItem, const QString& id, UserResponseOptionHints hints)
{
    // TODO: currently UserMessagesHandler implementation limits to use 3 options only
    Q_ASSERT(m_responseOptions.size() < 3);
    m_responseOptions.emplace_back(std::make_unique<UserResponseOption>(guiItem, id, hints));
}

}

#endif
