/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "singledocumentstrategy.hpp"
#include "singledocumentstrategy_p.hpp"

namespace Kasten {

SingleDocumentStrategy::SingleDocumentStrategy(DocumentManager* documentManager,
                                               ViewManager* viewManager)
    : AbstractDocumentStrategy(new SingleDocumentStrategyPrivate(this,
                                                                 documentManager,
                                                                 viewManager))
{
    Q_D(SingleDocumentStrategy);

    d->init();
}

SingleDocumentStrategy::~SingleDocumentStrategy() = default;

QVector<AbstractDocument*> SingleDocumentStrategy::documents() const
{
    Q_D(const SingleDocumentStrategy);

    return d->documents();
}

QStringList SingleDocumentStrategy::supportedRemoteTypes() const
{
    Q_D(const SingleDocumentStrategy);

    return d->supportedRemoteTypes();
}

bool SingleDocumentStrategy::canClose(AbstractDocument* document) const
{
    Q_D(const SingleDocumentStrategy);

    return d->canClose(document);
}

bool SingleDocumentStrategy::canCloseAllOther(AbstractDocument* document) const
{
    Q_D(const SingleDocumentStrategy);

    return d->canCloseAllOther(document);
}

bool SingleDocumentStrategy::canCloseAll() const
{
    Q_D(const SingleDocumentStrategy);

    return d->canCloseAll();
}

void SingleDocumentStrategy::createNew()
{
    Q_D(SingleDocumentStrategy);

    d->createNew();
}

void SingleDocumentStrategy::createNewFromClipboard()
{
    Q_D(SingleDocumentStrategy);

    d->createNewFromClipboard();
}

void SingleDocumentStrategy::createNewWithGenerator(AbstractModelDataGenerator* generator)
{
    Q_D(SingleDocumentStrategy);

    d->createNewWithGenerator(generator);
}

void SingleDocumentStrategy::load(const QUrl& url)
{
    Q_D(SingleDocumentStrategy);

    d->load(url);
}

void SingleDocumentStrategy::closeAll()
{
    Q_D(SingleDocumentStrategy);

    d->closeAll();
}

void SingleDocumentStrategy::closeAllOther(AbstractDocument* document)
{
    Q_D(SingleDocumentStrategy);

    d->closeAllOther(document);
}

void SingleDocumentStrategy::closeDocument(AbstractDocument* document)
{
    Q_D(SingleDocumentStrategy);

    d->closeDocument(document);
}

}
