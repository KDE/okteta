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

#ifndef FILTERJOB_H
#define FILTERJOB_H

// Okteta core
#include <addressrange.h>
#include <byte.h>
// Qt
#include <QtCore/QObject>

class AbstractByteArrayFilter;

namespace Okteta {
class AbstractByteArrayModel;
}


namespace Kasten2
{

class FilterJob : public QObject // not yet: KJob
{
  Q_OBJECT

  public:
    FilterJob( AbstractByteArrayFilter* byteArrayFilter,
               Okteta::Byte* result, Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range );

  public:
    bool exec();

  protected Q_SLOTS:
    void onFilteredBytes();

  protected:
    AbstractByteArrayFilter* mByteArrayFilter;

    Okteta::Byte* mResult;
    Okteta::AbstractByteArrayModel* mModel;
    const Okteta::AddressRange mRange;
};


inline FilterJob::FilterJob( AbstractByteArrayFilter* byteArrayFilter,
               Okteta::Byte* result, Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range )
 : mByteArrayFilter( byteArrayFilter ), mResult( result ), mModel( model ), mRange( range )
{}

}

#endif
