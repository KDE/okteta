/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012-2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewprofilesmanagedialog.hpp"

// this
#include "viewprofileeditdialog.hpp"
#include "viewprofiletablemodel.hpp"
// Okteta Gui Kasten
#include <Kasten/Okteta/ByteArrayViewProfileManager>
#include <Kasten/Okteta/ByteArrayViewProfileLock>
// KF
#include <KLocalizedString>
#include <KGuiItem>
// Qt
#include <QPushButton>
#include <QHBoxLayout>
#include <QTreeView>
#include <QDialogButtonBox>
#include <QItemSelectionModel>

namespace Kasten {

ViewProfilesManageDialog::ViewProfilesManageDialog(ByteArrayViewProfileManager* viewProfileManager,
                                                   QWidget* parent)
    : QDialog(parent)
    , mViewProfileManager(viewProfileManager)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    setWindowTitle(i18nc("@title:window", "View Profiles"));

    auto* pageLayout = new QHBoxLayout;

    // profile list
    mViewProfileTableView = new QTreeView;
    mViewProfileTableModel = new ViewProfileTableModel(mViewProfileManager, this);
    mViewProfileTableView->setObjectName(QStringLiteral("ViewProfileTableView"));
    mViewProfileTableView->setHeaderHidden(true);
    mViewProfileTableView->setRootIsDecorated(false);
    mViewProfileTableView->setItemsExpandable(false);
    mViewProfileTableView->setUniformRowHeights(true);
    mViewProfileTableView->setAllColumnsShowFocus(true);
    mViewProfileTableView->setModel(mViewProfileTableModel);
    mViewProfileTableView->resizeColumnToContents(ViewProfileTableModel::CurrentColumnId);
    connect(mViewProfileTableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &ViewProfilesManageDialog::onViewProfileSelectionChanged);
    connect(mViewProfileTableModel, &ViewProfileTableModel::modelReset,
            this, &ViewProfilesManageDialog::onModelReset);

    // buttons
    auto* buttonLayout = new QVBoxLayout;
    auto* createButton = // copy from selected
                         new QPushButton;
    KGuiItem::assign(createButton,
                     KGuiItem(i18nc("@action:button",
                                    "&Create new..."),
                              QStringLiteral("document-new"),
                              i18nc("@info:tooltip",
                                    "Opens an editor for a new view profile."),
                              xi18nc("@info:whatsthis",
                                     "If you press the <interface>Create new...</interface> button, "
                                     "an editor is opened where you can create and edit a new view profile. "
                                     "The values will be based on the ones of the view profile you selected "
                                     "in the list.")));
    connect(createButton, &QPushButton::clicked, this, &ViewProfilesManageDialog::onCreateNewButtonClicked);
    buttonLayout->addWidget(createButton);
    mEditButton = new QPushButton;
    KGuiItem::assign(mEditButton,
                     KGuiItem(i18nc("@action:button",
                                    "&Edit..."),
                              QStringLiteral("document-edit"),
                              i18nc("@info:tooltip",
                                    "Opens an editor for the view profile."),
                              xi18nc("@info:whatsthis",
                                     "If you press the <interface>Edit...</interface> button, "
                                     "an editor will be opened for the view profile you selected "
                                     "in the list.")));
    connect(mEditButton, &QPushButton::clicked, this, &ViewProfilesManageDialog::onEditButtonClicked);
    buttonLayout->addWidget(mEditButton);
    mSetDefaultButton = new QPushButton;
    KGuiItem::assign(mSetDefaultButton,
                     KGuiItem(i18nc("@action:button",
                                    "&Set as Default"),
                              QStringLiteral("starred-symbolic"),
                              i18nc("@info:tooltip",
                                    "Sets the selected view profile as default for all views."),
                              xi18nc("@info:whatsthis",
                                     "If you press the <interface>Set as Default</interface> button, "
                                     "the view profile you selected in the list is set as default for all views.")));
    connect(mSetDefaultButton, &QPushButton::clicked, this, &ViewProfilesManageDialog::onSetDefaultButtonClicked);
    buttonLayout->addWidget(mSetDefaultButton);
    mDeleteButton = new QPushButton;
    KGuiItem::assign(mDeleteButton,
                     KGuiItem(i18nc("@action:button",
                                    "&Delete"),
                              QStringLiteral("edit-delete"),
                              i18nc("@info:tooltip",
                                    "Deletes the selected view profile."),
                              xi18nc("@info:whatsthis",
                                     "If you press the <interface>Delete</interface> button, "
                                     "the view profile you selected in the list is deleted.")));
    connect(mDeleteButton, &QPushButton::clicked, this, &ViewProfilesManageDialog::onDeleteButtonClicked);
    buttonLayout->addWidget(mDeleteButton);
    buttonLayout->addStretch();

