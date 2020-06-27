/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2008-2009, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewmanager.hpp"
#include "viewmanager_p.hpp"

namespace Kasten {

ViewManager::ViewManager()
    : d_ptr(new ViewManagerPrivate(this))
{
}

ViewManager::~ViewManager() = default;

ModelCodecViewManager* ViewManager::codecViewManager() const
{
    Q_D(const ViewManager);

    return d->codecViewManager();
}

void ViewManager::setViewFactory(AbstractViewFactory* factory)
{
    Q_D(ViewManager);

    d->setViewFactory(factory);
}

QVector<AbstractView*> ViewManager::views() const
{
    Q_D(const ViewManager);

    return d->views();
}

AbstractView* ViewManager::viewByWidget(QWidget* widget) const
{
    Q_D(const ViewManager);

    return d->viewByWidget(widget);
}

void ViewManager::createCopyOfView(AbstractView* view, Qt::Alignment alignment)
{
    Q_D(ViewManager);

    d->createCopyOfView(view, alignment);
}

void ViewManager::createViewsFor(const QVector<Kasten::AbstractDocument*>& documents)
{
    Q_D(ViewManager);

    d->createViewsFor(documents);
}

void ViewManager::removeViewsFor(const QVector<Kasten::AbstractDocument*>& documents)
{
    Q_D(ViewManager);

    d->removeViewsFor(documents);
}

void ViewManager::removeViews(const QVector<AbstractView*>& views)
{
    Q_D(ViewManager);

    d->removeViews(views);
}

}
