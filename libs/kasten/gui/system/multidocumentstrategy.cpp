/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "multidocumentstrategy.hpp"
#include "multidocumentstrategy_p.hpp"

namespace Kasten {

MultiDocumentStrategy::MultiDocumentStrategy(DocumentManager* documentManager,
                                             ViewManager* viewManager)
    : AbstractDocumentStrategy(new MultiDocumentStrategyPrivate(this,
                                                                documentManager,
                                                                viewManager))
{
    Q_D(MultiDocumentStrategy);

    d->init();
}

MultiDocumentStrategy::~MultiDocumentStrategy() = default;

QVector<AbstractDocument*> MultiDocumentStrategy::documents() const
{
    Q_D(const MultiDocumentStrategy);

    return d->documents();
}

QStringList MultiDocumentStrategy::supportedRemoteTypes() const
{
    Q_D(const MultiDocumentStrategy);

    return d->supportedRemoteTypes();
}

bool MultiDocumentStrategy::canClose(AbstractDocument* document) const
{
    Q_D(const MultiDocumentStrategy);

    return d->canClose(document);
}

bool MultiDocumentStrategy::canCloseAllOther(AbstractDocument* document) const
{
    Q_D(const MultiDocumentStrategy);

    return d->canCloseAllOther(document);
}

bool MultiDocumentStrategy::canCloseAll() const
{
    Q_D(const MultiDocumentStrategy);

    return d->canCloseAll();
}

void MultiDocumentStrategy::createNew()
{
    Q_D(MultiDocumentStrategy);

    d->createNew();
}

void MultiDocumentStrategy::createNewFromClipboard()
{
    Q_D(MultiDocumentStrategy);

    d->createNewFromClipboard();
}

void MultiDocumentStrategy::createNewWithGenerator(AbstractModelDataGenerator* generator)
{
    Q_D(MultiDocumentStrategy);

    d->createNewWithGenerator(generator);
}

void MultiDocumentStrategy::load(const QUrl& url)
{
    Q_D(MultiDocumentStrategy);

    d->load(url);
}

void MultiDocumentStrategy::closeAll()
{
    Q_D(MultiDocumentStrategy);

    d->closeAll();
}

void MultiDocumentStrategy::closeAllOther(AbstractDocument* document)
{
    Q_D(MultiDocumentStrategy);

    d->closeAllOther(document);
}

void MultiDocumentStrategy::closeDocument(AbstractDocument* document)
{
    Q_D(MultiDocumentStrategy);

    d->closeDocument(document);
}

}
