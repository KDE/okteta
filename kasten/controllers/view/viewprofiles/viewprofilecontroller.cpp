/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewprofilecontroller.h"

// controller
#include "viewprofileeditdialog.h"
// Okteta Gui Kasten
#include <kasten/okteta/bytearrayviewprofilemanager.h>
#include <kasten/okteta/bytearrayviewprofilesynchronizer.h>
#include <kasten/okteta/bytearrayview.h>
// KF5
#include <KXMLGUIClient>
#include <KXMLGUIFactory>
#include <KActionMenu>
#include <KActionCollection>
#include <KLocalizedString>
// Qt
#include <QAction>

namespace Kasten {

ViewProfileController::ViewProfileController(ByteArrayViewProfileManager* viewProfileManager,
                                             QWidget* parentWidget,
                                             KXMLGUIClient* guiClient)
    : AbstractXmlGuiController()
    , mGuiClient(guiClient)
    , mViewProfileManager(viewProfileManager)
    , mParentWidget(parentWidget)
{
    mViewProfileActionMenu =
        new KActionMenu(i18nc("@title:menu submenu to select the view profile or change it",
                              "View Profile"),
                        this);
    mViewProfileActionMenu->setDelayed(false);

    mCreateNewAction =
        new QAction(QIcon::fromTheme(QStringLiteral("document-new")),
                    i18nc("@action:inmenu create a new view profile",
                          "Create new..."),
                    this);
    connect(mCreateNewAction, &QAction::triggered, this, &ViewProfileController::onCreateNewActionTriggered);

    mSaveChangesAction =
        new QAction(QIcon::fromTheme(QStringLiteral("document-save")),
                    i18nc("@action:inmenu save changed to the view profile to the base profile",
                          "Save changes"),
                    this);
    connect(mSaveChangesAction, &QAction::triggered, this, &ViewProfileController::onSaveChangesActionTriggered);

    mResetChangesAction =
        new QAction(QIcon::fromTheme(QStringLiteral("document-revert")),
                    i18nc("@action:inmenu reset settings back to those of the saved base profile",
                          "Reset changes"),
                    this);
    connect(mResetChangesAction, &QAction::triggered, this, &ViewProfileController::onResetChangesActionTriggered);

    mViewProfileActionMenu->addAction(mCreateNewAction);
    mViewProfileActionMenu->addSeparator();
    mViewProfileActionMenu->addAction(mSaveChangesAction);
    mViewProfileActionMenu->addAction(mResetChangesAction);

    mViewProfilesActionGroup = new QActionGroup(this);
    mViewProfilesActionGroup->setExclusive(true);
    connect(mViewProfilesActionGroup, &QActionGroup::triggered,
            this, &ViewProfileController::onViewProfileTriggered);

    guiClient->actionCollection()->addAction(QStringLiteral("view_profile"), mViewProfileActionMenu);

    connect(mViewProfileManager, &ByteArrayViewProfileManager::viewProfilesChanged,
            this, &ViewProfileController::onViewProfilesChanged);
    connect(mViewProfileManager, &ByteArrayViewProfileManager::viewProfilesRemoved,
            this, &ViewProfileController::onViewProfilesChanged);

    onViewProfilesChanged();

    setTargetModel(nullptr);
}

void ViewProfileController::setTargetModel(AbstractModel* model)
{
    if (mByteArrayViewProfileSynchronizer) {
        mByteArrayViewProfileSynchronizer->disconnect(this);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    mByteArrayViewProfileSynchronizer = mByteArrayView ? mByteArrayView->synchronizer() : nullptr;

    const bool hasSynchronizer = (mByteArrayViewProfileSynchronizer != nullptr);
    if (hasSynchronizer) {
        onViewProfileChanged(mByteArrayViewProfileSynchronizer->viewProfileId());

        connect(mByteArrayViewProfileSynchronizer, &ByteArrayViewProfileSynchronizer::viewProfileChanged,
                this, &ViewProfileController::onViewProfileChanged);

        onLocalSyncStateChanged(mByteArrayViewProfileSynchronizer->localSyncState());

        connect(mByteArrayViewProfileSynchronizer, &ByteArrayViewProfileSynchronizer::localSyncStateChanged,
                this, &ViewProfileController::onLocalSyncStateChanged);
    } else {
        mSaveChangesAction->setEnabled(false);
        mResetChangesAction->setEnabled(false);
    }

    mCreateNewAction->setEnabled(hasSynchronizer);
    mViewProfileActionMenu->setEnabled(hasSynchronizer);
}

void ViewProfileController::onViewProfileChanged(const Kasten::ByteArrayViewProfile::Id& viewProfileId)
{
    const QList<QAction*> actions = mViewProfilesActionGroup->actions();

    for (QAction* action : actions) {
        if (action->data().toString() == viewProfileId) {
            action->setChecked(true);
            break;
        }
    }
}

void ViewProfileController::onViewProfilesChanged()
{
    qDeleteAll(mViewProfilesActionGroup->actions());

    const QList<ByteArrayViewProfile> viewProfiles = mViewProfileManager->viewProfiles();
    const ByteArrayViewProfile::Id currentViewProfileId = mByteArrayViewProfileSynchronizer ?
                                                          mByteArrayViewProfileSynchronizer->viewProfileId() :
                                                          ByteArrayViewProfile::Id();

    if (!viewProfiles.isEmpty()) {
        mViewProfileActionMenu->addSeparator();
    }

    bool isCurrentViewProfileExisting = false;
    for (const ByteArrayViewProfile& viewProfile : viewProfiles) {
        const QString title = viewProfile.viewProfileTitle();
        QAction* action = new QAction(title, mViewProfilesActionGroup);
        action->setCheckable(true);
        const ByteArrayViewProfile::Id viewProfileId = viewProfile.id();
        action->setData(viewProfileId);
        const bool isCurrentViewProfile = (viewProfileId == currentViewProfileId);
        action->setChecked(isCurrentViewProfile);
        if (isCurrentViewProfile) {
            isCurrentViewProfileExisting = true;
        }

        mViewProfilesActionGroup->addAction(action);
        mViewProfileActionMenu->addAction(action);
    }

    // reset id if no longer existing
    if (!isCurrentViewProfileExisting && mByteArrayViewProfileSynchronizer) {
        mByteArrayViewProfileSynchronizer->setViewProfileId(ByteArrayViewProfile::Id());
    }
}

void ViewProfileController::onViewProfileTriggered(QAction* action)
{
    mByteArrayViewProfileSynchronizer->setViewProfileId(action->data().toString());
}

void ViewProfileController::onLocalSyncStateChanged(Kasten::LocalSyncState localSyncState)
{
    const bool hasDifference = (localSyncState == LocalHasChanges);

    mSaveChangesAction->setEnabled(hasDifference);
    mResetChangesAction->setEnabled(hasDifference);
}

void ViewProfileController::onCreateNewActionTriggered()
{
    ViewProfileEditDialog* dialog = new ViewProfileEditDialog(mParentWidget);
    const QString dialogTitle = i18nc("@window:title",
                                      "New View Profile");
    dialog->setWindowTitle(dialogTitle);

    ByteArrayViewProfile viewProfile;
    viewProfile.setId(QString());
//     const QString modifiedTitle = i18n( "Modification of %1", newByteArrayViewProfile.viewProfileTitle() );
//     viewProfile.setViewProfileTitle( modifiedTitle );
    viewProfile.setOffsetColumnVisible(mByteArrayView->offsetColumnVisible());
    viewProfile.setVisibleByteArrayCodings(mByteArrayView->visibleByteArrayCodings());
    viewProfile.setViewModus(mByteArrayView->viewModus());
    viewProfile.setLayoutStyle(mByteArrayView->layoutStyle());
    viewProfile.setNoOfGroupedBytes(mByteArrayView->noOfGroupedBytes());
    viewProfile.setNoOfBytesPerLine(mByteArrayView->noOfBytesPerLine());
    viewProfile.setValueCoding(mByteArrayView->valueCoding());
    viewProfile.setCharCoding(mByteArrayView->charCodingName());
    viewProfile.setShowsNonprinting(mByteArrayView->showsNonprinting());
    viewProfile.setUndefinedChar(mByteArrayView->undefinedChar());
    viewProfile.setSubstituteChar(mByteArrayView->substituteChar());
    dialog->setViewProfile(viewProfile);

    const int answer = dialog->exec();

    if (answer == QDialog::Accepted) {
        QList<ByteArrayViewProfile> viewProfiles {
            dialog->viewProfile()
        };
        mViewProfileManager->saveViewProfiles(viewProfiles);

        mByteArrayViewProfileSynchronizer->setViewProfileId(viewProfiles.at(0).id());
    }

    delete dialog;
}

void ViewProfileController::onResetChangesActionTriggered()
{
    mByteArrayViewProfileSynchronizer->syncFromRemote();
}

void ViewProfileController::onSaveChangesActionTriggered()
{
    mByteArrayViewProfileSynchronizer->syncToRemote();
}

}
