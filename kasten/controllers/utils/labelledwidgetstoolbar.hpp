/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_LABELLEDWIDGETTOOLBAR_HPP
#define KASTEN_LABELLEDWIDGETTOOLBAR_HPP

// Qt
#include <QToolBar>

namespace Kasten {

// TODO: ideally not needed class
// When to have a margin with unframed labels and when not?
// Should LabelledToolBarWidget instead have the logic to check its position
// and need to have a margin? Can it query the state changes to properly update?
class LabelledWidgetsToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit LabelledWidgetsToolBar(QWidget* parent);

protected:
    void actionEvent(QActionEvent* actionEvent) override;
};

}

#endif
