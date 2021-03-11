/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVIEWPROFILEMANAGER_HPP
#define KASTEN_BYTEARRAYVIEWPROFILEMANAGER_HPP

// lib
#include "bytearrayviewprofile.hpp"
// Qt
#include <QObject>
#include <QVector>
#include <QDateTime>
#include <QHash>

class KDirWatch;

namespace Kasten {
class ByteArrayViewProfileLock;

class ByteArrayViewProfileFileInfo
{
public:
    ByteArrayViewProfileFileInfo(const QDateTime& lastModified, bool locked)
        : mLastModified(lastModified)
        , mLocked(locked)
    {}

    const QDateTime& lastModified() const { return mLastModified; }
    bool isLocked()                 const { return mLocked; }

    void setLastModified(const QDateTime& lastModified)  { mLastModified = lastModified; }
    void setLocked(bool isLocked) { mLocked = isLocked; }

private:
    QDateTime mLastModified;
    bool mLocked;
//     bool mUserOwned;
};

using ByteArrayViewProfileFileInfoLookup = QHash<ByteArrayViewProfile::Id, ByteArrayViewProfileFileInfo>;

class OKTETAKASTENGUI_EXPORT ByteArrayViewProfileManager : public QObject
{
    Q_OBJECT

public:
    ByteArrayViewProfileManager();
    ~ByteArrayViewProfileManager() override;

public:
    QVector<ByteArrayViewProfile> viewProfiles() const;
    int viewProfilesCount() const;
    ByteArrayViewProfile viewProfile(const ByteArrayViewProfile::Id& id) const;
    ByteArrayViewProfile::Id defaultViewProfileId() const;
    ByteArrayViewProfile defaultViewProfile() const;
    bool isViewProfileLocked(const ByteArrayViewProfile::Id& id) const;

public:
    void saveViewProfiles(QVector<ByteArrayViewProfile>& viewProfiles);
    void removeViewProfiles(const QVector<ByteArrayViewProfile::Id>& viewProfileIds);
    void setDefaultViewProfile(const ByteArrayViewProfile::Id& viewProfileId);

    ByteArrayViewProfileLock createLock(const ByteArrayViewProfile::Id& viewProfileId);

Q_SIGNALS:
    void viewProfilesChanged(const QVector<Kasten::ByteArrayViewProfile>& viewProfiles);
    void viewProfilesRemoved(const QVector<Kasten::ByteArrayViewProfile::Id>& viewProfileIds);
    void defaultViewProfileChanged(const Kasten::ByteArrayViewProfile::Id& viewProfileId);
    void viewProfilesLocked(const QVector<Kasten::ByteArrayViewProfile::Id>& viewProfileIds);
    void viewProfilesUnlocked(const QVector<Kasten::ByteArrayViewProfile::Id>& viewProfileIds);

private:
    QString filePathOfViewProfile(const ByteArrayViewProfile::Id& viewProfileId) const;
    // Returns the viewProfile as loaded from the file with the given fileName.
    // If the loading fails the viewProfile has no id set.
    ByteArrayViewProfile loadViewProfile(const QString& fileName) const;
    void saveViewProfile(const ByteArrayViewProfile& viewProfile) const;
    void removeViewProfile(const ByteArrayViewProfile::Id& viewProfileId);

private Q_SLOTS:
    void onViewProfilesFolderChanged(const QString& path);
    void onDefaultViewProfileChanged(const QString& path);

private:
    QVector<ByteArrayViewProfile> mViewProfiles;

    ByteArrayViewProfile::Id mDefaultViewProfileId;

    KDirWatch* mViewProfileFileWatcher;

    QHash<QString, ByteArrayViewProfileFileInfoLookup> mViewProfileFileInfoLookupPerFolder;
};

}

#endif
