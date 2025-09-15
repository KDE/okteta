/*
    SPDX-FileCopyrightText: 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TOOLVIEWDOCKWIDGET_P_HPP
#define KASTEN_TOOLVIEWDOCKWIDGET_P_HPP

// lib
#include "toolviewdockwidget.hpp"
// Std
#include <memory>

namespace Kasten {

class ToolViewDockWidgetPrivate
{
public:
    explicit ToolViewDockWidgetPrivate(std::unique_ptr<AbstractToolView>&& toolView);
    ToolViewDockWidgetPrivate(const ToolViewDockWidgetPrivate&) = delete;
    ToolViewDockWidgetPrivate(ToolViewDockWidgetPrivate&&) = delete;

    ~ToolViewDockWidgetPrivate();

    ToolViewDockWidgetPrivate& operator=(const ToolViewDockWidgetPrivate&) = delete;
    ToolViewDockWidgetPrivate& operator=(ToolViewDockWidgetPrivate&&) = delete;

public:
    void init(ToolViewDockWidget* q);

public:
    [[nodiscard]]
    AbstractToolView* toolView() const;
    [[nodiscard]]
    bool isShown() const;

private: // slots
    void onVisibilityChanged(bool isVisible);

private:
    const std::unique_ptr<AbstractToolView> mToolView;
    // TODO: find out why isVisible does not work here
    bool mIsShown = false;
};

inline AbstractToolView* ToolViewDockWidgetPrivate::toolView() const { return mToolView.get(); }
inline bool ToolViewDockWidgetPrivate::isShown() const { return mIsShown; }

}

#endif
