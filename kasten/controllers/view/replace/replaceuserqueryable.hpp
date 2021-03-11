/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_REPLACEUSERQUERYABLE_HPP
#define KASTEN_IF_REPLACEUSERQUERYABLE_HPP

// libfinddialog
#include "finddirection.hpp"
// Qt
#include <QtPlugin>

namespace Kasten {

enum ReplaceBehaviour
{
    ReplaceAll,
    SkipCurrent,
    ReplaceCurrent,
    CancelReplacing
};

namespace If {

class ReplaceUserQueryable
{
public:
    virtual ~ReplaceUserQueryable();

public: // API to be implemented
    virtual void queryContinue(FindDirection direction, int noOfReplacements) = 0;
    virtual void queryReplaceCurrent() = 0;

public: // signals
    virtual void queryContinueFinished(bool result) = 0;
    virtual void queryReplaceCurrentFinished(Kasten::ReplaceBehaviour result) = 0;
};

inline ReplaceUserQueryable::~ReplaceUserQueryable() = default;

}
}

Q_DECLARE_METATYPE(Kasten::ReplaceBehaviour)
Q_DECLARE_INTERFACE(Kasten::If::ReplaceUserQueryable, "org.kde.kasten.if.replaceuserqueryable/1.0")

#endif
