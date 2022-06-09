/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "labelledtoolbarwidget.hpp"

// Qt
#include <QLabel>
#include <QHBoxLayout>

namespace Kasten {

LabelledToolBarWidget::LabelledToolBarWidget(QLabel* label,
                                             QWidget* widget,
                                             QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    label->setBuddy(widget);

    layout->addWidget(label);
    layout->addWidget(widget);

    setLayout(layout);
}

}
