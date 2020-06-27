/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewbox.hpp"

// lib
#include "abstractview.hpp"
// Qt
#include <QVBoxLayout>

namespace Kasten {

ViewBox::ViewBox(AbstractView* view, QWidget* parent)
    : QWidget(parent)
    , mView(view)
{
    QWidget* widget = view->widget();
    setFocusProxy(widget);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(widget);
}

ViewBox::~ViewBox()
{
    mView->widget()->setParent(nullptr);
}

AbstractView* ViewBox::view() const { return mView; }

void ViewBox::add(ViewBox::Area area)
{
    Q_UNUSED(area);
}

}
