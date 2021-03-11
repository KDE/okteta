/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TOOLVIEWDOCKWIDGET_HPP
#define KASTEN_TOOLVIEWDOCKWIDGET_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QDockWidget>

namespace Kasten {

class AbstractToolView;

class ToolViewDockWidgetPrivate;

class KASTENGUI_EXPORT ToolViewDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    ToolViewDockWidget(AbstractToolView* mToolView, QWidget* parent);
    ~ToolViewDockWidget() override;

public:
    AbstractToolView* toolView() const;
    bool isShown() const;

private:
    const QScopedPointer<class ToolViewDockWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ToolViewDockWidget)
};

}

#endif
