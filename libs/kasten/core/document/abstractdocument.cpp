/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractdocument.hpp"
#include "abstractdocument_p.hpp"

namespace Kasten {

AbstractDocument::AbstractDocument()
    : AbstractModel(new AbstractDocumentPrivate(this))
{}

AbstractDocument::~AbstractDocument() = default;

QString AbstractDocument::id() const
{
    Q_D(const AbstractDocument);

    return d->id();
}
AbstractModelSynchronizer* AbstractDocument::synchronizer() const
{
    Q_D(const AbstractDocument);

    return d->synchronizer();
}

void AbstractDocument::setId(const QString& id)
{
    Q_D(AbstractDocument);

    d->setId(id);
}
void AbstractDocument::setSynchronizer(AbstractModelSynchronizer* synchronizer)
{
    Q_D(AbstractDocument);

    d->setSynchronizer(synchronizer);
}

}
