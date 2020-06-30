/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "singleviewarea.hpp"
#include "singleviewarea_p.hpp"

namespace Kasten {
// TODO: catch area focus change!
SingleViewArea::SingleViewArea()
    : AbstractViewArea(new SingleViewAreaPrivate(this))
{
    Q_D(SingleViewArea);

    d->init();
}

SingleViewArea::~SingleViewArea() = default;

QWidget* SingleViewArea::widget() const
{
    Q_D(const SingleViewArea);

    return d->widget();
}

bool SingleViewArea::hasFocus() const
{
    Q_D(const SingleViewArea);

    return d->hasFocus();
}

void SingleViewArea::setFocus()
{
    Q_D(SingleViewArea);

    d->setFocus();
}

void SingleViewArea::setCurrentToolInlineView(AbstractToolInlineView* view)
{
    Q_D(SingleViewArea);

    d->setCurrentToolInlineView(view);
}

AbstractToolInlineView* SingleViewArea::currentToolInlineView() const
{
    Q_D(const SingleViewArea);

    return d->currentToolInlineView();
}

void SingleViewArea::setView(AbstractView* view)
{
    Q_D(SingleViewArea);

    d->setView(view);
}

}
