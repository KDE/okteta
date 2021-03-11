/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_SEARCHUSERQUERYABLE_HPP
#define KASTEN_IF_SEARCHUSERQUERYABLE_HPP

// libfinddialog
#include "finddirection.hpp"
// Qt
#include <QtPlugin>

namespace Kasten {

namespace If {

class SearchUserQueryable
{
public:
    virtual ~SearchUserQueryable();

public: // API to be implemented
    virtual bool queryContinue(FindDirection direction) const = 0;
};

inline SearchUserQueryable::~SearchUserQueryable() = default;

}
}

// Q_DECLARE_INTERFACE( Kasten::If::UserListable, "org.kde.kasten.if.userlistable/1.0" )

#endif
