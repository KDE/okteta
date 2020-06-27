/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "statusbar.hpp"

// lib
#include "statusbarlayout.hpp"
// Qt
#include <QEvent>

namespace Kasten {

StatusBar::StatusBar(QWidget* parent)
    : QStatusBar(parent)
{
//     setMinimumWidth( 1 );

    QWidget* baseWidget = new QWidget(this);

    mLayout = new StatusBarLayout(baseWidget);
    mLayout->setSpacing(6);   // hard coded in QStatusBar
    QStatusBar::addWidget(baseWidget, 1);
}

StatusBar::~StatusBar() = default;

void StatusBar::addWidget(QWidget* widget)
{
    // ensure statusbar is the parent, as expected also with QStatusBar
    widget->setParent(this);
    mLayout->addWidget(widget);
}

void StatusBar::updateLayout()
{
    mLayout->invalidate();
}

void StatusBar::changeEvent(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::StyleChange:
        mLayout->invalidate();
//         mLayout->updateMarginAndSpacing();
        break;
    default:
        break;
    }

    QStatusBar::changeEvent(event);
}

}
