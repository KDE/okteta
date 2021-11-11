/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewareacontextmenucontroller.hpp"

// Kasten gui
#include <Kasten/MultiViewAreas>
#include <Kasten/AbstractView>
// Kasten core
#include <Kasten/DocumentSyncManager>
#include <Kasten/AbstractModelFileSystemSynchronizer>
// KF
#include <KXMLGUIClient>
#include <KXMLGUIFactory>
#include <KActionCollection>
#include <KStandardAction>
#include <KLocalizedString>
// Qt
#include <QMenu>

namespace Kasten {

ViewAreaContextMenuController::ViewAreaContextMenuController(MultiViewAreas* multiViewAreas,
                                                             DocumentSyncManager* syncManager,
                                                             KXMLGUIClient* guiClient)
    : mGuiClient(guiClient)
    , mMultiViewAreas(multiViewAreas)
    , mSyncManager(syncManager)
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mReloadAction = new QAction(QIcon::fromTheme(QStringLiteral("view-refresh")),
                                i18nc("@action:inmenu", "Reloa&d"), this);
    mReloadAction->setObjectName(QStringLiteral("viewarea_reload"));
    connect(mReloadAction, &QAction::triggered,
            this, &ViewAreaContextMenuController::reload);
    mReloadAction->setEnabled(false);

    actionCollection->addAction(mReloadAction->objectName(), mReloadAction);

    const QIcon documentCloseIcon = QIcon::fromTheme(QStringLiteral("document-close"));
    mCloseAction = KStandardAction::close(this, &ViewAreaContextMenuController::close, this);
    mCloseAction->setObjectName(QStringLiteral("viewarea_close"));
    mCloseAction->setProperty("defaultShortcuts", QVariant());
    mCloseAction->setShortcuts({});
    mCloseAction->setEnabled(false);

    actionCollection->addAction(mCloseAction->objectName(), mCloseAction);

    mCloseAllAction = new QAction(documentCloseIcon,
                                    i18nc("@action:inmenu", "Close All"), this);
    mCloseAllAction->setObjectName(QStringLiteral("viewarea_close_all"));
    connect(mCloseAllAction, &QAction::triggered,
            this, &ViewAreaContextMenuController::closeAll);
    mCloseAllAction->setEnabled(false);

    mCloseAllOtherAction = new QAction(documentCloseIcon,
                                        i18nc("@action:inmenu", "Close All Other"), this);
    mCloseAllOtherAction->setObjectName(QStringLiteral("viewarea_close_all_other"));
    connect(mCloseAllOtherAction, &QAction::triggered,
            this, &ViewAreaContextMenuController::closeAllOther);
    mCloseAllOtherAction->setEnabled(false);

    actionCollection->addAction(mCloseAllAction->objectName(), mCloseAllAction);
    actionCollection->addAction(mCloseAllOtherAction->objectName(), mCloseAllOtherAction);

    connect(mMultiViewAreas, &MultiViewAreas::contextMenuRequested,
            this, &ViewAreaContextMenuController::showContextMenu);
}

void ViewAreaContextMenuController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

void ViewAreaContextMenuController::showContextMenu(AbstractGroupedViews* viewArea, AbstractView* view, QPoint pos)
{
    mViewArea = viewArea;
    mView = view;
    mDocument = mView ? mView->findBaseModel<AbstractDocument*>() : nullptr;
    auto* synchronizer = mDocument ? qobject_cast<AbstractModelFileSystemSynchronizer*>(mDocument->synchronizer()) : nullptr;

    const bool isForView = (mView != nullptr);
    const auto views = mViewArea->viewList();
    bool canSync = false;
    if (synchronizer) {
        const LocalSyncState localSyncState = synchronizer->localSyncState();
        const RemoteSyncState remoteSyncState = synchronizer->remoteSyncState();
        canSync = (localSyncState == LocalHasChanges)
                  || (remoteSyncState == RemoteHasChanges)
                  || (remoteSyncState == RemoteUnknown);
    }
    mReloadAction->setEnabled(canSync);

    mCloseAction->setEnabled(isForView);
    mCloseAllOtherAction->setEnabled(isForView && views.size() > 1);
    mCloseAllAction->setEnabled(!views.isEmpty());

    QWidget* w = mGuiClient->factory()->container(QStringLiteral("viewAreaContextMenu"), mGuiClient);
    auto* popup = static_cast<QMenu*>(w);

    const auto popupPoint = mMultiViewAreas->widget()->mapToGlobal(pos);

    popup->popup(popupPoint);
}

void ViewAreaContextMenuController::reload()
{
    mSyncManager->reload(mDocument);
}

void ViewAreaContextMenuController::close()
{
    emit mMultiViewAreas->closeRequest({mView});
}

void ViewAreaContextMenuController::closeAll()
{
    emit mMultiViewAreas->closeRequest(mViewArea->viewList());
}

void ViewAreaContextMenuController::closeAllOther()
{
    auto views = mViewArea->viewList();
    views.removeOne(mView);
    emit mMultiViewAreas->closeRequest(views);
}

}
