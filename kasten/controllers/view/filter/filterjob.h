/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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
#include <section.h>
// Qt
#include <QtCore/QObject>

class AbstractByteArrayFilter;

namespace Okteta {
class AbstractByteArrayModel;
}


namespace Kasten
{

class FilterJob : public QObject // not yet: KJob
{
  Q_OBJECT

  public:
    FilterJob( AbstractByteArrayFilter *byteArrayFilter,
               char *result, Okteta::AbstractByteArrayModel *model, const KDE::Section &section );

  public:
    bool exec();

  protected Q_SLOTS:
    void onFilteredBytes();

  protected:
    AbstractByteArrayFilter *mByteArrayFilter;

    char *mResult;
    Okteta::AbstractByteArrayModel *mModel;
    const KDE::Section mSection;
};


inline FilterJob::FilterJob( AbstractByteArrayFilter *byteArrayFilter,
               char *result, Okteta::AbstractByteArrayModel *model, const KDE::Section &section )
 : mByteArrayFilter( byteArrayFilter ), mResult( result ), mModel( model ), mSection( section )
{}

}

#endif
