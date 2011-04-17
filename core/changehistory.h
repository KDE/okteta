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

#ifndef OKTETA_CHANGEHISTORY_H
#define OKTETA_CHANGEHISTORY_H

// lib
#include "bytearraychange.h"
// Qt
#include <QtCore/QList>
#include <QtCore/QtPlugin>

class QByteArray;


namespace Okteta
{

// TODO: split readonly part into own interface
class ChangeHistory
{
  public:
    virtual ~ChangeHistory();

  public: // get
    virtual QList<ByteArrayChange> changes( int firstVersionIndex, int lastVersionIndex ) const = 0;
    virtual QByteArray initialData() const = 0;

  public: // set
    virtual void doChanges( const QList<Okteta::ByteArrayChange>& changes,
                                  int oldVersionIndex, int newVersionIndex ) = 0;

  public: // signal
    virtual void changesDone( const QList<Okteta::ByteArrayChange>& changes,
                                    int oldVersionIndex, int newVersionIndex ) = 0;
};

inline ChangeHistory::~ChangeHistory() {}

}

Q_DECLARE_INTERFACE( Okteta::ChangeHistory, "org.kde.khecore.changehistory/1.0" )

#endif
