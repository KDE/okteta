/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstracttoolwidget_p.hpp"

// Qt
#include <QPushButton>
#include <QShortcut>

namespace Kasten {

AbstractToolWidgetPrivate::AbstractToolWidgetPrivate(AbstractToolWidget* parent)
    : p(parent)

{
    mReturnShortcut = new QShortcut(Qt::Key_Return, p);   // TODO: what about Enter?
    mReturnShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    QObject::connect(mReturnShortcut, &QShortcut::activated,
                     p, [&]() { onReturnPressed(); });
}

void AbstractToolWidgetPrivate::addButton(QPushButton* button, AbstractToolWidget::DefaultType defaultType)
{
    button->setAutoDefault(true);
    if (defaultType == AbstractToolWidget::Default) {
        if (mDefaultButton) {
            mDefaultButton->setDefault(false);
        }

        // TODO: catch button deletion
        mDefaultButton = button;
        button->setDefault(true);
    }
}

void AbstractToolWidgetPrivate::onReturnPressed()
{
    if (mDefaultButton) {
        mDefaultButton->click();
    }
}

}
