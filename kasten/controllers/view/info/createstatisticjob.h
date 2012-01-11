/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#ifndef CREATESTATISTICJOB_H
#define CREATESTATISTICJOB_H

// Okteta core
#include <addressrange.h>
// Qt
#include <QtCore/QObject>

namespace Okteta {
class AbstractByteArrayModel;
}


namespace Kasten2
{

class CreateStatisticJob : public QObject // not yet: KJob
{
  Q_OBJECT

  public:
    CreateStatisticJob( const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& selection,
                        int* byteCount );

  public:
    // return size of selected byte array
    int exec();

  protected:
    const Okteta::AbstractByteArrayModel* mByteArrayModel;
    const Okteta::AddressRange mSelection;

    int* const mByteCount;
};


inline CreateStatisticJob::CreateStatisticJob( const Okteta::AbstractByteArrayModel* model,
                                               const Okteta::AddressRange& selection,
                                               int* byteCount )
 : mByteArrayModel( model ), mSelection( selection ), mByteCount( byteCount )
{}

}

#endif
