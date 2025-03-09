/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERRESPONSEOPTION_P_HPP
#define KASTEN_USERRESPONSEOPTION_P_HPP

// lib
#include "userresponseoption.hpp"
// KF
#include <KGuiItem>
// Qt
#include <QString>

namespace Kasten {

class UserResponseOptionPrivate
{
public:
    explicit UserResponseOptionPrivate(const KGuiItem& guiItem, const QString& id, UserResponseOptionHints hints);
    UserResponseOptionPrivate(const UserResponseOptionPrivate&) = delete;
    UserResponseOptionPrivate(UserResponseOptionPrivate&&) = delete;

    ~UserResponseOptionPrivate();

    UserResponseOptionPrivate& operator=(const UserResponseOptionPrivate&) = delete;
    UserResponseOptionPrivate& operator=(UserResponseOptionPrivate&&) = delete;

public:
    [[nodiscard]]
    const KGuiItem& guiItem() const;
    [[nodiscard]]
    const QString& id() const;
    [[nodiscard]]
    UserResponseOptionHints hints() const;

private:
    const KGuiItem m_guiItem;
    const QString m_id;
    const UserResponseOptionHints m_hints;
};

inline UserResponseOptionPrivate::UserResponseOptionPrivate(const KGuiItem& guiItem, const QString& id,
                                                            UserResponseOptionHints hints)
    : m_guiItem(guiItem)
    , m_id(id)
    , m_hints(hints)
{
}

inline UserResponseOptionPrivate::~UserResponseOptionPrivate() = default;

inline const KGuiItem& UserResponseOptionPrivate::guiItem() const { return m_guiItem; }
inline const QString& UserResponseOptionPrivate::id() const { return m_id; }
inline UserResponseOptionHints UserResponseOptionPrivate::hints() const { return m_hints; }

}

#endif