    pageLayout->addWidget(mViewProfileTableView);
    pageLayout->addLayout(buttonLayout);

    // dialog buttons
    auto* dialogButtonBox = new QDialogButtonBox;
    mCloseButton = dialogButtonBox->addButton(QDialogButtonBox::Close);
    connect(mCloseButton, &QAbstractButton::clicked, this, &QDialog::accept);

    // main layout
    auto* layout = new QVBoxLayout;
    layout->addLayout(pageLayout);
    layout->addWidget(dialogButtonBox);
    setLayout(layout);

    mCloseButton->setDefault(true);

    connect(mViewProfileManager, &ByteArrayViewProfileManager::viewProfilesLocked,
            this, &ViewProfilesManageDialog::onViewProfilesLocked);
    connect(mViewProfileManager, &ByteArrayViewProfileManager::viewProfilesLocked,
            this, &ViewProfilesManageDialog::onViewProfilesUnlocked);
    connect(mViewProfileManager, &ByteArrayViewProfileManager::defaultViewProfileChanged,
            this, &ViewProfilesManageDialog::onDefaultViewProfileChanged);

    // select first by default
    onModelReset();
}

ViewProfilesManageDialog::~ViewProfilesManageDialog() = default;

void
ViewProfilesManageDialog::onViewProfileSelectionChanged()
{
    const QItemSelectionModel* selectionModel = mViewProfileTableView->selectionModel();
    const QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    const bool hasSelection = (!selectedIndexes.isEmpty());

    mCurrentViewProfileId = hasSelection ? mViewProfileTableModel->viewProfileId(selectedIndexes.at(0)) : ByteArrayViewProfile::Id();

    const bool isEditable = hasSelection &&
                            !mViewProfileManager->isViewProfileLocked(mCurrentViewProfileId);

    mEditButton->setEnabled(isEditable);
    mDeleteButton->setEnabled(isEditable);

    mSetDefaultButton->setEnabled(isEditable &&
                                  mCurrentViewProfileId != mViewProfileManager->defaultViewProfileId());
}

void
ViewProfilesManageDialog::onCreateNewButtonClicked()
{
    auto* dialog = new ViewProfileEditDialog(this);
    {
        const bool isBasedOnExisting = (!mCurrentViewProfileId.isEmpty());
        ByteArrayViewProfile newByteArrayViewProfile = isBasedOnExisting ?
                                                       mViewProfileManager->viewProfile(mCurrentViewProfileId) : ByteArrayViewProfile();

        if (isBasedOnExisting) {
            // reset id
            newByteArrayViewProfile.setId(QString());
            // twist title
            const QString modifiedTitle = i18n("Modification of %1", newByteArrayViewProfile.viewProfileTitle());
            newByteArrayViewProfile.setViewProfileTitle(modifiedTitle);
        }

        dialog->setViewProfile(newByteArrayViewProfile);
        const QString dialogTitle = i18nc("@window:title",
                                          "New View Profile");
        dialog->setWindowTitle(dialogTitle);

        connect(dialog, &ViewProfileEditDialog::viewProfileAccepted,
                this, &ViewProfilesManageDialog::saveViewProfile);
    }

    dialog->open();

    mCloseButton->setDefault(true);
}

