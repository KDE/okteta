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

#ifndef OKTETA_ARRAYCHANGEMETRICSLIST_H
#define OKTETA_ARRAYCHANGEMETRICSLIST_H

// lib
#include "arraychangemetrics.h"
//
#include <QtCore/QList>


namespace Okteta
{

// TODO: do we need the invalid status?
class ArrayChangeMetricsList : public QList<ArrayChangeMetrics>
{
  public:
    static ArrayChangeMetricsList oneReplacement( Address offset, Size removeLength, Size insertLength );
    static ArrayChangeMetricsList oneSwapping( Address firstOffset, Size secondOffset, Size secondLength );

  public:
    ArrayChangeMetricsList();
    explicit ArrayChangeMetricsList( const ArrayChangeMetrics& metrics );
};

inline ArrayChangeMetricsList ArrayChangeMetricsList::oneReplacement( Address offset, Size removeLength, Size insertLength )
{
    return ArrayChangeMetricsList( ArrayChangeMetrics::asReplacement(offset,removeLength,insertLength) );
}

inline ArrayChangeMetricsList ArrayChangeMetricsList::oneSwapping( Address firstOffset, Size secondOffset, Size secondLength )
{
    return ArrayChangeMetricsList( ArrayChangeMetrics::asSwapping(firstOffset,secondOffset,secondLength) );
}

inline ArrayChangeMetricsList::ArrayChangeMetricsList() {}
inline ArrayChangeMetricsList::ArrayChangeMetricsList( const ArrayChangeMetrics& metrics )
{
    append( metrics );
}

}

#endif
