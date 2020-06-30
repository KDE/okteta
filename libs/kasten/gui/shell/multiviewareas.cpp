/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "multiviewareas.hpp"
#include "multiviewareas_p.hpp"

namespace Kasten {
// TODO: catch area focus change!
MultiViewAreas::MultiViewAreas()
    : AbstractGroupedViews(new MultiViewAreasPrivate(this))
{
    Q_D(MultiViewAreas);

    d->init();
}

MultiViewAreas::~MultiViewAreas() = default;

QVector<AbstractView*> MultiViewAreas::viewList() const
{
    Q_D(const MultiViewAreas);

    return d->viewList();
}

int MultiViewAreas::viewCount() const
{
    Q_D(const MultiViewAreas);

    return d->viewCount();
}

int MultiViewAreas::indexOf(AbstractView* view) const
{
    Q_D(const MultiViewAreas);

    return d->indexOf(view);
}

QWidget* MultiViewAreas::widget() const
{
    Q_D(const MultiViewAreas);

    return d->widget();
}

bool MultiViewAreas::hasFocus() const
{
    Q_D(const MultiViewAreas);

    return d->hasFocus();
}

AbstractView* MultiViewAreas::viewFocus() const
{
    Q_D(const MultiViewAreas);

    return d->viewFocus();
}

AbstractViewArea* MultiViewAreas::viewAreaFocus() const
{
    Q_D(const MultiViewAreas);

    return d->viewAreaFocus();
}

int MultiViewAreas::viewAreasCount() const
{
    Q_D(const MultiViewAreas);

    return d->viewAreasCount();
}

void MultiViewAreas::setFocus()
{
    Q_D(MultiViewAreas);

    d->setFocus();
}

void MultiViewAreas::addViews(const QVector<AbstractView*>& views)
{
    Q_D(MultiViewAreas);

    d->addViews(views);
}

void MultiViewAreas::removeViews(const QVector<AbstractView*>& views)
{
    Q_D(MultiViewAreas);

    d->removeViews(views);
}

void MultiViewAreas::setCurrentToolInlineView(AbstractToolInlineView* view)
{
    Q_D(MultiViewAreas);

    d->setCurrentToolInlineView(view);
}

AbstractToolInlineView* MultiViewAreas::currentToolInlineView() const
{
    Q_D(const MultiViewAreas);

    return d->currentToolInlineView();
}

void MultiViewAreas::setViewFocus(AbstractView* view)
{
    Q_D(MultiViewAreas);

    d->setViewFocus(view);
}

// TODO: this method could be removed, as it is the same as viewArea->setFocus(), or?
void MultiViewAreas::setViewAreaFocus(AbstractViewArea* viewArea)
{
    Q_D(MultiViewAreas);

    d->setViewAreaFocus(viewArea);
}

AbstractViewArea* MultiViewAreas::splitViewArea(AbstractViewArea* viewArea, Qt::Orientation orientation)
{
    Q_D(MultiViewAreas);

    return d->splitViewArea(viewArea, orientation);
}

void MultiViewAreas::closeViewArea(AbstractViewArea* viewArea)
{
    Q_D(MultiViewAreas);

    d->closeViewArea(viewArea);
}

}
