/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2006,2008-2009,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
