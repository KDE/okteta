/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "bytearrayfilterfactory.h"

// lib
#include "filter/andbytearrayfilter.h"
#include "filter/orbytearrayfilter.h"
#include "filter/xorbytearrayfilter.h"
#include "filter/invertbytearrayfilter.h"
#include "filter/reversebytearrayfilter.h"
#include "filter/rotatebytearrayfilter.h"
#include "filter/shiftbytearrayfilter.h"
// NEWFILTER(start)
// Here add the name of your header file of your filter,
// e.g.
// #include "filter/my_bytearrayfilter.h"
// NEWFILTER(end)


QList<AbstractByteArrayFilter*> ByteArrayFilterFactory::createFilters()
{
    QList<AbstractByteArrayFilter*> result;

    result
        << new AndByteArrayFilter()
        << new OrByteArrayFilter()
        << new XOrByteArrayFilter()
        << new InvertByteArrayFilter()
        << new ReverseByteArrayFilter()
        << new RotateByteArrayFilter()
        << new ShiftByteArrayFilter();
// NEWFILTER(start)
// Here add the creation of an object of your filter class and add it to the list,
// e.g.
//     result
//         << new My_ByteArrayFilter();
// NEWFILTER(end)

    return result;
}
