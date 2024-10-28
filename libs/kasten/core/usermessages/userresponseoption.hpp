/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERRESPONSEOPTION_HPP
#define KASTEN_USERRESPONSEOPTION_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QtGlobal>
#include <QFlags>
// Std
#include <memory>

class KGuiItem;

namespace Kasten {

class AbstractModel;

enum UserResponseOptionHint {
    UserResponseNoHints = 0,
    UserResponseCancelHint = 0x1, // TODO: there is only one, make instead explicit option setter/getter?
    UserResponseDefaultHint = 0x2,
};
Q_DECLARE_FLAGS(UserResponseOptionHints, UserResponseOptionHint)
Q_DECLARE_OPERATORS_FOR_FLAGS(UserResponseOptionHints)

// TODO: using KGuiItem for now, to be later replaced by custom predefined items
class KASTENCORE_EXPORT UserResponseOption
{
public:
    explicit UserResponseOption(const KGuiItem& guiItem, const QString& id, UserResponseOptionHints hints = UserResponseNoHints);
    UserResponseOption(const UserResponseOption&) = delete;

    ~UserResponseOption();

public:
    UserResponseOption& operator=(const UserResponseOption&) = delete;

public:
    KGuiItem guiItem() const;
    QString id() const;
    UserResponseOptionHints hints() const;

private:
    const std::unique_ptr<class UserResponseOptionPrivate> d_ptr;
    Q_DECLARE_PRIVATE(UserResponseOption)
};

}

#endif
