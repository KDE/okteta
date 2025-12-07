/*
    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewprofilecontroller.hpp"

// controller
#include "viewprofileeditdialog.hpp"
// Okteta Gui Kasten
#include <Kasten/Okteta/ByteArrayViewProfileManager>
#include <Kasten/Okteta/ByteArrayViewProfileSynchronizer>
#include <Kasten/Okteta/ByteArrayView>
// KF
#include <KXMLGUIClient>
#include <KActionMenu>
#include <KActionCollection>
#include <KLocalizedString>
// Qt
#include <QActionGroup>
#include <QAction>

namespace Kasten {

ViewProfileController::ViewProfileController(ByteArrayViewProfileManager* viewProfileManager,
                                             QWidget* parentWidget,
                                             KXMLGUIClient* guiClient)
    : mViewProfileManager(viewProfileManager)
    , mParentWidget(parentWidget)
{
    mViewProfileActionMenu =
        new KActionMenu(i18nc("@title:menu submenu to select the view profile or change it",
                              "View Profile"),
                        this);
    mViewProfileActionMenu->setPopupMode(QToolButton::InstantPopup);
    mViewProfileActionMenu->setEnabled(false);

    mCreateNewAction =
        new QAction(QIcon::fromTheme(QStringLiteral("document-new")),
                    i18nc("@action:inmenu create a new view profile",
                          "Create New…"),
                    this);
    connect(mCreateNewAction, &QAction::triggered, this, &ViewProfileController::onCreateNewActionTriggered);
    mCreateNewAction->setEnabled(false);

    mSaveChangesAction =
        new QAction(QIcon::fromTheme(QStringLiteral("document-save")),
                    i18nc("@action:inmenu save changed to the view profile to the base profile",
                          "Save Changes"),
                    this);
    connect(mSaveChangesAction, &QAction::triggered, this, &ViewProfileController::onSaveChangesActionTriggered);
    mSaveChangesAction->setEnabled(false);

    mResetChangesAction =
        new QAction(QIcon::fromTheme(QStringLiteral("document-revert")),
                    i18nc("@action:inmenu reset settings back to those of the saved base profile",
                          "Reset Changes"),
                    this);
    connect(mResetChangesAction, &QAction::triggered, this, &ViewProfileController::onResetChangesActionTriggered);
    mResetChangesAction->setEnabled(false);

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
}

void ViewProfileController::setTargetModel(AbstractModel* model)
{
    ByteArrayView* const  byteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;
    if (byteArrayView == mByteArrayView) {
        return;
    }

    if (mByteArrayViewProfileSynchronizer) {
        mByteArrayViewProfileSynchronizer->disconnect(this);
    }

    mByteArrayView = byteArrayView;
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
        auto* const action = new QAction(title, mViewProfilesActionGroup);
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
    auto* const dialog = new ViewProfileEditDialog(mParentWidget);
    const QString dialogTitle = i18nc("@title:window",
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
    connect(dialog, &ViewProfileEditDialog::viewProfileAccepted,
            this, &ViewProfileController::addNewViewProfile);

    dialog->open();
}

void ViewProfileController::addNewViewProfile(const ByteArrayViewProfile& viewProfile)
{
    QList<ByteArrayViewProfile> viewProfiles {
        viewProfile
    };
    mViewProfileManager->saveViewProfiles(viewProfiles);

    mByteArrayViewProfileSynchronizer->setViewProfileId(viewProfiles.at(0).id());
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

#include "moc_viewprofilecontroller.cpp"
