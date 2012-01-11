/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KDE_IF_REPLACEUSERQUERYABLE_H
#define KDE_IF_REPLACEUSERQUERYABLE_H

// libfinddialog
#include <kfinddirection.h>
// Qt
#include <QtCore/QtPlugin>


namespace Kasten2
{

enum ReplaceBehaviour { ReplaceAll, SkipCurrent, ReplaceCurrent, CancelReplacing };


namespace If
{

class ReplaceUserQueryable
{
  public:
    virtual ~ReplaceUserQueryable();

  public: // API to be implemented
    virtual bool queryContinue( KFindDirection direction, int noOfReplacements ) const = 0;
    virtual ReplaceBehaviour queryReplaceCurrent() const = 0;
};

inline ReplaceUserQueryable::~ReplaceUserQueryable() {}

}
}

Q_DECLARE_INTERFACE( Kasten2::If::ReplaceUserQueryable, "org.kde.kasten2.if.userlistable/1.0" )

#endif
