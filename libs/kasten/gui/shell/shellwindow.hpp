/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SHELLWINDOW_HPP
#define KASTEN_SHELLWINDOW_HPP

// Kasten gui
#include "kastengui_export.hpp"
#include "widgetsdockable.hpp"
// KF
#include <KXmlGuiWindow>

namespace Kasten {
class ShellWindowPrivate;

class ViewManager;
class MultiViewAreas;
class AbstractXmlGuiController;
class AbstractToolView;
class AbstractView;
class AbstractDocument;

class KASTENGUI_EXPORT ShellWindow : public KXmlGuiWindow
                                   , public If::WidgetsDockable
{
    Q_OBJECT
    Q_INTERFACES(
        Kasten::If::WidgetsDockable
    )

public:
    explicit ShellWindow(ViewManager* viewManager);
    ~ShellWindow() override;

public Q_SLOTS:
    void showDocument(Kasten::AbstractDocument* document);   // TODO: better name

public:
    void updateControllers(AbstractView* view);
    void addXmlGuiController(AbstractXmlGuiController* controller);
    void addTool(AbstractToolView* toolView);

public: // If::WidgetsDockable API
    QVector<ToolViewDockWidget*> dockWidgets() const override;

protected:
    MultiViewAreas* viewArea() const;
    ViewManager* viewManager() const;

protected:
    const QScopedPointer<ShellWindowPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(ShellWindow)
};

}

#endif
