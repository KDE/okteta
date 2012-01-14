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

#include "viewprofilesynccontroller.h"

// controller
#include "viewprofileeditdialog.h"
// Okteta Gui Kasten
#include <bytearrayviewprofilemanager.h>
#include <bytearrayviewprofilesynchronizer.h>
#include <bytearrayview.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KActionCollection>
#include <KAction>


namespace Kasten2
{

ViewProfileSyncController::ViewProfileSyncController( KXMLGUIClient* guiClient,
                                                      ByteArrayViewProfileManager* viewProfileManager,
                                                      QWidget* parentWidget )
  : mGuiClient( guiClient )
  , mViewProfileManager( viewProfileManager )
  , mParentWidget( parentWidget )
  , mByteArrayView( 0 )
  , mByteArrayViewProfileSynchronizer( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mCreateNewAction = actionCollection->addAction( QLatin1String("view_profile_new"),
                                                    this, SLOT(onCreateNewActionTriggered()) );
    mCreateNewAction->setIcon( KIcon( QLatin1String("document-new") ) );
    mCreateNewAction->setText( i18nc("@title:menu create a new view profile",
                                     "Create new...") );

    mSaveChangesAction = actionCollection->addAction( QLatin1String("view_profile_save"),
                                                    this, SLOT(onSaveChangesActionTriggered()) );
    mSaveChangesAction->setIcon( KIcon( QLatin1String("document-save") ) );
    mSaveChangesAction->setText( i18nc("@title:menu save changed to the view profile to the base profile",
                                       "Save changes") );
    mResetChangesAction = actionCollection->addAction( QLatin1String("view_profile_reload"),
                                                    this, SLOT(onResetChangesActionTriggered()) );
    mResetChangesAction->setIcon( KIcon( QLatin1String("document-revert") ) );
    mResetChangesAction->setText( i18nc("@title:menu reset settings back to those of the saved base profile",
                                        "Reset changes") );

    setTargetModel( 0 );
}

void ViewProfileSyncController::setTargetModel( AbstractModel* model )
{
    if( mByteArrayViewProfileSynchronizer )
        mByteArrayViewProfileSynchronizer->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;
    mByteArrayViewProfileSynchronizer = mByteArrayView ? mByteArrayView->synchronizer() : 0;

    const bool hasView = ( mByteArrayView != 0 );
    const bool hasSynchronizer = ( mByteArrayViewProfileSynchronizer != 0 );
    if( hasSynchronizer )
    {
        onLocalSyncStateChanged( mByteArrayViewProfileSynchronizer->localSyncState() );

        connect( mByteArrayViewProfileSynchronizer, SIGNAL(localSyncStateChanged(Kasten2::LocalSyncState)),
                 SLOT(onLocalSyncStateChanged(Kasten2::LocalSyncState)) );
    }
    else
    {
        mSaveChangesAction->setEnabled( false );
        mResetChangesAction->setEnabled( false );
    }

    mCreateNewAction->setEnabled( hasView );
}


void ViewProfileSyncController::onLocalSyncStateChanged( Kasten2::LocalSyncState localSyncState )
{
    const bool hasDifference = ( localSyncState == LocalHasChanges );

    mSaveChangesAction->setEnabled( hasDifference );
    mResetChangesAction->setEnabled( hasDifference );
}

void ViewProfileSyncController::onCreateNewActionTriggered()
{
    ViewProfileEditDialog* dialog = new ViewProfileEditDialog( mParentWidget );
    const QString dialogTitle = i18nc("@window:title",
                                      "Create new View Profile");
    dialog->setWindowTitle( dialogTitle );

    ByteArrayViewProfile viewProfile;
    viewProfile.setId( QString() );
//     const QString modifiedTitle = i18n( "Modification of %1", newByteArrayViewProfile.viewProfileTitle() );
//     viewProfile.setViewProfileTitle( modifiedTitle );
    viewProfile.setOffsetColumnVisible( mByteArrayView->offsetColumnVisible() );
    viewProfile.setVisibleByteArrayCodings( mByteArrayView->visibleByteArrayCodings() );
    viewProfile.setViewModus( mByteArrayView->viewModus() );
    viewProfile.setLayoutStyle( mByteArrayView->layoutStyle() );
    viewProfile.setNoOfGroupedBytes( mByteArrayView->noOfGroupedBytes() );
    viewProfile.setNoOfBytesPerLine( mByteArrayView->noOfBytesPerLine() );
    viewProfile.setValueCoding( mByteArrayView->valueCoding() );
    viewProfile.setCharCoding( mByteArrayView->charCodingName() );
    viewProfile.setShowsNonprinting( mByteArrayView->showsNonprinting() );
    viewProfile.setUndefinedChar( mByteArrayView->undefinedChar() );
    viewProfile.setSubstituteChar( mByteArrayView->substituteChar() );
    dialog->setViewProfile( viewProfile );

    const int answer = dialog->exec();

    if( answer == KDialog::Accepted )
    {
        QList<ByteArrayViewProfile> viewProfiles;
        viewProfiles << dialog->viewProfile();
        mViewProfileManager->saveViewProfiles( viewProfiles );

        mByteArrayViewProfileSynchronizer->setViewProfileId( viewProfiles.at(0).id() );
    }

    delete dialog;
}

void ViewProfileSyncController::onResetChangesActionTriggered()
{
    if( mByteArrayViewProfileSynchronizer )
        mByteArrayViewProfileSynchronizer->syncFromRemote();
}

void ViewProfileSyncController::onSaveChangesActionTriggered()
{
    if( mByteArrayViewProfileSynchronizer )
        mByteArrayViewProfileSynchronizer->syncToRemote();
}

}
