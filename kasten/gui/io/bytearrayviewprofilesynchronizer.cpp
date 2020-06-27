/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayviewprofilesynchronizer.hpp"

// library
#include <bytearrayviewprofilemanager.hpp>
#include <bytearrayview.hpp>

namespace Kasten {

ByteArrayViewProfileSynchronizer::ByteArrayViewProfileSynchronizer(ByteArrayViewProfileManager* viewProfileManager)
    : mViewProfileManager(viewProfileManager)
{
}

LocalSyncState
ByteArrayViewProfileSynchronizer::localSyncState() const
{
    return (mDirtyFlags == 0) ? LocalInSync : LocalHasChanges;
}

void
ByteArrayViewProfileSynchronizer::setView(ByteArrayView* view)
{
    if (mView && !mViewProfileId.isEmpty()) {
        mView->disconnect(this);
    }

    mView = view;

    // TODO: drop all changes and set view to profile if set
    mDirtyFlags = 0;

    if (mView && !mViewProfileId.isEmpty()) {
        const ByteArrayViewProfile viewProfile = mViewProfileManager->viewProfile(mViewProfileId);
        updateView(viewProfile);
        connectViewSignals();
    }
}

void ByteArrayViewProfileSynchronizer::setViewProfileId(const ByteArrayViewProfile::Id& viewProfileId)
{
    if (mViewProfileId == viewProfileId) {
        return;
    }

    const bool isListeningBefore = (!mViewProfileId.isEmpty());

    mViewProfileId = viewProfileId;

    // TODO: drop all changes and set view to profile if set
    mDirtyFlags = 0;
    if (!viewProfileId.isEmpty()) {
        const ByteArrayViewProfile viewProfile = mViewProfileManager->viewProfile(viewProfileId);
        updateView(viewProfile);
    }

    const bool isListeningAfter = (!mViewProfileId.isEmpty());

    if (isListeningAfter) {
        if (!isListeningBefore) {
            connect(mViewProfileManager, &ByteArrayViewProfileManager::viewProfilesChanged,
                    this, &ByteArrayViewProfileSynchronizer::onViewProfilesChanged);
            connect(mViewProfileManager, &ByteArrayViewProfileManager::viewProfilesRemoved,
                    this, &ByteArrayViewProfileSynchronizer::onViewProfilesRemoved);
            if (mView) {
                connectViewSignals();
            }
        }
    } else {
        if (isListeningBefore) {
            mViewProfileManager->disconnect(this);
            if (mView) {
                mView->disconnect(this);
            }
        }
    }

    emit viewProfileChanged(mViewProfileId);
    if (mView) {
        emit localSyncStateChanged(LocalInSync);
    }
}

void
ByteArrayViewProfileSynchronizer::syncToRemote()
{
    if ((!mView) || (mViewProfileId.isEmpty())) {
        return;
    }

    ByteArrayViewProfile viewProfile = mViewProfileManager->viewProfile(mViewProfileId);
    updateViewProfile(viewProfile);

    mDirtyFlags = 0;

    QVector<ByteArrayViewProfile> viewProfiles {
        viewProfile
    };
    mViewProfileManager->saveViewProfiles(viewProfiles);

    if (mView) {
        emit localSyncStateChanged(LocalInSync);
    }
}

void
ByteArrayViewProfileSynchronizer::syncFromRemote()
{
    if ((!mView) || (mViewProfileId.isEmpty())) {
        return;
    }

    const ByteArrayViewProfile viewProfile = mViewProfileManager->viewProfile(mViewProfileId);
    // TODO: this is a lazy hack, simply sets all of the profile, even if not needed. Improve that.
    mDirtyFlags = 0;
    updateView(viewProfile);

    if (mView) {
        emit localSyncStateChanged(LocalInSync);
    }
}

void
ByteArrayViewProfileSynchronizer::onViewProfilesChanged(const QVector<ByteArrayViewProfile>& viewProfiles)
{
    if (!mView) {
        return;
    }

    for (const ByteArrayViewProfile& viewProfile : viewProfiles) {
        if (viewProfile.id() == mViewProfileId) {
            updateView(viewProfile);
            break;
        }
    }
}

void ByteArrayViewProfileSynchronizer::updateView(const ByteArrayViewProfile& viewProfile)
{
    if (!mView) {
        return;
    }

    mUpdatingView = true;

    if ((mDirtyFlags & ShowsNonprintingChanged) == 0) {
        mView->setShowsNonprinting(viewProfile.showsNonprinting());
    }

    if ((mDirtyFlags & OffsetCodingChanged) == 0) {
        mView->setOffsetCoding(viewProfile.offsetCoding());
    }

    if ((mDirtyFlags & ValueCodingChanged) == 0) {
        mView->setValueCoding(viewProfile.valueCoding());
    }

    if ((mDirtyFlags & CharCodecChanged) == 0) {
        mView->setCharCoding(viewProfile.charCodingName());
    }

    if ((mDirtyFlags & SubstituteCharChanged) == 0) {
        mView->setSubstituteChar(viewProfile.substituteChar());
    }

    if ((mDirtyFlags & UndefinedCharChanged) == 0) {
        mView->setUndefinedChar(viewProfile.undefinedChar());
    }

    if ((mDirtyFlags & VisibleByteArrayCodingsChanged) == 0) {
        mView->setVisibleByteArrayCodings(viewProfile.visibleByteArrayCodings());
    }

    if ((mDirtyFlags & OffsetColumnVisibleChanged) == 0) {
        mView->toggleOffsetColumn(viewProfile.offsetColumnVisible());
    }

    if ((mDirtyFlags & NoOfBytesPerLineChanged) == 0) {
        mView->setNoOfBytesPerLine(viewProfile.noOfBytesPerLine());
    }

    if ((mDirtyFlags & NoOfGroupedBytesChanged) == 0) {
        mView->setNoOfGroupedBytes(viewProfile.noOfGroupedBytes());
    }

    if ((mDirtyFlags & LayoutStyleChanged) == 0) {
        mView->setLayoutStyle(viewProfile.layoutStyle());
    }

    if ((mDirtyFlags & ViewModusChanged) == 0) {
        mView->setViewModus(viewProfile.viewModus());
    }

    mUpdatingView = false;
}

void ByteArrayViewProfileSynchronizer::updateViewProfile(ByteArrayViewProfile& viewProfile)
{
    if (!mView) {
        return;
    }

    if ((mDirtyFlags & ShowsNonprintingChanged)) {
        viewProfile.setShowsNonprinting(mView->showsNonprinting());
    }

    if ((mDirtyFlags & OffsetCodingChanged)) {
        viewProfile.setOffsetCoding(mView->offsetCoding());
    }

    if ((mDirtyFlags & ValueCodingChanged)) {
        viewProfile.setValueCoding(mView->valueCoding());
    }

    if ((mDirtyFlags & CharCodecChanged)) {
        viewProfile.setCharCoding(mView->charCodingName());
    }

    if ((mDirtyFlags & SubstituteCharChanged)) {
        viewProfile.setSubstituteChar(mView->substituteChar());
    }

    if ((mDirtyFlags & UndefinedCharChanged)) {
        viewProfile.setUndefinedChar(mView->undefinedChar());
    }

    if ((mDirtyFlags & VisibleByteArrayCodingsChanged)) {
        viewProfile.setVisibleByteArrayCodings(mView->visibleByteArrayCodings());
    }

    if ((mDirtyFlags & OffsetColumnVisibleChanged)) {
        viewProfile.setOffsetColumnVisible(mView->offsetColumnVisible());
    }

    if ((mDirtyFlags & NoOfBytesPerLineChanged)) {
        viewProfile.setNoOfBytesPerLine(mView->noOfBytesPerLine());
    }

    if ((mDirtyFlags & NoOfGroupedBytesChanged)) {
        viewProfile.setNoOfGroupedBytes(mView->noOfGroupedBytes());
    }

    if ((mDirtyFlags & LayoutStyleChanged)) {
        viewProfile.setLayoutStyle(mView->layoutStyle());
    }

    if ((mDirtyFlags & ViewModusChanged)) {
        viewProfile.setViewModus(mView->viewModus());
    }
}

void ByteArrayViewProfileSynchronizer::connectViewSignals()
{
    connect(mView, &ByteArrayView::showsNonprintingChanged,
            this, &ByteArrayViewProfileSynchronizer::onShowsNonprintingChanged);
    connect(mView, &ByteArrayView::offsetCodingChanged,
            this, &ByteArrayViewProfileSynchronizer::onOffsetCodingChanged);
    connect(mView, &ByteArrayView::valueCodingChanged,
            this, &ByteArrayViewProfileSynchronizer::onValueCodingChanged);
    connect(mView, &ByteArrayView::charCodecChanged,
            this, &ByteArrayViewProfileSynchronizer::onCharCodecChanged);
    connect(mView, &ByteArrayView::substituteCharChanged,
            this, &ByteArrayViewProfileSynchronizer::onSubstituteCharChanged);
    connect(mView, &ByteArrayView::undefinedCharChanged,
            this, &ByteArrayViewProfileSynchronizer::onUndefinedCharChanged);
    connect(mView, &ByteArrayView::visibleByteArrayCodingsChanged,
            this, &ByteArrayViewProfileSynchronizer::onVisibleByteArrayCodingsChanged);
    connect(mView, &ByteArrayView::offsetColumnVisibleChanged,
            this, &ByteArrayViewProfileSynchronizer::onOffsetColumnVisibleChanged);
    connect(mView, &ByteArrayView::noOfBytesPerLineChanged,
            this, &ByteArrayViewProfileSynchronizer::onNoOfBytesPerLineChanged);
    connect(mView, &ByteArrayView::noOfGroupedBytesChanged,
            this, &ByteArrayViewProfileSynchronizer::onNoOfGroupedBytesChanged);
    connect(mView, &ByteArrayView::layoutStyleChanged,
            this, &ByteArrayViewProfileSynchronizer::onLayoutStyleChanged);
    connect(mView, &ByteArrayView::viewModusChanged,
            this, &ByteArrayViewProfileSynchronizer::onViewModusChanged);
}

void ByteArrayViewProfileSynchronizer::onViewProfilesRemoved(const QVector<ByteArrayViewProfile::Id>& viewProfileIds)
{
    if (!mView) {
        return;
    }

    for (const ByteArrayViewProfile::Id& viewProfileId : viewProfileIds) {
        if (viewProfileId == mViewProfileId) {
            // TODO: really forget binding to that profile completely? cannot reappear?
            setViewProfileId(ByteArrayViewProfile::Id());
            break;
        }
    }
}

void
ByteArrayViewProfileSynchronizer::setDirtyFlag(int dirtyFlag)
{
    if (mUpdatingView) {
        return;
    }

    const bool isCleanBefore = (mDirtyFlags == 0);

    mDirtyFlags |= dirtyFlag;

    if (isCleanBefore) {
        emit localSyncStateChanged(LocalHasChanges);
    }
}

void ByteArrayViewProfileSynchronizer::onShowsNonprintingChanged()
{
    setDirtyFlag(ShowsNonprintingChanged);
}
void ByteArrayViewProfileSynchronizer::onOffsetCodingChanged()
{
    setDirtyFlag(OffsetCodingChanged);
}
void ByteArrayViewProfileSynchronizer::onValueCodingChanged()
{
    setDirtyFlag(ValueCodingChanged);
}
void ByteArrayViewProfileSynchronizer::onCharCodecChanged()
{
    setDirtyFlag(CharCodecChanged);
}
void ByteArrayViewProfileSynchronizer::onSubstituteCharChanged()
{
    setDirtyFlag(SubstituteCharChanged);
}
void ByteArrayViewProfileSynchronizer::onUndefinedCharChanged()
{
    setDirtyFlag(UndefinedCharChanged);
}
void ByteArrayViewProfileSynchronizer::onVisibleByteArrayCodingsChanged()
{
    setDirtyFlag(VisibleByteArrayCodingsChanged);
}
void ByteArrayViewProfileSynchronizer::onOffsetColumnVisibleChanged()
{
    setDirtyFlag(OffsetColumnVisibleChanged);
}
void ByteArrayViewProfileSynchronizer::onNoOfBytesPerLineChanged()
{
    setDirtyFlag(NoOfBytesPerLineChanged);
}
void ByteArrayViewProfileSynchronizer::onNoOfGroupedBytesChanged()
{
    setDirtyFlag(NoOfGroupedBytesChanged);
}
void ByteArrayViewProfileSynchronizer::onLayoutStyleChanged()
{
    setDirtyFlag(LayoutStyleChanged);
}
void ByteArrayViewProfileSynchronizer::onViewModusChanged()
{
    setDirtyFlag(ViewModusChanged);
}

}
