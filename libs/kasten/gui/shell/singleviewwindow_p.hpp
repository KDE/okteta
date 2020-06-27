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

class QDragMoveEvent;
class QDropEvent;

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

    ~SingleViewWindowPrivate();

public:
    void init();

public:
    void setView(AbstractView* view);
    void addXmlGuiController(AbstractXmlGuiController* controller);
    void addTool(AbstractToolView* toolView);

public: // If::WidgetsDockable API
    QVector<ToolViewDockWidget*> dockWidgets() const;

private:
    AbstractView* view() const;
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

    SingleViewArea* const mViewArea;
    QVector<AbstractXmlGuiController*> mControllers;
    QVector<ToolViewDockWidget*> mDockWidgets;
    QVector<AbstractTool*> mTools;
};

inline QVector<ToolViewDockWidget*> SingleViewWindowPrivate::dockWidgets() const { return mDockWidgets; }
inline AbstractView* SingleViewWindowPrivate::view() const { return mView; }
inline SingleViewArea* SingleViewWindowPrivate::viewArea() const { return mViewArea; }

}

#endif
