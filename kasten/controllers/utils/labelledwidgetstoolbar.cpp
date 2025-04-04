/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "labelledwidgetstoolbar.hpp"

// lib
#include "labelledtoolbarwidget.hpp"
// Qt
#include <QActionEvent>
#include <QLayoutItem>
#include <QLayout>

namespace Kasten {

LabelledWidgetsToolBar::LabelledWidgetsToolBar(QWidget* parent)
    : QToolBar(parent)
{
}

void LabelledWidgetsToolBar::actionEvent(QActionEvent* actionEvent)
{
    if (actionEvent->type() == QEvent::ActionRemoved) {
        auto* const firstLayoutItem = layout()->itemAt(0);
        if (firstLayoutItem) {
            auto* const labelledToolBarWidget = qobject_cast<LabelledToolBarWidget*>(firstLayoutItem->widget());
            if (labelledToolBarWidget) {
                labelledToolBarWidget->setHasLabelMargin(false);
            }
        }
    }

    QToolBar::actionEvent(actionEvent);

    if (actionEvent->type() == QEvent::ActionAdded) {
        QWidget* const widget = widgetForAction(actionEvent->action());
        if (widget) {
            auto* const labelledToolBarWidget = qobject_cast<LabelledToolBarWidget*>(widget);
            if (labelledToolBarWidget) {
                const int index = layout()->indexOf(widget);
                if (index != -1) {
                    const bool isFirst = (index == 0);
                    labelledToolBarWidget->setHasLabelMargin(!isFirst);
                }
            }
        }
    }
 }

}

#include "moc_labelledwidgetstoolbar.cpp"
