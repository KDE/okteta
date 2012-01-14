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

#include "viewprofilescontroller.h"

// controller
#include "viewprofileeditdialog.h"
// Okteta Gui Kasten
#include <bytearrayviewprofilemanager.h>
#include <bytearrayviewprofilesynchronizer.h>
#include <bytearrayview.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KAction>


namespace Kasten2
{

ViewProfilesController::ViewProfilesController( KXMLGUIClient* guiClient,
                                                ByteArrayViewProfileManager* viewProfileManager,
                                                QWidget* parentWidget )
  : mGuiClient( guiClient )
  , mViewProfileManager( viewProfileManager )
  , mParentWidget( parentWidget )
  , mByteArrayView( 0 )
  , mByteArrayViewProfileSynchronizer( 0 )
{
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

void ViewProfilesController::setTargetModel( AbstractModel* model )
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
    }

    mViewProfilesActionGroup->setEnabled( hasSynchronizer );
}

void ViewProfilesController::onViewProfileChanged( const Kasten2::ByteArrayViewProfile::Id& viewProfileId )
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

void ViewProfilesController::onViewProfilesChanged()
{
    mGuiClient->unplugActionList( QLatin1String("view_profile_list") );

    qDeleteAll( mViewProfilesActionGroup->actions() );

    const QList<ByteArrayViewProfile> viewProfiles = mViewProfileManager->viewProfiles();
    const ByteArrayViewProfile::Id currentViewProfileId = mByteArrayViewProfileSynchronizer ?
        mByteArrayViewProfileSynchronizer->viewProfileId() :
        ByteArrayViewProfile::Id();

    foreach( const ByteArrayViewProfile& viewProfile, viewProfiles )
    {
        const QString title = viewProfile.viewProfileTitle();
        QAction* action = new QAction( title, mViewProfilesActionGroup );
        action->setCheckable( true );
        const ByteArrayViewProfile::Id viewProfileId = viewProfile.id();
        action->setData( viewProfileId );
        if( viewProfileId == currentViewProfileId )
            action->setChecked( true );

        mViewProfilesActionGroup->addAction( action );
    }

    mGuiClient->plugActionList( QLatin1String("view_profile_list"), mViewProfilesActionGroup->actions() );
}

void ViewProfilesController::onViewProfileTriggered( QAction* action )
{
    if( mByteArrayViewProfileSynchronizer )
        mByteArrayViewProfileSynchronizer->setViewProfileId( action->data().toString() );
}

}
