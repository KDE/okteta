/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TOOLVIEWDOCKWIDGET_P_HPP
#define KASTEN_TOOLVIEWDOCKWIDGET_P_HPP

// lib
#include "toolviewdockwidget.hpp"

namespace Kasten {

class ToolViewDockWidgetPrivate
{
public:
    explicit ToolViewDockWidgetPrivate(AbstractToolView* mToolView);
    ~ToolViewDockWidgetPrivate();

public:
    void init(ToolViewDockWidget* q);

public:
    AbstractToolView* toolView() const;
    bool isShown() const;

private: // slots
    void onVisibilityChanged(bool isVisible);

private:
    AbstractToolView* const mToolView;
    // TODO: find out why isVisible does not work here
    bool mIsShown = false;
};

inline AbstractToolView* ToolViewDockWidgetPrivate::toolView() const { return mToolView; }
inline bool ToolViewDockWidgetPrivate::isShown() const { return mIsShown; }

}

#endif