void
ViewProfilesManageDialog::onEditButtonClicked()
{
    if (mCurrentViewProfileId.isEmpty()) {
        return;
    }

    ByteArrayViewProfileLock viewProfileLock =
        mViewProfileManager->createLock(mCurrentViewProfileId);

    if (!viewProfileLock.isLocked()) {
        return;
    }

    const ByteArrayViewProfile viewProfile =
        mViewProfileManager->viewProfile(mCurrentViewProfileId);
    auto* dialog = new ViewProfileEditDialog(this);
    dialog->setViewProfile(viewProfile);
    const QString dialogTitle = i18nc("@window:title",
                                      "\"%1\" View Profile", viewProfile.viewProfileTitle());
    dialog->setWindowTitle(dialogTitle);
    connect(dialog, &ViewProfileEditDialog::viewProfileAccepted,
            this, &ViewProfilesManageDialog::saveViewProfile);

    dialog->open();

    mCloseButton->setDefault(true);
}

void ViewProfilesManageDialog::saveViewProfile(const ByteArrayViewProfile& viewProfile)
{
    QVector<ByteArrayViewProfile> viewProfiles {
        viewProfile
    };
    mViewProfileManager->saveViewProfiles(viewProfiles);
}

void
ViewProfilesManageDialog::onSetDefaultButtonClicked()
{
    if (mCurrentViewProfileId.isEmpty()) {
        return;
    }

    mViewProfileManager->setDefaultViewProfile(mCurrentViewProfileId);

    mCloseButton->setDefault(true);
}

void
ViewProfilesManageDialog::onDeleteButtonClicked()
{
    if (mCurrentViewProfileId.isEmpty()) {
        return;
    }

    // TODO: ask user if she really wants to delete
    const QVector<ByteArrayViewProfile::Id> viewProfileIds {
        mCurrentViewProfileId
    };
    mViewProfileManager->removeViewProfiles(viewProfileIds);

    mCloseButton->setDefault(true);
}

void
ViewProfilesManageDialog::onModelReset()
{
    int row = mViewProfileTableModel->row(mCurrentViewProfileId);

    // no longer exists? set current to first if there is one
    if ((row < 0) && (0 < mViewProfileManager->viewProfilesCount())) {
        row = 0;
    }

    const bool isViewProfileSelected = (0 <= row);
    if (isViewProfileSelected) {
        const QItemSelection selection = QItemSelection(mViewProfileTableModel->index(row, ViewProfileTableModel::CurrentColumnId),
                                                        mViewProfileTableModel->index(row, ViewProfileTableModel::NameColumnId));

        mViewProfileTableView->selectionModel()->select(selection, QItemSelectionModel::Select);
    } else {
        mCurrentViewProfileId.clear();
    }
    // TODO: show a ghost profile with the built-in parameters if there is none

    mEditButton->setEnabled(isViewProfileSelected);
    mSetDefaultButton->setEnabled(isViewProfileSelected);
    mDeleteButton->setEnabled(isViewProfileSelected);
}

void
ViewProfilesManageDialog::onViewProfilesLocked(const QVector<ByteArrayViewProfile::Id>& viewProfileIds)
{
    // find if any locked profile is the currently selected, then disable all buttons
    if (viewProfileIds.contains(mCurrentViewProfileId)) {
        mEditButton->setEnabled(false);
        mDeleteButton->setEnabled(false);
        mSetDefaultButton->setEnabled(false);
    }
}

void
ViewProfilesManageDialog::onViewProfilesUnlocked(const QVector<ByteArrayViewProfile::Id>& viewProfileIds)
{
    // find if any locked profile is the currently selected, then enable all buttons
    if (viewProfileIds.contains(mCurrentViewProfileId)) {
        mEditButton->setEnabled(true);
        mDeleteButton->setEnabled(true);
        if (mCurrentViewProfileId != mViewProfileManager->defaultViewProfileId()) {
            mSetDefaultButton->setEnabled(true);
        }
    }
}

void
ViewProfilesManageDialog::onDefaultViewProfileChanged(const ByteArrayViewProfile::Id& viewProfileId)
{
    mSetDefaultButton->setEnabled((!mCurrentViewProfileId.isEmpty()) &&
                                  (mCurrentViewProfileId != viewProfileId));
}

}
