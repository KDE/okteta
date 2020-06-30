/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraydatageneratorfactory.hpp"

// lib
#include "generator/pattern/bytearraypatterngenerator.hpp"
#include "generator/randomdata/bytearrayrandomdatagenerator.hpp"
#include "generator/sequence/bytearraysequencegenerator.hpp"
//// NEWBYTEARRAYDATAGENERATOR(start)
//// Here add the name of your header file of your datagenerator,
//// e.g.
//// #include "my_bytearraydatagenerator.hpp"
//// NEWBYTEARRAYDATAGENERATOR(end)
// Qt
#include <QVector>

namespace Kasten {

QVector<AbstractModelDataGenerator*> ByteArrayDataGeneratorFactory::createDataGenerators()
{
    const QVector<AbstractModelDataGenerator*> result {
        new ByteArrayPatternGenerator(),
        new ByteArrayRandomDataGenerator(),
        new ByteArraySequenceGenerator(),
//// NEWBYTEARRAYDATAGENERATOR(start)
//// Here add the creation of an object of your datagenerator class and add it to the list,
//// e.g.
////         new My_ByteArrayDataGenerator(),
//// NEWBYTEARRAYDATAGENERATOR(end)
    };
    return result;
}

}
