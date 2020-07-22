/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewlistmenucontroller.hpp"

// Kasten gui
#include <Kasten/AbstractGroupedViews>
#include <Kasten/ViewManager>
// Kasten core
#include <Kasten/AbstractDocument>
// KF
#include <KXMLGUIClient>
#include <KXMLGUIFactory>
#include <KActionCollection>
#include <KLocalizedString>
#include <KStringHandler>
// Qt
#include <QActionGroup>

namespace Kasten {

static constexpr int MaxEntryLength = 150;
static constexpr char WindowsListActionListId[] = "windows_list";

ViewListMenuController::ViewListMenuController(ViewManager* viewManager, AbstractGroupedViews* groupedViews,
                                               KXMLGUIClient* guiClient)
    : mViewManager(viewManager)
    , mGroupedViews(groupedViews)
    , mGuiClient(guiClient)
{
    mWindowsActionGroup = new QActionGroup(this);   // TODO: do we use this only for the signal mapping?
//     mWindowsActionGroup->setExclusive( true );
    connect(mWindowsActionGroup, &QActionGroup::triggered,
            this, &ViewListMenuController::onActionTriggered);

    connect(mViewManager, &ViewManager::opened,
            this, &ViewListMenuController::updateActions);
    connect(mViewManager, &ViewManager::closing,
            this, &ViewListMenuController::updateActions);

    updateActions();
}

void ViewListMenuController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

void ViewListMenuController::updateActions()
{
    mGuiClient->unplugActionList(QLatin1String(WindowsListActionListId));

    qDeleteAll(mWindowsActionGroup->actions());

    const QVector<AbstractView*> views = mViewManager->views();
    const bool hasViews = (!views.isEmpty());

    if (hasViews) {
        // TODO: sortieren nach namen und erste 10 mit Zahl, siehe unten
        for (int v = 0; v < views.size(); ++v) {
            AbstractView* view = views.at(v);
            const QString title = KStringHandler::rsqueeze(view->title(), MaxEntryLength);
            QAction* action = new QAction(v < 9 ? QStringLiteral("&%1 %2").arg(v + 1).arg(title) : title, mWindowsActionGroup);
            //         action->setCheckable( true );

            //         if(m_viewManager->activeView() && doc == m_viewManager->activeView()->document())
            //             action->setChecked(true);
            action->setData(QVariant::fromValue(view));
            mWindowsActionGroup->addAction(action);
        }
    } else {
        QAction* noneAction = new QAction(i18nc("@item There are no windows.", "None."), mWindowsActionGroup);
        mWindowsActionGroup->addAction(noneAction);
    }
    mWindowsActionGroup->setEnabled(hasViews);

    mGuiClient->plugActionList(QLatin1String(WindowsListActionListId), mWindowsActionGroup->actions());
}

void ViewListMenuController::onActionTriggered(QAction* action)
{
    auto* view = action->data().value<AbstractView*>();
    mGroupedViews->setViewFocus(view);
}

}
