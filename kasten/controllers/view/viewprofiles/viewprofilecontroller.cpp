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
#include <bytearrayviewprofilemanager.h>
#include <bytearrayviewprofilesynchronizer.h>
#include <bytearrayview.h>
// KDE
#include <KXMLGUIClient>
#include <KXMLGUIFactory>
#include <KActionMenu>
#include <KActionCollection>
#include <KAction>
#include <KLocale>


namespace Kasten2
{

ViewProfileController::ViewProfileController( ByteArrayViewProfileManager* viewProfileManager,
                                              QWidget* parentWidget,
                                              KXMLGUIClient* guiClient )
  : AbstractXmlGuiController()
  , mGuiClient( guiClient )
  , mViewProfileManager( viewProfileManager )
  , mParentWidget( parentWidget )
  , mByteArrayView( 0 )
  , mByteArrayViewProfileSynchronizer( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mViewProfileActionMenu = actionCollection->add<KActionMenu>( QLatin1String("view_profile") );
    mViewProfileActionMenu->setDelayed( false );
    mViewProfileActionMenu->setText( i18nc("@title:menu submenu to select the view profile or change it",
                                        "View Profile") );

    mCreateNewAction =
        new QAction( KIcon( QLatin1String("document-new") ),
                     i18nc("@title:menu create a new view profile",
                            "Create new..." ),
                     this );
    connect( mCreateNewAction, SIGNAL(triggered(bool)), SLOT(onCreateNewActionTriggered()) );

    mSaveChangesAction =
        new QAction( KIcon( QLatin1String("document-save") ),
                     i18nc("@title:menu save changed to the view profile to the base profile",
                           "Save changes" ),
                     this );
    connect( mSaveChangesAction, SIGNAL(triggered(bool)), SLOT(onSaveChangesActionTriggered()) );

    mResetChangesAction =
        new QAction( KIcon(QLatin1String("document-revert")),
                     i18nc("@title:menu reset settings back to those of the saved base profile",
                           "Reset changes" ),
                     this );
    connect( mResetChangesAction, SIGNAL(triggered(bool)), SLOT(onResetChangesActionTriggered()) );

    mViewProfileActionMenu->addAction( mCreateNewAction );
    mViewProfileActionMenu->addSeparator();
    mViewProfileActionMenu->addAction( mSaveChangesAction );
    mViewProfileActionMenu->addAction( mResetChangesAction );

    mViewProfilesActionGroup = new QActionGroup( this );
    mViewProfilesActionGroup->setExclusive( true );
    connect( mViewProfilesActionGroup, SIGNAL(triggered(QAction*)),
             SLOT(onViewProfileTriggered(QAction*)) );

    connect( mViewProfileManager, SIGNAL(viewProfilesChanged(QList<Kasten2::ByteArrayViewProfile>)),
             SLOT(onViewProfilesChanged()) );
    connect( mViewProfileManager, SIGNAL(viewProfilesRemoved(QList<Kasten2::ByteArrayViewProfile::Id>)),
             SLOT(onViewProfilesChanged()) );

    onViewProfilesChanged();

    setTargetModel( 0 );
}

void ViewProfileController::setTargetModel( AbstractModel* model )
{
    if( mByteArrayViewProfileSynchronizer )
        mByteArrayViewProfileSynchronizer->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;
    mByteArrayViewProfileSynchronizer = mByteArrayView ? mByteArrayView->synchronizer() : 0;

    const bool hasSynchronizer = ( mByteArrayViewProfileSynchronizer != 0 );
    if( hasSynchronizer )
    {
        onViewProfileChanged( mByteArrayViewProfileSynchronizer->viewProfileId() );

        connect( mByteArrayViewProfileSynchronizer, SIGNAL(viewProfileChanged(Kasten2::ByteArrayViewProfile::Id)),
                 SLOT(onViewProfileChanged(Kasten2::ByteArrayViewProfile::Id)) );

        onLocalSyncStateChanged( mByteArrayViewProfileSynchronizer->localSyncState() );

        connect( mByteArrayViewProfileSynchronizer, SIGNAL(localSyncStateChanged(Kasten2::LocalSyncState)),
                 SLOT(onLocalSyncStateChanged(Kasten2::LocalSyncState)) );
    }
    else
    {
        mSaveChangesAction->setEnabled( false );
        mResetChangesAction->setEnabled( false );
    }

    mCreateNewAction->setEnabled( hasSynchronizer );
    mViewProfileActionMenu->setEnabled( hasSynchronizer );
}


void ViewProfileController::onViewProfileChanged( const Kasten2::ByteArrayViewProfile::Id& viewProfileId )
{
    const QList<QAction*> actions = mViewProfilesActionGroup->actions();

    foreach( QAction* action, actions )
    {
        if( action->data().toString() == viewProfileId )
        {
            action->setChecked( true );
            break;
        }
    }
}

void ViewProfileController::onViewProfilesChanged()
{
    qDeleteAll( mViewProfilesActionGroup->actions() );

    const QList<ByteArrayViewProfile> viewProfiles = mViewProfileManager->viewProfiles();
    const ByteArrayViewProfile::Id currentViewProfileId = mByteArrayViewProfileSynchronizer ?
        mByteArrayViewProfileSynchronizer->viewProfileId() :
        ByteArrayViewProfile::Id();

    if( ! viewProfiles.isEmpty() )
        mViewProfileActionMenu->addSeparator();

    bool isCurrentViewProfileExisting = false;
    foreach( const ByteArrayViewProfile& viewProfile, viewProfiles )
    {
        const QString title = viewProfile.viewProfileTitle();
        QAction* action = new QAction( title, mViewProfilesActionGroup );
        action->setCheckable( true );
        const ByteArrayViewProfile::Id viewProfileId = viewProfile.id();
        action->setData( viewProfileId );
        const bool isCurrentViewProfile = ( viewProfileId == currentViewProfileId );
        action->setChecked( isCurrentViewProfile );
        if( isCurrentViewProfile )
            isCurrentViewProfileExisting = true;

        mViewProfilesActionGroup->addAction( action );
        mViewProfileActionMenu->addAction( action );
    }

    // reset id if no longer existing
    if( ! isCurrentViewProfileExisting && mByteArrayViewProfileSynchronizer )
            mByteArrayViewProfileSynchronizer->setViewProfileId( ByteArrayViewProfile::Id() );
}



void ViewProfileController::onViewProfileTriggered( QAction* action )
{
    mByteArrayViewProfileSynchronizer->setViewProfileId( action->data().toString() );
}

void ViewProfileController::onLocalSyncStateChanged( Kasten2::LocalSyncState localSyncState )
{
    const bool hasDifference = ( localSyncState == LocalHasChanges );

    mSaveChangesAction->setEnabled( hasDifference );
    mResetChangesAction->setEnabled( hasDifference );
}

void ViewProfileController::onCreateNewActionTriggered()
{
    ViewProfileEditDialog* dialog = new ViewProfileEditDialog( mParentWidget );
    const QString dialogTitle = i18nc( "@window:title",
                                       "New View Profile" );
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

void ViewProfileController::onResetChangesActionTriggered()
{
    mByteArrayViewProfileSynchronizer->syncFromRemote();
}

void ViewProfileController::onSaveChangesActionTriggered()
{
    mByteArrayViewProfileSynchronizer->syncToRemote();
}

}
