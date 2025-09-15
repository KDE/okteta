/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodeldatagenerator.hpp"
#include "abstractmodeldatagenerator_p.hpp"

namespace Kasten {

AbstractModelDataGenerator::AbstractModelDataGenerator(std::unique_ptr<AbstractModelDataGeneratorPrivate>&& dd)
    : d_ptr(std::move(dd))
{
}

AbstractModelDataGenerator::AbstractModelDataGenerator(const QString& typeName,
                                                       const QString& mimeType, Flags flags)
    : d_ptr(std::make_unique<AbstractModelDataGeneratorPrivate>(this, typeName, mimeType, flags))
{
}

AbstractModelDataGenerator::~AbstractModelDataGenerator() = default;

QString AbstractModelDataGenerator::typeName() const
{
    Q_D(const AbstractModelDataGenerator);

    return d->typeName();
}

QString AbstractModelDataGenerator::mimeType() const
{
    Q_D(const AbstractModelDataGenerator);

    return d->mimeType();
}

AbstractModelDataGenerator::Flags AbstractModelDataGenerator::flags() const
{
    Q_D(const AbstractModelDataGenerator);

    return d->flags();
}

}

#include "moc_abstractmodeldatagenerator.cpp"
