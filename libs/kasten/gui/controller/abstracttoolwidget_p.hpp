/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTTOOLWIDGET_P_HPP
#define KASTEN_ABSTRACTTOOLWIDGET_P_HPP

// lib
#include "abstracttoolwidget.hpp"

class QShortcut;

namespace Kasten {

class AbstractToolWidget;

class AbstractToolWidgetPrivate
{
public:
    explicit AbstractToolWidgetPrivate(AbstractToolWidget* parent);

public:
    void addButton(QPushButton* button, AbstractToolWidget::DefaultType defaultType);

public:
    void onReturnPressed();

private:
    AbstractToolWidget* const p;

    QPushButton* mDefaultButton = nullptr;
    QShortcut* mReturnShortcut;
};

}

#endif
