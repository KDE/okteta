/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009, 2011-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "shellwindow_p.hpp"

// lib
#include <multiviewareas.hpp>
#include <viewmanager.hpp>
#include <abstractview.hpp>
#include <abstracttoolview.hpp>
#include <abstractxmlguicontroller.hpp>
#include <toolviewdockwidget.hpp>
// Kasten core
#include <Kasten/AbstractTool>
#include <Kasten/AbstractModelSynchronizer>
#include <Kasten/AbstractDocument>
// Std
#include <utility>

namespace Kasten {

ShellWindowPrivate::ShellWindowPrivate(ShellWindow* parent,
                                       ViewManager* viewManager)
    : q_ptr(parent)
    , mViewManager(viewManager)
    , mGroupedViews(std::make_unique<MultiViewAreas>())
{
    parent->setCentralWidget(mGroupedViews->widget());

    QObject::connect(mViewManager, &ViewManager::opened,
                     mGroupedViews.get(), &AbstractGroupedViews::addViews);
    QObject::connect(mViewManager, &ViewManager::closing,
                     mGroupedViews.get(), &AbstractGroupedViews::removeViews);

    QObject::connect(mGroupedViews.get(), &AbstractGroupedViews::viewFocusChanged,
                     parent, [this](Kasten::AbstractView* view) { onViewFocusChanged(view); });
}

ShellWindowPrivate::~ShellWindowPrivate()
{
    // we have to explicitly reset any inline tool view before first deleting all tools
    // and then the grouped views, because on destruction of the inline tool view it
    // operates on the tool, which then has been no longer ->crash
    // The other option would be to first delete the view, but for reasons if do not
    // remember currently I prefer the destruction in this order
    // TODO: make this call unneeded
    mGroupedViews->setCurrentToolInlineView(nullptr);

    qDeleteAll(mDockWidgets);
}

void ShellWindowPrivate::addXmlGuiController(std::unique_ptr<AbstractXmlGuiController>&& controller)
{
    mControllers.emplace_back(std::move(controller));
}

void ShellWindowPrivate::addTool(std::unique_ptr<AbstractTool>&& tool, std::unique_ptr<AbstractToolView>&& toolView)
{
    Q_Q(ShellWindow);

    auto* dockWidget = new ToolViewDockWidget(std::move(toolView), q);
    // TODO: where to set the initial area?
    q->addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    // For now hide initially, to avoid triggering bug 462703
    // as currently (KF 5.116) in KXmlGuiWindow::setupGUI()
    // window size is still restored before the dock widgets' visibility state.
    // So adding all the dock widgets here to any area might
    // increase the minimal needed size beyond some stored size.
    dockWidget->hide();

    if (dockWidget->isVisible() && mCurrentView) {
        tool->setTargetModel(mCurrentView);
    }

    mTools.emplace_back(std::move(tool));
    mDockWidgets.append(dockWidget);

    QObject::connect(dockWidget, &QDockWidget::visibilityChanged,
                     q, [this](bool visible) { onToolVisibilityChanged(visible); });
}

void ShellWindowPrivate::showDocument(AbstractDocument* document)
{
    auto* currentGroupedViews = static_cast<AbstractGroupedViews*>(mGroupedViews->viewAreaFocus());
    const QVector<AbstractView*> viewList = currentGroupedViews->viewList();

    AbstractView* viewOfDocument = nullptr;
    for (AbstractView* view : viewList) {
        if (view->findBaseModel<AbstractDocument*>() == document) {
            viewOfDocument = view;
            break;
        }
    }

    if (viewOfDocument) {
        mGroupedViews->setViewFocus(viewOfDocument);
    } else {
        const QVector<Kasten::AbstractDocument*> documents { document };
        mViewManager->createViewsFor(documents);
    }
}

void ShellWindowPrivate::updateControllers(AbstractView* view)
{
    for (const auto& controller : std::as_const(mControllers)) {
        controller->setTargetModel(view);
    }

    for (ToolViewDockWidget* dockWidget : std::as_const(mDockWidgets)) {
        if (dockWidget->isShown()) {
            dockWidget->toolView()->tool()->setTargetModel(view);
        }
    }
}

void ShellWindowPrivate::onTitleChanged(const QString& newTitle)
{
    Q_Q(ShellWindow);

    const bool hasChanges =
        mCurrentSynchronizer ? (mCurrentSynchronizer->localSyncState() == LocalHasChanges) :
        mCurrentDocument ?     mCurrentDocument->contentFlags().testFlag(ContentHasUnstoredChanges) :
        false;
    q->setCaption(newTitle, hasChanges);
}

void ShellWindowPrivate::onContentFlagsChanged(ContentFlags contentFlags)
{
    Q_Q(ShellWindow);

    if (mCurrentView) {
        const bool hasChanges = (contentFlags & ContentHasUnstoredChanges);
        q->setCaption(mCurrentView->title(), hasChanges);
    }
}

void ShellWindowPrivate::onLocalSyncStateChanged(LocalSyncState newState)
{
    Q_Q(ShellWindow);

    if (mCurrentView) {
        const bool hasChanges = (newState == LocalHasChanges);
        q->setCaption(mCurrentView->title(), hasChanges);
    }
}

void ShellWindowPrivate::onViewFocusChanged(AbstractView* view)
{
    Q_Q(ShellWindow);

    if (mCurrentView) {
        mCurrentView->disconnect(q);
    }

    mCurrentView = view;

    updateControllers(view);

    AbstractDocument* oldDocument = mCurrentDocument;
    mCurrentDocument = view ? view->findBaseModel<AbstractDocument*>() : nullptr;
    const bool isNewDocument = (mCurrentDocument != oldDocument);

    AbstractModelSynchronizer* oldSynchronizer = mCurrentSynchronizer;
    mCurrentSynchronizer = mCurrentDocument ? mCurrentDocument->synchronizer() : nullptr;
    const bool isNewSynchronizer = (mCurrentSynchronizer != oldSynchronizer);

    if (oldSynchronizer) {
        if (isNewSynchronizer) {
            oldSynchronizer->disconnect(q);
        }
    } else {
        if (oldDocument && isNewDocument) {
            oldDocument->disconnect(q);
        }
    }

    const QString title = view ? view->title() : QString();
    const bool hasChanges =
        mCurrentSynchronizer ? (mCurrentSynchronizer->localSyncState() == LocalHasChanges) :
        mCurrentDocument ?     mCurrentDocument->contentFlags().testFlag(ContentHasUnstoredChanges) :
        false;
    q->setCaption(title, hasChanges);

    if (view) {
        QObject::connect(view, &AbstractModel::titleChanged,
                         q, [this](const QString& Title) { onTitleChanged(Title); });
    }

    if (mCurrentSynchronizer) {
        if (isNewSynchronizer) {
            QObject::connect(mCurrentSynchronizer, &AbstractModelSynchronizer::localSyncStateChanged,
                             q, [this](LocalSyncState localSyncState) { onLocalSyncStateChanged(localSyncState); });
            QObject::connect(mCurrentSynchronizer, &QObject::destroyed,
                             q, [this](QObject* object) { onSynchronizerDeleted(object); });
        }
    } else if (mCurrentDocument) {
        if (isNewDocument) {
            QObject::connect(mCurrentDocument, &AbstractDocument::contentFlagsChanged,
                             q, [this](ContentFlags contentFlags) { onContentFlagsChanged(contentFlags); });
        }
    }
}

void ShellWindowPrivate::onToolVisibilityChanged(bool isVisible)
{
    Q_Q(ShellWindow);

    auto* dockWidget = qobject_cast<ToolViewDockWidget*>(q->sender());
    if (dockWidget) {
        AbstractView* view = isVisible ? mCurrentView : nullptr;
        dockWidget->toolView()->tool()->setTargetModel(view);
    }
}

void ShellWindowPrivate::onSynchronizerDeleted(QObject* synchronizer)
{
    Q_Q(ShellWindow);

    if (synchronizer != mCurrentSynchronizer) {
        return;
    }

    mCurrentSynchronizer = nullptr;

    // switch to document state
    QObject::connect(mCurrentDocument, &AbstractDocument::contentFlagsChanged,
                     q, [this](ContentFlags contentFlags) { onContentFlagsChanged(contentFlags); });

    onContentFlagsChanged(mCurrentDocument->contentFlags());
}

}
