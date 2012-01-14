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

#include "viewprofilesmanagedialog.h"

// this
#include "viewprofileeditdialog.h"
#include "viewprofiletablemodel.h"
// Okteta Gui Kasten
#include <bytearrayviewprofilemanager.h>
#include <bytearrayviewprofilelock.h>
// KDE
#include <KPushButton>
#include <KLocale>
// Qt
#include <QtGui/QHBoxLayout>
#include <QtGui/QTreeView>
#include <QtGui/QItemSelectionModel>


namespace Kasten2
{

ViewProfilesManageDialog::ViewProfilesManageDialog( ByteArrayViewProfileManager* viewProfileManager,
                                                    QWidget* parent )
  : KDialog( parent )
  , mViewProfileManager( viewProfileManager )
  , mCurrentViewProfileId()
{
    QWidget* page = new QWidget( this );
    setMainWidget( page );

    QHBoxLayout* pageLayout = new QHBoxLayout( page );

    // profile list
    mViewProfileTableView = new QTreeView( page );
    mViewProfileTableModel = new ViewProfileTableModel( mViewProfileManager, this );
    mViewProfileTableView->setObjectName( QLatin1String("ViewProfileTableView") );
    mViewProfileTableView->setHeaderHidden( true );
    mViewProfileTableView->setRootIsDecorated( false );
    mViewProfileTableView->setItemsExpandable( false );
    mViewProfileTableView->setUniformRowHeights( true );
    mViewProfileTableView->setAllColumnsShowFocus( true );
    mViewProfileTableView->setModel( mViewProfileTableModel );
    connect( mViewProfileTableView->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             SLOT(onViewProfileSelectionChanged()) );
    connect( mViewProfileTableModel, SIGNAL(modelReset()),
             SLOT(onModelReset()) );

    // buttons
    QVBoxLayout* buttonLayout = new QVBoxLayout;
    KPushButton* createButton = // copy from selected
        new KPushButton(
            KGuiItem(i18nc("@action:button",
                           "&Create new..."),
                     QLatin1String("document-new"),
                     i18nc("@info:tooltip",
                           "Opens an editor for a new view profile."),
                     i18nc("@info:whatsthis",
                           "If you press the <interface>Create new...</interface> button, "
                           "an editor is opened where you can create and edit a new view profile. "
                           "The values will be based on the ones of the view profile you selected "
                           "in the list.")),
            page );
    connect( createButton, SIGNAL(clicked(bool)), SLOT(onCreateNewButtonClicked()) );
    buttonLayout->addWidget( createButton );
    mEditButton =
        new KPushButton(
            KGuiItem(i18nc("@action:button",
                           "&Edit..."),
                     QLatin1String("document-edit"),
                     i18nc("@info:tooltip",
                           "Opens an editor for the view profile."),
                     i18nc("@info:whatsthis",
                           "If you press the <interface>Edit...</interface> button, "
                           "an editor will be opened for the view profile you selected "
                           "in the list.")),
            page );
    connect( mEditButton, SIGNAL(clicked(bool)), SLOT(onEditButtonClicked()) );
    buttonLayout->addWidget( mEditButton );
    mSetDefaultButton =
        new KPushButton(
            KGuiItem(i18nc("@action:button",
                           "&Set as Default"),
                     QString(),
                     i18nc("@info:tooltip",
                           "Sets the selected view profile as default for all views."),
                     i18nc("@info:whatsthis",
                           "If you press the <interface>Set as Default</interface> button, "
                           "the view profile you selected in the list is set as default for all views.")),
            page );
    connect( mSetDefaultButton, SIGNAL(clicked(bool)), SLOT(onSetDefaultButtonClicked()) );
    buttonLayout->addWidget( mSetDefaultButton );
    mDeleteButton =
        new KPushButton(
            KGuiItem(i18nc("@action:button",
                           "&Delete"),
                     QLatin1String("list-remove"),
                     i18nc("@info:tooltip",
                           "Deletes the selected view profile."),
                     i18nc("@info:whatsthis",
                           "If you press the <interface>Delete</interface> button, "
                           "the view profile you selected in the list is deleted.")),
            page );
    connect( mDeleteButton, SIGNAL(clicked(bool)), SLOT(onDeleteButtonClicked()) );
    buttonLayout->addWidget( mDeleteButton );
    buttonLayout->addStretch();

    pageLayout->addWidget( mViewProfileTableView );
    pageLayout->addLayout( buttonLayout );

    setButtons( Close );

    connect( mViewProfileManager, SIGNAL(viewProfilesLocked(QList<Kasten2::ByteArrayViewProfile::Id>)),
             SLOT(onViewProfilesLocked(QList<Kasten2::ByteArrayViewProfile::Id>)) );
    connect( mViewProfileManager, SIGNAL(viewProfilesLocked(QList<Kasten2::ByteArrayViewProfile::Id>)),
             SLOT(onViewProfilesUnlocked(QList<Kasten2::ByteArrayViewProfile::Id>)) );
    connect( mViewProfileManager, SIGNAL(defaultViewProfileChanged(Kasten2::ByteArrayViewProfile::Id)),
             SLOT(onDefaultViewProfileChanged(Kasten2::ByteArrayViewProfile::Id)) );

    // select first by default
    const QItemSelection selection =
        QItemSelection( mViewProfileTableModel->index(0,ViewProfileTableModel::CurrentColumnId),
                        mViewProfileTableModel->index(0,ViewProfileTableModel::NameColumnId) );

    mViewProfileTableView->selectionModel()->select( selection, QItemSelectionModel::Select );
}


void
ViewProfilesManageDialog::onViewProfileSelectionChanged()
{
    const QItemSelectionModel* selectionModel = mViewProfileTableView->selectionModel();
    const QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    const bool hasSelection = ( ! selectedIndexes.isEmpty() );

    mCurrentViewProfileId = hasSelection ? mViewProfileTableModel->viewProfileId( selectedIndexes.at(0) ) : ByteArrayViewProfile::Id();

    const bool isEditable = hasSelection &&
                            ! mViewProfileManager->isViewProfileLocked( mCurrentViewProfileId );

    mEditButton->setEnabled( isEditable );
    mDeleteButton->setEnabled( isEditable );

    mSetDefaultButton->setEnabled( isEditable &&
                                   mCurrentViewProfileId != mViewProfileManager->defaultViewProfileId() );
}

void
ViewProfilesManageDialog::onCreateNewButtonClicked()
{
    ViewProfileEditDialog* dialog = new ViewProfileEditDialog( this );
    {
        const bool isBasedOnExisting = (! mCurrentViewProfileId.isEmpty());
        ByteArrayViewProfile newByteArrayViewProfile = isBasedOnExisting ?
            mViewProfileManager->viewProfile( mCurrentViewProfileId ) : ByteArrayViewProfile();

        if( isBasedOnExisting )
        {
            // reset id
            newByteArrayViewProfile.setId( QString() );
            // twist title
            const QString modifiedTitle = i18n( "Modification of %1", newByteArrayViewProfile.viewProfileTitle() );
            newByteArrayViewProfile.setViewProfileTitle( modifiedTitle );
        }

        dialog->setViewProfile( newByteArrayViewProfile );
    }

    const int answer = dialog->exec();

    if( answer == KDialog::Accepted )
    {
        QList<ByteArrayViewProfile> viewProfiles;
        viewProfiles << dialog->viewProfile();
        mViewProfileManager->saveViewProfiles( viewProfiles );
    }

    delete dialog;

    setButtonFocus( Close );
}

void
ViewProfilesManageDialog::onEditButtonClicked()
{
    if( mCurrentViewProfileId.isEmpty() )
        return;

    ByteArrayViewProfileLock viewProfileLock =
        mViewProfileManager->createLock( mCurrentViewProfileId );

    if( ! viewProfileLock.isLocked() )
        return;

    const ByteArrayViewProfile viewProfile =
            mViewProfileManager->viewProfile( mCurrentViewProfileId );
    ViewProfileEditDialog* dialog = new ViewProfileEditDialog( this );
    dialog->setViewProfile( viewProfile );

    const int answer = dialog->exec();
    if( answer == KDialog::Accepted )
    {
        QList<ByteArrayViewProfile> viewProfiles;
        viewProfiles << dialog->viewProfile();
        mViewProfileManager->saveViewProfiles( viewProfiles );
    }

    delete dialog;

    setButtonFocus( Close );
}

void
ViewProfilesManageDialog::onSetDefaultButtonClicked()
{
    if( mCurrentViewProfileId.isEmpty() )
        return;

    mViewProfileManager->setDefaultViewProfile( mCurrentViewProfileId );

    setButtonFocus( Close );
}

void
ViewProfilesManageDialog::onDeleteButtonClicked()
{
    if( mCurrentViewProfileId.isEmpty() )
        return;

    // TODO: ask user if she really wants to delete
    QList<ByteArrayViewProfile::Id> viewProfileIds;
    viewProfileIds << mCurrentViewProfileId;
    mViewProfileManager->removeViewProfiles( viewProfileIds );

    setButtonFocus( Close );
}

void
ViewProfilesManageDialog::onModelReset()
{
    int row = mViewProfileTableModel->row( mCurrentViewProfileId );

    // no longer exists? set current to first if there is one
    if( (row < 0) && (0 < mViewProfileManager->viewProfilesCount()) )
        row = 0;

    if( 0 <= row )
    {
        const QItemSelection selection = QItemSelection( mViewProfileTableModel->index(row,ViewProfileTableModel::CurrentColumnId),
                                                        mViewProfileTableModel->index(row,ViewProfileTableModel::NameColumnId) );

        mViewProfileTableView->selectionModel()->select( selection, QItemSelectionModel::Select );
    }
    else
        mCurrentViewProfileId.clear();
    // TODO: show a ghost profile with the built-in parameters if there is none
}

void
ViewProfilesManageDialog::onViewProfilesLocked( const QList<ByteArrayViewProfile::Id>& viewProfileIds )
{
    // find if any locked profile is the currently selected, then disable all buttons
    if( viewProfileIds.contains(mCurrentViewProfileId) )
    {
        mEditButton->setEnabled( false );
        mDeleteButton->setEnabled( false );
        mSetDefaultButton->setEnabled( false );
    }
}

void
ViewProfilesManageDialog::onViewProfilesUnlocked( const QList<ByteArrayViewProfile::Id>& viewProfileIds )
{
    // find if any locked profile is the currently selected, then enable all buttons
    if( viewProfileIds.contains(mCurrentViewProfileId) )
    {
        mEditButton->setEnabled( true );
        mDeleteButton->setEnabled( true );
        if( mCurrentViewProfileId != mViewProfileManager->defaultViewProfileId() )
            mSetDefaultButton->setEnabled( true );
    }
}

void
ViewProfilesManageDialog::onDefaultViewProfileChanged( const ByteArrayViewProfile::Id& viewProfileId )
{
    mSetDefaultButton->setEnabled( (! mCurrentViewProfileId.isEmpty()) &&
                                   (mCurrentViewProfileId != viewProfileId) );
}


ViewProfilesManageDialog::~ViewProfilesManageDialog()
{
}

}
