/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2008,2011-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_SHELLWINDOW_P_HPP
#define KASTEN_SHELLWINDOW_P_HPP

// lib
#include "shellwindow.hpp"
// Okteta core
#include <kastencore.hpp>
// Qt
#include <QVector>

namespace Kasten {
class AbstractModelSynchronizer;
class AbstractDocument;
class AbstractTool;

class ShellWindowPrivate
{
public:
    ShellWindowPrivate(ShellWindow* parent,
                       ViewManager* viewManager);
    ~ShellWindowPrivate();

public:
    MultiViewAreas* viewArea() const;
    ViewManager* viewManager() const;

public:
    void updateControllers(AbstractView* view);
    void addXmlGuiController(AbstractXmlGuiController* controller);
    void addTool(AbstractToolView* toolView);
    void showDocument(AbstractDocument* document);

public: // If::WidgetsDockable API
    QVector<ToolViewDockWidget*> dockWidgets() const;

private: // Q_SLOTS
    void onTitleChanged(const QString& newTitle);
    void onContentFlagsChanged(Kasten::ContentFlags contentFlags);
    void onLocalSyncStateChanged(Kasten::LocalSyncState newState);
    void onViewFocusChanged(Kasten::AbstractView* view);
    void onToolVisibilityChanged(bool isVisible);
    void onSynchronizerDeleted(QObject* synchronizer);

private:
    Q_DECLARE_PUBLIC(ShellWindow)

private:
    ShellWindow* const q_ptr;

    MultiViewAreas* mGroupedViews;
    // hack:
    // used to store a pointer to the current, so we can disconnect to its signals... well, not perfect
    AbstractView* mCurrentView = nullptr;
    AbstractDocument* mCurrentDocument = nullptr;
    AbstractModelSynchronizer* mCurrentSynchronizer = nullptr;

    ViewManager* mViewManager;
    QVector<AbstractXmlGuiController*> mControllers;

    QVector<ToolViewDockWidget*> mDockWidgets;
    QVector<AbstractTool*> mTools;
};

inline MultiViewAreas* ShellWindowPrivate::viewArea() const { return mGroupedViews; }
inline ViewManager* ShellWindowPrivate::viewManager() const { return mViewManager; }
inline QVector<ToolViewDockWidget*> ShellWindowPrivate::dockWidgets() const { return mDockWidgets; }

inline void ShellWindowPrivate::addXmlGuiController(AbstractXmlGuiController* controller)
{
    mControllers.append(controller);
}

}

#endif
