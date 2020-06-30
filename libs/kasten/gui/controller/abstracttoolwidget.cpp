/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstracttoolwidget.hpp"
#include "abstracttoolwidget_p.hpp"

namespace Kasten {

AbstractToolWidget::AbstractToolWidget(QWidget* parent)
    : QWidget(parent)
    , d_ptr(new AbstractToolWidgetPrivate(this))
{
}

AbstractToolWidget::~AbstractToolWidget() = default;

void AbstractToolWidget::addButton(QPushButton* button, DefaultType defaultType)
{
    Q_D(AbstractToolWidget);

    d->addButton(button, defaultType);
}

}
