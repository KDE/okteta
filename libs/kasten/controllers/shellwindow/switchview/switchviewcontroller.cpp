/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "switchviewcontroller.hpp"

// Kasten gui
#include <Kasten/AbstractGroupedViews>
#include <Kasten/AbstractView>
// KF
#include <KXMLGUIClient>
#include <KActionCollection>
// Qt
#include <QActionGroup>

namespace Kasten {

SwitchViewController::SwitchViewController(AbstractGroupedViews* groupedViews, KXMLGUIClient* guiClient)
    : mGroupedViews(groupedViews)
{
    mForwardAction = KStandardAction::forward(this, &SwitchViewController::forward, this);

    mBackwardAction = KStandardAction::back(this, &SwitchViewController::backward, this);

    KActionCollection* actionCollection = guiClient->actionCollection();
    actionCollection->addAction(QStringLiteral("window_next"), mForwardAction);
    actionCollection->addAction(QStringLiteral("window_previous"), mBackwardAction);

    connect(groupedViews, &AbstractGroupedViews::added,
            this, &SwitchViewController::updateActions);
    connect(groupedViews, &AbstractGroupedViews::removing,
            this, &SwitchViewController::updateActions);
    connect(groupedViews, &AbstractGroupedViews::viewFocusChanged,
            this, &SwitchViewController::updateActions);

    updateActions();
}

void SwitchViewController::setTargetModel(AbstractModel* model)
{
    Q_UNUSED(model)
}

// TODO: think about moving this properties/abilities (hasNext/Previous,switchToNext/Previous) into a interface for the groupedview
void SwitchViewController::updateActions()
{
    bool hasNext;
    bool hasPrevious;
    const QVector<AbstractView*> viewList = mGroupedViews->viewList();
    if (viewList.isEmpty()) {
        hasNext = false;
        hasPrevious = false;
    } else {
        AbstractView* focussedView = mGroupedViews->viewFocus();
        const int indexOfFocussedView = viewList.indexOf(focussedView);

        hasNext = (indexOfFocussedView + 1 < viewList.count());
        hasPrevious = (indexOfFocussedView > 0);
    }

    mForwardAction->setEnabled(hasNext);
    mBackwardAction->setEnabled(hasPrevious);
}

void SwitchViewController::forward()
{
    const QVector<AbstractView*> viewList = mGroupedViews->viewList();
    AbstractView* focussedView = mGroupedViews->viewFocus();
    const int indexOfFocussedView = viewList.indexOf(focussedView);
    AbstractView* nextView = viewList.at(indexOfFocussedView + 1);
    mGroupedViews->setViewFocus(nextView);
}

void SwitchViewController::backward()
{
    const QVector<AbstractView*> viewList = mGroupedViews->viewList();
    AbstractView* focussedView = mGroupedViews->viewFocus();
    const int indexOfFocussedView = viewList.indexOf(focussedView);
    AbstractView* previousView = viewList.at(indexOfFocussedView - 1);
    mGroupedViews->setViewFocus(previousView);
}

}
