/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVIEWPROFILESYNCHRONIZER_HPP
#define KASTEN_BYTEARRAYVIEWPROFILESYNCHRONIZER_HPP

// lib
#include "oktetakastengui_export.hpp"
#include "bytearrayviewprofile.hpp"
// Kasten core
#include <Kasten/KastenCore>
// Qt Core
#include <QObject>

namespace Kasten {

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
        ViewModusChanged = 1 << 10,
        OffsetCodingChanged = 1 << 11
    };

public:
    explicit ByteArrayViewProfileSynchronizer(ByteArrayViewProfileManager* viewProfileManager);

public:
//     AbstractLoadJob *startLoad( const QUrl &url );
    void syncToRemote();
    void syncFromRemote();
//     AbstractConnectJob *startConnect( AbstractDocument* document,
//                                               const QUrl& url, AbstractModelSynchronizer::ConnectOption option );

    ByteArrayView* view() const;
    ByteArrayViewProfile::Id viewProfileId() const;

    void setView(ByteArrayView* view);
    void setViewProfileId(const ByteArrayViewProfile::Id& viewProfileId);

    LocalSyncState localSyncState() const;

Q_SIGNALS:
    void localSyncStateChanged(Kasten::LocalSyncState newState);
    void viewProfileChanged(const Kasten::ByteArrayViewProfile::Id& viewProfileId);

private Q_SLOTS:
    OKTETAKASTENGUI_NO_EXPORT void onViewProfilesChanged(const QVector<Kasten::ByteArrayViewProfile>& viewProfiles);
    OKTETAKASTENGUI_NO_EXPORT void onViewProfilesRemoved(const QVector<Kasten::ByteArrayViewProfile::Id>& viewProfileIds);

    // TODO: turn to one signal/slot with enum parameter for property and QVariant as new value
    OKTETAKASTENGUI_NO_EXPORT void onShowsNonprintingChanged();
    OKTETAKASTENGUI_NO_EXPORT void onOffsetCodingChanged();
    OKTETAKASTENGUI_NO_EXPORT void onValueCodingChanged();
    OKTETAKASTENGUI_NO_EXPORT void onCharCodecChanged();
    OKTETAKASTENGUI_NO_EXPORT void onSubstituteCharChanged();
    OKTETAKASTENGUI_NO_EXPORT void onUndefinedCharChanged();
    OKTETAKASTENGUI_NO_EXPORT void onVisibleByteArrayCodingsChanged();
    OKTETAKASTENGUI_NO_EXPORT void onOffsetColumnVisibleChanged();
    OKTETAKASTENGUI_NO_EXPORT void onNoOfBytesPerLineChanged();
    OKTETAKASTENGUI_NO_EXPORT void onNoOfGroupedBytesChanged();
    OKTETAKASTENGUI_NO_EXPORT void onLayoutStyleChanged();
    OKTETAKASTENGUI_NO_EXPORT void onViewModusChanged();

private:
    OKTETAKASTENGUI_NO_EXPORT void updateView(const ByteArrayViewProfile& viewProfile);
    OKTETAKASTENGUI_NO_EXPORT void updateViewProfile(ByteArrayViewProfile& viewProfile);
    OKTETAKASTENGUI_NO_EXPORT void setDirtyFlag(int dirtyFlag);
    OKTETAKASTENGUI_NO_EXPORT void connectViewSignals();

private:
    ByteArrayViewProfileManager* const mViewProfileManager;

    ByteArrayView* mView = nullptr;
    ByteArrayViewProfile::Id mViewProfileId;
    int mDirtyFlags = 0;
    bool mUpdatingView = false;
};

inline ByteArrayView* ByteArrayViewProfileSynchronizer::view() const { return mView; }
inline ByteArrayViewProfile::Id ByteArrayViewProfileSynchronizer::viewProfileId() const { return mViewProfileId; }

}

#endif
