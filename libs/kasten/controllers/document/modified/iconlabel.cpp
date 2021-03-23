/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "iconlabel.hpp"

// Qt
#include <QEvent>
#include <QStyle>

namespace Kasten {

IconLabel::IconLabel(QWidget* parent)
    : QLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    updateSize();
}

void IconLabel::setIcon(const QIcon& icon)
{
    mIcon = icon;
    updatePixmap();
}

void IconLabel::changeEvent(QEvent* event)
{
    QLabel::changeEvent(event);

    if (event->type() == QEvent::PaletteChange || event->type() == QEvent::StyleChange) {
        if (!mIcon.isNull()) {
            updatePixmap();
        }
    }
    if (event->type() == QEvent::StyleChange) {
        updateSize();
    }
}

QSize IconLabel::iconSize() const
{
    const int iconSizeExtent = style()->pixelMetric(QStyle::PM_ButtonIconSize, nullptr, this);
    return QSize(iconSizeExtent, iconSizeExtent);
}

void IconLabel::updateSize()
{
    setFixedSize(iconSize());
}

void IconLabel::updatePixmap()
{
    const QPixmap pixmap = mIcon.pixmap(iconSize());
    setPixmap(pixmap);
}

}
