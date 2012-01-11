/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewprofilescontroller.h"

// controller
#include "viewprofileeditdialog.h"
// Okteta Gui Kasten
#include <bytearrayviewprofilemanager.h>
#include <bytearrayviewprofile.h>
#include <bytearrayview.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KActionCollection>
#include <KActionMenu>
#include <KAction>


namespace Kasten2
{
// KSharedConfigPtr KSharedConfig::openConfig(profileName, KConfig::SimpleConfig, "data")

ViewProfilesController::ViewProfilesController( KXMLGUIClient* guiClient,
                                                ByteArrayViewProfileManager* viewProfileManager,
                                                QWidget* parentWidget )
  : mGuiClient( guiClient ),
    mViewProfileManager( viewProfileManager ),
    mParentWidget( parentWidget ),
    mByteArrayView( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    mViewProfileMenuAction =
        actionCollection->add<KActionMenu>( "view_profiles", this, SLOT(onNewActionTriggered()) );
    mViewProfileMenuAction->setText( i18nc("@title:menu submenu to select the view profile or change it",
                                  "View Profile" ) );
//     newMenuAction->setIcon( KIcon("document-new") );

    mCreateNewAction =
        new QAction( KIcon("document-new"),
                     i18nc("@title:menu create a new view profile", "Create new..." ), this );
    connect( mCreateNewAction, SIGNAL(triggered(bool)), SLOT(onCreateNewActionTriggered()) );

    mResetChangesAction =
        new QAction( KIcon("document-revert"),
                     i18nc("@title:menu ", "Reset changes" ), this );
    connect( mResetChangesAction, SIGNAL(triggered(bool)), SLOT(onResetChangesActionTriggered()) );

    mSaveChangesAction =
        new QAction( KIcon("document-save"),
                     i18nc("@title:menu ", "Save changes" ), this );
    connect( mSaveChangesAction, SIGNAL(triggered(bool)), SLOT(onSaveChangesActionTriggered()) );

    mViewProfileMenuAction->addAction( mCreateNewAction );
    mViewProfileMenuAction->addSeparator();
    mViewProfileMenuAction->addAction( mResetChangesAction );
    mViewProfileMenuAction->addAction( mSaveChangesAction );
    mViewProfileMenuAction->addSeparator();

    mViewProfilesActionGroup = new QActionGroup( this ); // TODO: do we use this only for the signal mapping?
//     mBookmarksActionGroup->setExclusive( true );
    connect( mViewProfilesActionGroup, SIGNAL(triggered(QAction*)),
             SLOT(onViewProfileTriggered(QAction*)) );

    updateViewProfileActions();

    setTargetModel( 0 );
}

void ViewProfilesController::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    const bool hasView = ( mByteArrayView != 0 );
    if( hasView )
    {
#if 0
        mShowOffsetColumnAction->setChecked( mByteArrayView->offsetColumnVisible() );
        mShowsNonprintingAction->setChecked( mByteArrayView->showsNonprinting() );

        mCodingAction->setCurrentItem( mByteArrayView->valueCoding() );
        mEncodingAction->setCurrentItem( Okteta::CharCodec::codecNames().indexOf(mByteArrayView->charCodingName()) );

        mResizeStyleAction->setCurrentItem( mByteArrayView->layoutStyle() );

        mToggleColumnsAction->setCurrentItem( mByteArrayView->visibleByteArrayCodings()-1 );

        connect( mByteArrayView, SIGNAL(valueCodingChanged(int)), SLOT(onValueCodingChanged(int)) );
#endif
        connect( mByteArrayView, SIGNAL(valueCodingChanged(int)),
                 SLOT(onViewSettingsChanged()) );
        connect( mByteArrayView, SIGNAL(charCodecChanged(QString)),
                 SLOT(onViewSettingsChanged()) );
    }

    mCreateNewAction->setEnabled( hasView );
    onViewSettingsChanged();
}

void ViewProfilesController::updateViewProfileActions()
{
#if 0
    mGuiClient->unplugActionList( BookmarkListActionListId );

    qDeleteAll( mViewProfilesActionGroup->actions() );

    const QList<ViewProfile> viewProfiles; // = mViewProfileManager->viewProfiles();

    foreach( const ViewProfile& viewProfile, viewProfiles )
    {
        const QString title = viewProfile.title();
        QAction* action = new QAction( title, mViewProfilesActionGroup );

        action->setData( viewProfile.id() );
        mViewProfilesActionGroup->addAction( action );
        mViewProfileMenuAction->addAction( action );
    }

    mGuiClient->plugActionList( BookmarkListActionListId, mBookmarksActionGroup->actions() );
#endif
}

void ViewProfilesController::onViewSettingsChanged()
{
    const bool hasView = ( mByteArrayView != 0 );

    // TODO: find out if still different
    const bool hasDifference = hasView /*&& ()*/;
    mSaveChangesAction->setEnabled( hasDifference );
    mResetChangesAction->setEnabled( hasDifference );
}

void ViewProfilesController::onViewProfilesAdded( const QList<ViewProfile>& viewProfiles )
{
Q_UNUSED( viewProfiles )

    updateViewProfileActions();
}

void ViewProfilesController::onViewProfilesChanged( const QList<ViewProfile>& viewProfiles )
{
Q_UNUSED( viewProfiles )
    // TODO: find out if profile of current view is contained in changed
//     if( viewProfiles.contains() )
    {
        // TODO: find out if still different
        const bool hasDifference = true /*&& ()*/;
        mSaveChangesAction->setEnabled( hasDifference );
        mResetChangesAction->setEnabled( hasDifference );
    }

    // titles could have changed
    updateViewProfileActions();
}

void ViewProfilesController::onViewProfilesRemoved( const QList<ViewProfile>& viewProfiles )
{
Q_UNUSED( viewProfiles )
    // TODO: find out if profile of current view is contained in removed

    const bool hasViewProfile = true;
    const bool hasDifference = hasViewProfile /*&& ()*/;
    mSaveChangesAction->setEnabled( hasDifference );
    mResetChangesAction->setEnabled( hasDifference );

    updateViewProfileActions();
}

void ViewProfilesController::onCreateNewActionTriggered()
{
    ViewProfileEditDialog* dialog = new ViewProfileEditDialog( mParentWidget );
    const QString dialogTitle = i18nc("@window:title",
                                      "Create new View Profile");
    dialog->setWindowTitle( dialogTitle );

    ByteArrayViewProfile viewProfile;
    viewProfile.setViewProfileTitle( i18n("Modification of X") ); // X = title of current view profile
    viewProfile.toggleOffsetColumn( mByteArrayView->offsetColumnVisible() );
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
    if( answer == KDialog::Ok )
    {
        QList<ByteArrayViewProfile> viewProfiles;
        viewProfiles << dialog->viewProfile();
        mViewProfileManager->addViewProfiles( viewProfiles );
        //register viewProfile for mByteArrayView
    }

    delete dialog;
}

void ViewProfilesController::onResetChangesActionTriggered()
{
}

void ViewProfilesController::onSaveChangesActionTriggered()
{
}

void ViewProfilesController::onSetViewProfileActionTriggered()
{
//     QAction* action = static_cast<QAction*>( sender() );

//     AbstractModelDataGenerator* generator = action->data().value<AbstractModelDataGenerator* >();
}

void ViewProfilesController::onViewProfileTriggered( QAction* action )
{
    Q_UNUSED( action );
}

}
