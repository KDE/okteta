/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "tabbedviews.hpp"
#include "tabbedviews_p.hpp"

namespace Kasten {

TabbedViews::TabbedViews(TabbedViewsPrivate* _d)
    : AbstractGroupedViews(_d)
{
    Q_D(TabbedViews);

    d->init();
}

TabbedViews::TabbedViews()
    : AbstractGroupedViews(new TabbedViewsPrivate(this))
{
    Q_D(TabbedViews);

    d->init();
}

TabbedViews::~TabbedViews() = default;

QVector<AbstractView*> TabbedViews::viewList() const
{
    Q_D(const TabbedViews);

    return d->viewList();
}

int TabbedViews::viewCount() const
{
    Q_D(const TabbedViews);

    return d->viewCount();
}

int TabbedViews::indexOf(AbstractView* view) const
{
    Q_D(const TabbedViews);

    return d->indexOf(view);
}

QWidget* TabbedViews::widget() const
{
    Q_D(const TabbedViews);

    return d->widget();
}

AbstractView* TabbedViews::viewFocus() const
{
    Q_D(const TabbedViews);

    return d->viewFocus();
}

bool TabbedViews::hasFocus() const
{
    Q_D(const TabbedViews);

    return d->hasFocus();
}

void TabbedViews::addViews(const QVector<AbstractView*>& views)
{
    Q_D(TabbedViews);

    d->addViews(views);
}

void TabbedViews::removeViews(const QVector<AbstractView*>& views)
{
    Q_D(TabbedViews);

    d->removeViews(views);
}

void TabbedViews::setCurrentToolInlineView(AbstractToolInlineView* view)
{
    Q_D(TabbedViews);

    d->setCurrentToolInlineView(view);
}

AbstractToolInlineView* TabbedViews::currentToolInlineView() const
{
    Q_D(const TabbedViews);

    return d->currentToolInlineView();
}

void TabbedViews::setViewFocus(AbstractView* view)
{
    Q_D(TabbedViews);

    d->setViewFocus(view);
}

void TabbedViews::setFocus()
{
    Q_D(TabbedViews);

    d->setFocus();
}

}
