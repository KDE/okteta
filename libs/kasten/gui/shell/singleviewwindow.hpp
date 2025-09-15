/*
    SPDX-FileCopyrightText: 2011-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINGLEVIEWWINDOW_HPP
#define KASTEN_SINGLEVIEWWINDOW_HPP

// lib
#include "kastengui_export.hpp"
#include "widgetsdockable.hpp"
// KF
#include <KXmlGuiWindow>
// Std
#include <memory>

namespace Kasten {

class SingleViewWindowPrivate;

class AbstractView;
class AbstractTool;
class AbstractToolView;
class AbstractXmlGuiController;
class SingleViewArea;

class KASTENGUI_EXPORT SingleViewWindow : public KXmlGuiWindow
                                        , public If::WidgetsDockable
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::WidgetsDockable
    )

public:
    explicit SingleViewWindow(AbstractView* view);
    ~SingleViewWindow() override;

public:
    void addXmlGuiController(std::unique_ptr<AbstractXmlGuiController>&& controller);
    void addTool(std::unique_ptr<AbstractTool>&& tool, std::unique_ptr<AbstractToolView>&& toolView);

    void setView(AbstractView* view);

public: // If::WidgetsDockable API
    [[nodiscard]]
    QVector<ToolViewDockWidget*> dockWidgets() const override;

protected:
    [[nodiscard]]
    AbstractView* view() const;
    [[nodiscard]]
    SingleViewArea* viewArea() const;

protected:
    const std::unique_ptr<SingleViewWindowPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(SingleViewWindow)
};

}

#endif
