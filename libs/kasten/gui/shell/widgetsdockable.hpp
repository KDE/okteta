/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_WIDGETSDOCKABLE_HPP
#define KASTEN_IF_WIDGETSDOCKABLE_HPP

// Qt
#include <QtPlugin>

template <typename C> class QVector;

namespace Kasten {

class ToolViewDockWidget;

namespace If {

class WidgetsDockable
{
public:
    virtual ~WidgetsDockable();

public: // set/action
//     virtual void setViewFocus( AbstractView *view ) = 0;

public: // get
    virtual QVector<ToolViewDockWidget*> dockWidgets() const = 0;

public: // signal
//     virtual void viewFocusChanged( AbstractView *view ) = 0;
};

inline WidgetsDockable::~WidgetsDockable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::WidgetsDockable, "org.kde.kasten.if.widgetsdockable/1.0")

#endif
