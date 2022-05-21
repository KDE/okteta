/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008, 2011-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SHELLWINDOW_P_HPP
#define KASTEN_SHELLWINDOW_P_HPP

// lib
#include "shellwindow.hpp"
// Okteta core
#include <kastencore.hpp>
// Qt
#include <QList>

namespace Kasten {
class AbstractModelSynchronizer;
class AbstractDocument;
class AbstractTool;

class ShellWindowPrivate
{
public:
    ShellWindowPrivate(ShellWindow* parent,
                       ViewManager* viewManager);
    ShellWindowPrivate(const ShellWindowPrivate&) = delete;

    ~ShellWindowPrivate();

    ShellWindowPrivate& operator=(const ShellWindowPrivate&) = delete;

public:
    MultiViewAreas* viewArea() const;
    ViewManager* viewManager() const;

public:
    void updateControllers(AbstractView* view);
    void addXmlGuiController(AbstractXmlGuiController* controller);
    void addTool(AbstractToolView* toolView);
    void showDocument(AbstractDocument* document);

public: // If::WidgetsDockable API
    QList<ToolViewDockWidget*> dockWidgets() const;

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
    QList<AbstractXmlGuiController*> mControllers;

    QList<ToolViewDockWidget*> mDockWidgets;
    QList<AbstractTool*> mTools;
};

inline MultiViewAreas* ShellWindowPrivate::viewArea() const { return mGroupedViews; }
inline ViewManager* ShellWindowPrivate::viewManager() const { return mViewManager; }
inline QList<ToolViewDockWidget*> ShellWindowPrivate::dockWidgets() const { return mDockWidgets; }

inline void ShellWindowPrivate::addXmlGuiController(AbstractXmlGuiController* controller)
{
    mControllers.append(controller);
}

}

#endif
