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

#ifndef BYTEARRAYVIEWPROFILESYNCHRONIZER_H
#define BYTEARRAYVIEWPROFILESYNCHRONIZER_H

// lib
#include "oktetakastengui_export.h"
#include <bytearrayviewprofile.h>
// Kasten core
#include <kastencore.h>
// Qt Core
#include <QtCore/QObject>


namespace Kasten2
{

class ByteArrayViewProfileManager;
class ByteArrayView;

class OKTETAKASTENGUI_EXPORT ByteArrayViewProfileSynchronizer : public QObject
{
    Q_OBJECT

    enum DirtyFlags
    {
        ShowsNonprintingChanged = 1 << 0,
        ValueCodingChanged = 1 << 1,
        CharCodecChanged = 1 << 2,
        SubstituteCharChanged = 1 << 3,
        UndefinedCharChanged = 1 << 4,
        VisibleByteArrayCodingsChanged = 1 << 5,
        OffsetColumnVisibleChanged = 1 << 6,
        NoOfBytesPerLineChanged = 1 << 7,
        NoOfGroupedBytesChanged = 1 << 8,
        LayoutStyleChanged = 1 << 9,
        ViewModusChanged = 1 << 10
    };

  public:
    explicit ByteArrayViewProfileSynchronizer( ByteArrayViewProfileManager* viewProfileManager );

  public:
//     AbstractLoadJob *startLoad( const KUrl &url );
    void syncToRemote();
    void syncFromRemote();
//     AbstractConnectJob *startConnect( AbstractDocument* document,
//                                               const KUrl& url, AbstractModelSynchronizer::ConnectOption option );

    ByteArrayView* view() const;
    ByteArrayViewProfile::Id viewProfileId() const;

    void setView( ByteArrayView* view );
    void setViewProfileId( const ByteArrayViewProfile::Id& viewProfileId );

    LocalSyncState localSyncState() const;

  Q_SIGNALS:
    void localSyncStateChanged( Kasten2::LocalSyncState newState );
    void viewProfileChanged( const Kasten2::ByteArrayViewProfile::Id& viewProfileId );

  private Q_SLOTS:
    void onViewProfilesChanged( const QList<Kasten2::ByteArrayViewProfile>& viewProfiles );
    void onViewProfilesRemoved( const QList<Kasten2::ByteArrayViewProfile::Id>& viewProfileIds );

    // TODO: turn to one signal/slot with enum parameter for property and QVariant as new value
    void onShowsNonprintingChanged();
    void onValueCodingChanged();
    void onCharCodecChanged();
    void onSubstituteCharChanged();
    void onUndefinedCharChanged();
    void onVisibleByteArrayCodingsChanged();
    void onOffsetColumnVisibleChanged();
    void onNoOfBytesPerLineChanged();
    void onNoOfGroupedBytesChanged();
    void onLayoutStyleChanged();
    void onViewModusChanged();

  private:
    void updateView( const ByteArrayViewProfile& viewProfile );
    void setDirtyFlag( int dirtyFlag );

  private:
    ByteArrayView* mView;
    ByteArrayViewProfile::Id mViewProfileId;
    int mDirtyFlags;
    bool mUpdatingView;

    ByteArrayViewProfileManager* mViewProfileManager;
};


inline ByteArrayView* ByteArrayViewProfileSynchronizer::view() const { return mView; }
inline ByteArrayViewProfile::Id ByteArrayViewProfileSynchronizer::viewProfileId() const { return mViewProfileId; }

}


#endif
