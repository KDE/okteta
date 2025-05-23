/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SINGLEVIEWWINDOW_P_HPP
#define KASTEN_SINGLEVIEWWINDOW_P_HPP

// lib
#include "singleviewwindow.hpp"
// Kasten core
#include <Kasten/KastenCore>
// Qt
#include <QVector>
// Std
#include <vector>
#include <memory>

namespace Kasten {

class SingleViewArea;
class AbstractTool;
class AbstractModelSynchronizer;
class AbstractDocument;

class SingleViewWindowPrivate
{
public:
    SingleViewWindowPrivate(SingleViewWindow* parent,
                            AbstractView* view);
    SingleViewWindowPrivate(const SingleViewWindowPrivate&) = delete;
    SingleViewWindowPrivate(SingleViewWindowPrivate&&) = delete;

    ~SingleViewWindowPrivate();

    SingleViewWindowPrivate& operator=(const SingleViewWindowPrivate&) = delete;
    SingleViewWindowPrivate& operator=(SingleViewWindowPrivate&&) = delete;

public:
    void init();

public:
    void setView(AbstractView* view);
    void addXmlGuiController(std::unique_ptr<AbstractXmlGuiController>&& controller);
    void addTool(std::unique_ptr<AbstractTool>&& tool, std::unique_ptr<AbstractToolView>&& toolView);

public: // If::WidgetsDockable API
    [[nodiscard]]
    QVector<ToolViewDockWidget*> dockWidgets() const;

private:
    [[nodiscard]]
    AbstractView* view() const;
    [[nodiscard]]
    SingleViewArea* viewArea() const;

private: // Q_SLOTS
    void onTitleChanged(const QString& newTitle);
    void onContentFlagsChanged(Kasten::ContentFlags contentFlags);
    void onLocalSyncStateChanged(Kasten::LocalSyncState newState);
    void onToolVisibilityChanged(bool isVisible);
    void onSynchronizerDeleted(QObject* synchronizer);

private:
    Q_DECLARE_PUBLIC(SingleViewWindow)

private:
    SingleViewWindow* const q_ptr;

    AbstractView* mView = nullptr;
    AbstractDocument* mDocument = nullptr;
    AbstractModelSynchronizer* mSynchronizer = nullptr;

    const std::unique_ptr<SingleViewArea> mViewArea;
    std::vector<std::unique_ptr<AbstractXmlGuiController>> mControllers;
    QVector<ToolViewDockWidget*> mDockWidgets;
    std::vector<std::unique_ptr<AbstractTool>> mTools;
};

inline QVector<ToolViewDockWidget*> SingleViewWindowPrivate::dockWidgets() const { return mDockWidgets; }
inline AbstractView* SingleViewWindowPrivate::view() const { return mView; }
inline SingleViewArea* SingleViewWindowPrivate::viewArea() const { return mViewArea.get(); }

}

#endif
