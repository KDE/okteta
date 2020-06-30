/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "versioncontroller.hpp"

// Kasten core
#include <Kasten/DocumentVersionData>
#include <Kasten/Versionable>
#include <Kasten/AbstractModel>
// KF
#include <KXMLGUIClient>
#include <KLocalizedString>
#include <KActionCollection>
#include <KToolBarPopupAction>
#include <KStandardShortcut>
// Qt
#include <QMenu>

namespace Kasten {

static constexpr int MaxMenuEntries = 10;

VersionController::VersionController(KXMLGUIClient* guiClient)
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mSetToOlderVersionAction = new KToolBarPopupAction(QIcon::fromTheme(QStringLiteral("edit-undo")), i18nc("@action:inmenu", "Undo"), this);
    actionCollection->addAction(QStringLiteral("edit_undo"), mSetToOlderVersionAction);
    actionCollection->setDefaultShortcuts(mSetToOlderVersionAction, KStandardShortcut::undo());

    connect(mSetToOlderVersionAction, &QAction::triggered,
            this, &VersionController::onSetToOlderVersionTriggered);
    connect(mSetToOlderVersionAction->menu(), &QMenu::aboutToShow,
            this, &VersionController::onOlderVersionMenuAboutToShow);
    connect(mSetToOlderVersionAction->menu(), &QMenu::triggered,
            this, &VersionController::onOlderVersionMenuTriggered);

    mSetToNewerVersionAction = new KToolBarPopupAction(QIcon::fromTheme(QStringLiteral("edit-redo")), i18nc("@action:inmenu", "Redo"), this);
    actionCollection->addAction(QStringLiteral("edit_redo"), mSetToNewerVersionAction);
    actionCollection->setDefaultShortcuts(mSetToNewerVersionAction, KStandardShortcut::redo());

    connect(mSetToNewerVersionAction, &QAction::triggered,
            this, &VersionController::onSetToNewerVersionTriggered);
    connect(mSetToNewerVersionAction->menu(), &QMenu::aboutToShow,
            this, &VersionController::onNewerVersionMenuAboutToShow);
    connect(mSetToNewerVersionAction->menu(), &QMenu::triggered,
            this, &VersionController::onNewerVersionMenuTriggered);

    setTargetModel(nullptr);
}

void VersionController::setTargetModel(AbstractModel* model)
{
    if (mModel) {
        mModel->disconnect(this);
        AbstractModel* versionedModel = mModel->findBaseModelWithInterface<If::Versionable*>();
        if (versionedModel) {
            versionedModel->disconnect(this);
        }
    }

    mModel = model;
    AbstractModel* versionedModel = mModel ? mModel->findBaseModelWithInterface<If::Versionable*>() : nullptr;
    mVersionControl = versionedModel ? qobject_cast<If::Versionable*>(versionedModel) : nullptr;

    if (mVersionControl) {
        connect(versionedModel, SIGNAL(revertedToVersionIndex(int)), SLOT(onVersionIndexChanged(int)));
        connect(versionedModel, SIGNAL(headVersionChanged(int)),     SLOT(onVersionIndexChanged(int)));
        connect(mModel, &AbstractModel::readOnlyChanged,
                this, &VersionController::onReadOnlyChanged);
    } else {
        mModel = nullptr;
    }

    const bool isVersionable = (mVersionControl && !mModel->isReadOnly());

    if (isVersionable) {
        onVersionIndexChanged(mVersionControl->versionIndex());
    } else {
        mSetToOlderVersionAction->setEnabled(false);
        mSetToNewerVersionAction->setEnabled(false);
    }
}

void VersionController::onVersionIndexChanged(int versionIndex)
{
    const bool hasOlderVersions = (versionIndex > 0);
    mSetToOlderVersionAction->setEnabled(hasOlderVersions);
    if (hasOlderVersions) {
        mSetToOlderVersionAction->setData(versionIndex - 1);
    }

    const bool hasNewerVersions = (versionIndex < (mVersionControl->versionCount() - 1));
    mSetToNewerVersionAction->setEnabled(hasNewerVersions);
    if (hasNewerVersions) {
        mSetToNewerVersionAction->setData(versionIndex + 1);
    }
}

void VersionController::onSetToOlderVersionTriggered()
{
    const int versionIndex = mSetToOlderVersionAction->data().toInt();
    mVersionControl->revertToVersionByIndex(versionIndex);
}

void VersionController::onSetToNewerVersionTriggered()
{
    const int versionIndex = mSetToNewerVersionAction->data().toInt();
    mVersionControl->revertToVersionByIndex(versionIndex);
}

void VersionController::onOlderVersionMenuAboutToShow()
{
    QMenu* menu = mSetToOlderVersionAction->menu();
    menu->clear();

    int menuEntries = 0;
    for (int versionIndex = mVersionControl->versionIndex();
         versionIndex > 0 && menuEntries < MaxMenuEntries;
         --versionIndex, ++menuEntries) {
        DocumentVersionData versionData = mVersionControl->versionData(versionIndex);

        const QString changeComment = versionData.changeComment();

        const QString actionText = i18nc("@action Undo: [change]", "Undo: %1", changeComment);

        QAction* action = menu->addAction(actionText);
        action->setData(versionIndex - 1);
    }
}

void VersionController::onNewerVersionMenuAboutToShow()
{
    QMenu* menu = mSetToNewerVersionAction->menu();
    menu->clear();

    int menuEntries = 0;
    for (int versionIndex = mVersionControl->versionIndex() + 1;
         versionIndex < mVersionControl->versionCount() && menuEntries < MaxMenuEntries;
         ++versionIndex, ++menuEntries) {
        DocumentVersionData versionData = mVersionControl->versionData(versionIndex);

        const QString changeComment = versionData.changeComment();

        const QString actionText = i18nc("@action Redo: [change]", "Redo: %1", changeComment);

        QAction* action = menu->addAction(actionText);
        action->setData(versionIndex);
    }
}

void VersionController::onOlderVersionMenuTriggered(QAction* action)
{
    const int versionIndex = action->data().toInt();
    mVersionControl->revertToVersionByIndex(versionIndex);
}

void VersionController::onNewerVersionMenuTriggered(QAction* action)
{
    const int versionIndex = action->data().toInt();
    mVersionControl->revertToVersionByIndex(versionIndex);
}

void VersionController::onReadOnlyChanged(bool isReadOnly)
{
    if (isReadOnly) {
        mSetToOlderVersionAction->setEnabled(false);
        mSetToNewerVersionAction->setEnabled(false);
    } else {
        onVersionIndexChanged(mVersionControl->versionIndex());
    }
}

}
