/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewareasplitcontroller.h"

// lib
#include <kasten/abstractgroupedviews.h>
#include <kasten/viewareasplitable.h>
#include <kasten/abstractviewarea.h>
#include <kasten/viewmanager.h>
// KF5
#include <KXMLGUIClient>
#include <KActionCollection>
#include <KLocalizedString>
// Qt
#include <QAction>

namespace Kasten {

ViewAreaSplitController::ViewAreaSplitController(ViewManager* viewManager, AbstractGroupedViews* groupedViews, KXMLGUIClient* guiClient)
    : mViewManager(viewManager)
    , mGroupedViews(groupedViews)
    , mViewAreaSplitable(nullptr)
    , mCurrentViewArea(nullptr)
{
    mViewAreaSplitable = mGroupedViews ? qobject_cast<If::ViewAreaSplitable*>(mGroupedViews) : nullptr;
    if (mViewAreaSplitable) {
        connect(mGroupedViews, SIGNAL(viewAreaFocusChanged(Kasten::AbstractViewArea*)),
                SLOT(onViewAreaFocusChanged(Kasten::AbstractViewArea*)));
        connect(mGroupedViews, SIGNAL(viewAreasAdded(QList<Kasten::AbstractViewArea*>)),
                SLOT(onViewAreasChanged()));
        connect(mGroupedViews, SIGNAL(viewAreasRemoved(QList<Kasten::AbstractViewArea*>)),
                SLOT(onViewAreasChanged()));
    }

    KActionCollection* actionCollection = guiClient->actionCollection();

    mSplitVerticallyAction = new QAction(QIcon::fromTheme(QStringLiteral("view-split-left-right")),
                                         i18nc("@action:inmenu", "Split Vertically"), this);
    mSplitVerticallyAction->setObjectName(QStringLiteral("view_area_split_vertically"));
    actionCollection->setDefaultShortcut(mSplitVerticallyAction, Qt::CTRL + Qt::SHIFT + Qt::Key_L);
    mSplitVerticallyAction->setEnabled(mViewAreaSplitable != nullptr);
    connect(mSplitVerticallyAction, &QAction::triggered,
            this, &ViewAreaSplitController::splitVertically);

    mSplitHorizontallyAction = new QAction(QIcon::fromTheme(QStringLiteral("split")),
                                i18nc("@action:inmenu", "Split Horizontal"), this);
    mSplitHorizontallyAction->setObjectName(QStringLiteral("view_area_split_horizontally"));
    actionCollection->setDefaultShortcut(mSplitHorizontallyAction, Qt::CTRL + Qt::SHIFT + Qt::Key_T);
    mSplitHorizontallyAction->setEnabled(mViewAreaSplitable != nullptr);
    connect(mSplitHorizontallyAction, &QAction::triggered,
            this, &ViewAreaSplitController::splitHorizontally);

    mCloseAction = new QAction(QIcon::fromTheme(QStringLiteral("view-close")),
                                i18nc("@action:inmenu", "Close View Area"), this);
    mCloseAction->setObjectName(QStringLiteral("view_area_close"));
    actionCollection->setDefaultShortcut(mCloseAction, Qt::CTRL + Qt::SHIFT + Qt::Key_R);
    connect(mCloseAction, &QAction::triggered,
            this, &ViewAreaSplitController::close);

    actionCollection->addActions({
        mSplitVerticallyAction,
        mSplitHorizontallyAction,
        mCloseAction,
    });

    onViewAreaFocusChanged(mViewAreaSplitable ? mViewAreaSplitable->viewAreaFocus() : nullptr);
    onViewAreasChanged();
}

void ViewAreaSplitController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

void ViewAreaSplitController::splitVertically()
{
    splitViewArea(Qt::Vertical);
}

void ViewAreaSplitController::splitHorizontally()
{
    splitViewArea(Qt::Horizontal);
}

void ViewAreaSplitController::close()
{
    mViewAreaSplitable->closeViewArea(mCurrentViewArea);
}

void ViewAreaSplitController::splitViewArea(Qt::Orientation orientation)
{
    AbstractView* currentView = mCurrentViewArea->viewFocus();

    mViewAreaSplitable->splitViewArea(mCurrentViewArea, orientation);

    // TODO: ideal would be a new view which copies the existing one
    // and starts visually where the old one stops after the resize
    const Qt::Alignment alignment = (orientation == Qt::Horizontal) ? Qt::AlignBottom : Qt::AlignRight;
    mViewManager->createCopyOfView(currentView, alignment);
}

void ViewAreaSplitController::onViewAreaFocusChanged(AbstractViewArea* viewArea)
{
    if (mCurrentViewArea) {
        mCurrentViewArea->disconnect(this);
    }

    // TODO: how to handle single view areas? examples, signals?
    mCurrentViewArea = qobject_cast<AbstractGroupedViews*>(viewArea);

    if (mCurrentViewArea) {
        connect(mCurrentViewArea, &AbstractGroupedViews::added,
                this, &ViewAreaSplitController::onViewsChanged);
        connect(mCurrentViewArea, &AbstractGroupedViews::removing,
                this, &ViewAreaSplitController::onViewsChanged);
    }
    onViewsChanged();
}

void ViewAreaSplitController::onViewAreasChanged()
{
    const bool hasMultipleViewArea = mViewAreaSplitable ? (mViewAreaSplitable->viewAreasCount() > 1) : false;

    mCloseAction->setEnabled(hasMultipleViewArea);
}

void ViewAreaSplitController::onViewsChanged()
{
    const bool hasViews = mCurrentViewArea ? (mCurrentViewArea->viewCount() > 0) : false;

    mSplitVerticallyAction->setEnabled(hasViews);
    mSplitHorizontallyAction->setEnabled(hasViews);
}

}
