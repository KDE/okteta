/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "toolinlineviewwidget.hpp"

// Qt
#include <QIcon>
#include <QHBoxLayout>
#include <QToolButton>
#include <QStyle>

namespace Kasten {

ToolInlineViewWidget::ToolInlineViewWidget(QWidget* viewWidget, QWidget* parent)
    : QWidget(parent)
    , mViewWidget(viewWidget)
{
    auto* layout = new QHBoxLayout(this);
    layout->addWidget(mViewWidget, 10);

    // TODO: use style buttons instead, like QDockWidget
    auto* closeButton = new QToolButton(this);
    closeButton->setAutoRaise(true);
    closeButton->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    connect(closeButton, &QAbstractButton::clicked, this, &ToolInlineViewWidget::done);
    layout->addWidget(closeButton);
    layout->setAlignment(closeButton, (Qt::Alignment)(Qt::AlignLeft | Qt::AlignTop));
}

ToolInlineViewWidget::~ToolInlineViewWidget()
{
    if (mViewWidget) {
        layout()->removeWidget(mViewWidget);
        mViewWidget->setParent(nullptr);
    }
}

}
