/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_CHANGESDESCRIBABLE_H
#define OKTETA_CHANGESDESCRIBABLE_H

// Qt
#include <QtCore/QtPlugin>
#include <QtCore/QString>

namespace Okteta
{

class ChangesDescribable
{
  public:
    virtual ~ChangesDescribable();

  public: // set/action
    virtual void openGroupedChange( const QString &description = QString() ) = 0;
    virtual void cancelGroupedChange() = 0;
    virtual void closeGroupedChange( const QString &description = QString() ) = 0;

  public: // get

  public: // signal
};

inline ChangesDescribable::~ChangesDescribable() {}

}

Q_DECLARE_INTERFACE( Okteta::ChangesDescribable, "org.kde.khecore.changesdescribable/1.0" )

#endif
