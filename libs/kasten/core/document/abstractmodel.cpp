/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodel.hpp"
#include "abstractmodel_p.hpp"

namespace Kasten {

AbstractModel::AbstractModel(AbstractModel* baseModel)
    : d_ptr(new AbstractModelPrivate(this, baseModel))
{}

AbstractModel::AbstractModel(AbstractModelPrivate* d)
    : d_ptr(d)
{}

AbstractModel::~AbstractModel() = default;

bool AbstractModel::isModifiable()        const { return false; }
bool AbstractModel::isReadOnly()          const { return true; }

AbstractModel* AbstractModel::baseModel() const
{
    Q_D(const AbstractModel);

    return d->baseModel();
}

void AbstractModel::setBaseModel(AbstractModel* baseModel)
{
    Q_D(AbstractModel);

    d->setBaseModel(baseModel);
}

void AbstractModel::setReadOnly(bool isReadOnly) { Q_UNUSED(isReadOnly) }

}
