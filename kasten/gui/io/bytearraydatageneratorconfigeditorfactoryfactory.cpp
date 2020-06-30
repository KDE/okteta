/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraydatageneratorconfigeditorfactoryfactory.hpp"

// lib
#include "generator/pattern/bytearraypatterngeneratorconfigeditorfactory.hpp"
#include "generator/randomdata/bytearrayrandomdatageneratorconfigeditorfactory.hpp"
//// NEWBYTEARRAYDATAGENERATORCONFIGEDITORFACTORY(start)
//// Here add the name of your header file of your configeditorfactory,
//// e.g.
//// #include "my_bytearraydatageneratorconfigeditorfactory.hpp"
//// NEWBYTEARRAYDATAGENERATORCONFIGEDITORFACTORY(end)
// Qt
#include <QVector>

namespace Kasten {

QVector<AbstractModelDataGeneratorConfigEditorFactory*> ByteArrayDataGeneratorConfigEditorFactoryFactory::createFactorys()
{
    const QVector<AbstractModelDataGeneratorConfigEditorFactory*> result {
        new ByteArrayPatternGeneratorConfigEditorFactory(),
        new ByteArrayRandomDataGeneratorConfigEditorFactory(),
//// NEWBYTEARRAYDATAGENERATORCONFIGEDITORFACTORY(start)
//// Here add the creation of an object of your configeditorfactory class and add it to the list,
//// e.g.
////         new My_ByteArrayDataGeneratorConfigEditorFactory();,
//// NEWBYTEARRAYDATAGENERATORCONFIGEDITORFACTORY(end)
    };

    return result;
}

}
