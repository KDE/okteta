/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayviewprofilesynchronizer.h"

// library
#include <bytearrayviewprofilemanager.h>
#include <bytearrayview.h>

#include <KDebug>
namespace Kasten2
{

ByteArrayViewProfileSynchronizer::ByteArrayViewProfileSynchronizer( ByteArrayViewProfileManager* viewProfileManager )
  : QObject()
  , mView( 0 )
  , mViewProfileId()
  , mDirtyFlags( 0 )
  , mUpdatingView( false )
  , mViewProfileManager( viewProfileManager )
{
}

LocalSyncState
ByteArrayViewProfileSynchronizer::localSyncState() const
{
    return ( mDirtyFlags == 0 ) ? LocalInSync : LocalHasChanges;
}

void
ByteArrayViewProfileSynchronizer::setView( ByteArrayView* view )
{
    if( mView && ! mViewProfileId.isEmpty() ) mView->disconnect( this );

    mView = view;

    // TODO: drop all changes and set view to profile if set
    mDirtyFlags = 0;

    if( mView && ! mViewProfileId.isEmpty() )
    {
        const ByteArrayViewProfile viewProfile = mViewProfileManager->viewProfile( mViewProfileId );
        updateView( viewProfile );

        connect( mView, SIGNAL(showsNonprintingChanged(bool)),
                 SLOT(onShowsNonprintingChanged()) );
        connect( mView, SIGNAL(valueCodingChanged(int)),
                 SLOT(onValueCodingChanged()) );
        connect( mView, SIGNAL(charCodecChanged(QString)),
                 SLOT(onCharCodecChanged()) );
        connect( mView, SIGNAL(substituteCharChanged(QChar)),
                 SLOT(onSubstituteCharChanged()) );
        connect( mView, SIGNAL(undefinedCharChanged(QChar)),
                 SLOT(onUndefinedCharChanged()) );
        connect( mView, SIGNAL(visibleByteArrayCodingsChanged(int)),
                 SLOT(onVisibleByteArrayCodingsChanged()) );
        connect( mView, SIGNAL(offsetColumnVisibleChanged(bool)),
                 SLOT(onOffsetColumnVisibleChanged()) );
        connect( mView, SIGNAL(noOfBytesPerLineChanged(int)),
                 SLOT(onNoOfBytesPerLineChanged()) );
        connect( mView, SIGNAL(noOfGroupedBytesChanged(int)),
                 SLOT(onNoOfGroupedBytesChanged()) );
        connect( mView, SIGNAL(layoutStyleChanged(int)),
                 SLOT(onLayoutStyleChanged()) );
        connect( mView, SIGNAL(viewModusChanged(int)),
                 SLOT(onViewModusChanged()) );
    }
}

void ByteArrayViewProfileSynchronizer::setViewProfileId( const ByteArrayViewProfile::Id& viewProfileId )
{
    if( mViewProfileId == viewProfileId )
        return;

    const bool isListeningBefore = ( ! mViewProfileId.isEmpty() );

    mViewProfileId = viewProfileId;

    // TODO: drop all changes and set view to profile if set
    mDirtyFlags = 0;
    if( ! viewProfileId.isEmpty() )
    {
        const ByteArrayViewProfile viewProfile = mViewProfileManager->viewProfile( viewProfileId );
        updateView( viewProfile );
    }

    const bool isListeningAfter = ( ! mViewProfileId.isEmpty() );

    if( isListeningAfter)
    {
        if( ! isListeningBefore  )
        {
            connect( mViewProfileManager, SIGNAL(viewProfilesChanged(QList<Kasten2::ByteArrayViewProfile>)),
                     SLOT(onViewProfilesChanged(QList<Kasten2::ByteArrayViewProfile>)) );
            connect( mViewProfileManager, SIGNAL(viewProfilesRemoved(QList<Kasten2::ByteArrayViewProfile::Id>)),
                     SLOT(onViewProfilesRemoved(QList<Kasten2::ByteArrayViewProfile::Id>)) );
            if( mView  )
            {
                connect( mView, SIGNAL(valueCodingChanged(int)), SIGNAL(valueCodingChanged(int)) );
                connect( mView, SIGNAL(charCodecChanged(QString)), SIGNAL(charCodecChanged(QString)) );
            }
        }
    }
    else
    {
        if ( isListeningBefore )
        {
            mViewProfileManager->disconnect( this );
            if( mView ) mView->disconnect( this );
        }
    }

    if( mView )
        emit localSyncStateChanged( LocalInSync );
}

void
ByteArrayViewProfileSynchronizer::syncToRemote()
{
    if( (mView == 0) || (mViewProfileId.isEmpty()) )
        return;

    ByteArrayViewProfile viewProfile = mViewProfileManager->viewProfile( mViewProfileId );

    QList<ByteArrayViewProfile> viewProfiles;
    viewProfiles << viewProfile;
    mViewProfileManager->saveViewProfiles( viewProfiles );
}

void
ByteArrayViewProfileSynchronizer::syncFromRemote()
{
    if( (mView == 0) || (mViewProfileId.isEmpty()) )
        return;

    const ByteArrayViewProfile viewProfile = mViewProfileManager->viewProfile( mViewProfileId );
    // TODO: this is a lazy hack, simply sets all of the profile, even if not needed. Improve that.
    mDirtyFlags = 0;
    updateView( viewProfile );

    if( mView )
        emit localSyncStateChanged( LocalInSync );
}


void
ByteArrayViewProfileSynchronizer::onViewProfilesChanged( const QList<ByteArrayViewProfile>& viewProfiles )
{
    if( mView == 0 )
        return;

    foreach( const ByteArrayViewProfile& viewProfile, viewProfiles )
    {
        if( viewProfile.id() == mViewProfileId )
        {
            updateView( viewProfile );
            break;
        }
    }
}

void ByteArrayViewProfileSynchronizer::updateView( const ByteArrayViewProfile& viewProfile )
{
    if( ! mView )
        return;

    mUpdatingView = true;

    if( (mDirtyFlags&ShowsNonprintingChanged) == 0 )
        mView->setShowsNonprinting( viewProfile.showsNonprinting() );

    if( (mDirtyFlags&ValueCodingChanged) == 0 )
        mView->setValueCoding( viewProfile.valueCoding() );

    if( (mDirtyFlags&CharCodecChanged) == 0 )
        mView->setCharCoding( viewProfile.charCodingName() );

    if( (mDirtyFlags&SubstituteCharChanged) == 0 )
        mView->setSubstituteChar( viewProfile.substituteChar() );

    if( (mDirtyFlags&UndefinedCharChanged) == 0 )
        mView->setUndefinedChar( viewProfile.undefinedChar() );

    if( (mDirtyFlags&VisibleByteArrayCodingsChanged) == 0 )
        mView->setVisibleByteArrayCodings( viewProfile.visibleByteArrayCodings() );

    if( (mDirtyFlags&OffsetColumnVisibleChanged) == 0 )
        mView->toggleOffsetColumn( viewProfile.offsetColumnVisible() );

    if( (mDirtyFlags&NoOfBytesPerLineChanged) == 0 )
        mView->setNoOfBytesPerLine( viewProfile.noOfBytesPerLine() );

    if( (mDirtyFlags&NoOfGroupedBytesChanged) == 0 )
        mView->setNoOfGroupedBytes( viewProfile.noOfGroupedBytes() );

    if( (mDirtyFlags&LayoutStyleChanged) == 0 )
        mView->setLayoutStyle( viewProfile.layoutStyle() );

    if( (mDirtyFlags&ViewModusChanged) == 0 )
        mView->setViewModus( viewProfile.viewModus() );

    mUpdatingView = false;
}

void ByteArrayViewProfileSynchronizer::onViewProfilesRemoved( const QList<ByteArrayViewProfile::Id>& viewProfileIds )
{
    if( mView == 0 )
        return;

    foreach( const ByteArrayViewProfile::Id& viewProfileId, viewProfileIds )
    {
        if( viewProfileId == mViewProfileId )
        {
            // TODO: really forget binding to that profile completely? cannot reappear?
            setViewProfileId( ByteArrayViewProfile::Id() );
            break;
        }
    }
}

void
ByteArrayViewProfileSynchronizer::setDirtyFlag( int dirtyFlag )
{
    if( mUpdatingView )
        return;

    const bool isCleanBefore = ( mDirtyFlags == 0 );

    mDirtyFlags |= dirtyFlag;

    if( isCleanBefore )
        emit localSyncStateChanged( LocalHasChanges );
}

void ByteArrayViewProfileSynchronizer::onShowsNonprintingChanged()
{
    setDirtyFlag( ShowsNonprintingChanged );
}
void ByteArrayViewProfileSynchronizer::onValueCodingChanged()
{
    setDirtyFlag( ValueCodingChanged );
}
void ByteArrayViewProfileSynchronizer::onCharCodecChanged()
{
    setDirtyFlag( CharCodecChanged );
}
void ByteArrayViewProfileSynchronizer::onSubstituteCharChanged()
{
    setDirtyFlag( SubstituteCharChanged );
}
void ByteArrayViewProfileSynchronizer::onUndefinedCharChanged()
{
    setDirtyFlag( UndefinedCharChanged );
}
void ByteArrayViewProfileSynchronizer::onVisibleByteArrayCodingsChanged()
{
    setDirtyFlag( VisibleByteArrayCodingsChanged );
}
void ByteArrayViewProfileSynchronizer::onOffsetColumnVisibleChanged()
{
    setDirtyFlag( OffsetColumnVisibleChanged );
}
void ByteArrayViewProfileSynchronizer::onNoOfBytesPerLineChanged()
{
    setDirtyFlag( NoOfBytesPerLineChanged );
}
void ByteArrayViewProfileSynchronizer::onNoOfGroupedBytesChanged()
{
    setDirtyFlag( NoOfGroupedBytesChanged );
}
void ByteArrayViewProfileSynchronizer::onLayoutStyleChanged()
{
    setDirtyFlag( LayoutStyleChanged );
}
void ByteArrayViewProfileSynchronizer::onViewModusChanged()
{
    setDirtyFlag( ViewModusChanged );
}

}
