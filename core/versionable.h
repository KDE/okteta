/*
    This file is part of the Okteta Core library, made within the KDE community.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_VERSIONABLE_H
#define OKTETA_VERSIONABLE_H

// Qt
#include <QtCore/QtPlugin>

class QString;

namespace Okteta
{

class Versionable
{
  public:
    virtual ~Versionable();

  public: // get
    virtual int versionIndex() const = 0;
    virtual int versionCount() const = 0;
    virtual QString versionDescription( int versionIndex ) const = 0;

  public: // set/action
    virtual void revertToVersionByIndex( int versionIndex ) = 0;

  public: // signal
    virtual void revertedToVersionIndex( int versionIndex ) = 0;
    virtual void headVersionDescriptionChanged( const QString &versionDescription ) = 0;
    virtual void headVersionChanged( int newHeadVersionIndex ) = 0;
};

inline Versionable::~Versionable() {}

}

Q_DECLARE_INTERFACE( Okteta::Versionable, "org.kde.khecore.versionable/1.0" )

#endif
