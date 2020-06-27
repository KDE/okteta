/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "togglebutton.hpp"
#include "togglebutton_p.hpp"

namespace Kasten {

ToggleButton::ToggleButton(const QIcon& icon, const QString& text, const QString& toolTip, QWidget* parent)
    : QToolButton(parent)
    , d_ptr(new ToggleButtonPrivate(this))
{
    setIcon(icon);
    setText(text);
    setToolTip(toolTip);
}

ToggleButton::ToggleButton(const QString& text, const QString& toolTip, QWidget* parent)
    : QToolButton(parent)
    , d_ptr(new ToggleButtonPrivate(this))
{
    setText(text);
    setToolTip(toolTip);
}

ToggleButton::~ToggleButton() = default;

void ToggleButton::setCheckedState(const QIcon& icon, const QString& text, const QString& toolTip)
{
    Q_D(ToggleButton);

    d->setOtherState(icon, text, toolTip);
}

void ToggleButton::setCheckedState(const QString& text, const QString& toolTip)
{
    Q_D(ToggleButton);

    d->setOtherState(QIcon(), text, toolTip);
}

}
