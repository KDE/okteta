/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2008,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KASTEN_SHELLWINDOW_HPP
#define KASTEN_SHELLWINDOW_HPP

// Kasten gui
#include <kasten/kastengui_export.hpp>
#include <kasten/widgetsdockable.hpp>
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
