/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "togglebutton_p.hpp"
#include "togglebutton.hpp"

namespace Kasten {

ToggleButtonPrivate::ToggleButtonPrivate(ToggleButton* parent) : p(parent)
{
    p->setCheckable(true);
    p->setAutoRaise(true);
    QObject::connect(p, &QAbstractButton::toggled,
                     p, [&](bool checked) { onToggled(checked); });
}

void ToggleButtonPrivate::setOtherState(const QIcon& icon, const QString& text, const QString& toolTip)
{
    mOtherIcon = icon;
    mOtherText = text;
    mOtherToolTip = toolTip;

    if (!text.isEmpty()) {
        const QString currentText = p->text();

        const int currentTextWidth = p->sizeHint().width();

        p->setText(text);
        const int otherTextWidth = p->sizeHint().width();

        p->setText(currentText);

        // TODO: this breaks on new font (size) or style change
        // better would be to reimplement sizeHint()
        p->setFixedWidth(qMax(currentTextWidth, otherTextWidth));
    }
}

void ToggleButtonPrivate::onToggled(bool)
{
    const QIcon otherIcon = mOtherIcon;
    if (!otherIcon.isNull()) {
        mOtherIcon = p->icon();
        p->setIcon(otherIcon);
    }

    const QString otherText = mOtherText;
    if (!otherText.isEmpty()) {
        mOtherText = p->text();
        p->setText(otherText);
    }

    const QString otherToolTip = mOtherToolTip;
    if (!otherToolTip.isEmpty()) {
        mOtherToolTip = p->toolTip();
        p->setToolTip(otherToolTip);
    }
}

}
