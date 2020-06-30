/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayfilterfactory.hpp"

// lib
#include "filter/andbytearrayfilter.hpp"
#include "filter/orbytearrayfilter.hpp"
#include "filter/xorbytearrayfilter.hpp"
#include "filter/invertbytearrayfilter.hpp"
#include "filter/reversebytearrayfilter.hpp"
#include "filter/rotatebytearrayfilter.hpp"
#include "filter/shiftbytearrayfilter.hpp"
// NEWFILTER(start)
// Here add the name of your header file of your filter,
// e.g.
// #include "filter/my_bytearrayfilter.hpp"
// NEWFILTER(end)
// Qt
#include <QVector>

QVector<AbstractByteArrayFilter*> ByteArrayFilterFactory::createFilters()
{
    const QVector<AbstractByteArrayFilter*> result {
        new AndByteArrayFilter(),
        new OrByteArrayFilter(),
        new XOrByteArrayFilter(),
        new InvertByteArrayFilter(),
        new ReverseByteArrayFilter(),
        new RotateByteArrayFilter(),
        new ShiftByteArrayFilter(),
// NEWFILTER(start)
// Here add the creation of an object of your filter class and add it to the list,
// e.g.
//         new My_ByteArrayFilter(),
// NEWFILTER(end)
    };
    return result;
}
