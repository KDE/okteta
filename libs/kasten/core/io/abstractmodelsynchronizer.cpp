/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodelsynchronizer.hpp"
#include "abstractmodelsynchronizer_p.hpp"

namespace Kasten {

AbstractModelSynchronizer::AbstractModelSynchronizer(std::unique_ptr<AbstractModelSynchronizerPrivate>&& dd)
    : d_ptr(std::move(dd))
{
}

AbstractModelSynchronizer::AbstractModelSynchronizer()
    : d_ptr(std::make_unique<AbstractModelSynchronizerPrivate>(this))
{
}

AbstractModelSynchronizer::~AbstractModelSynchronizer() = default;

QUrl AbstractModelSynchronizer::url() const
{
    Q_D(const AbstractModelSynchronizer);

    return d->url();
}

void AbstractModelSynchronizer::setUrl(const QUrl& url)
{
    Q_D(AbstractModelSynchronizer);

    d->setUrl(url);
}

}

#include "moc_abstractmodelsynchronizer.cpp"
