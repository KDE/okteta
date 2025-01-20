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
// Std
#include <unordered_map>

class KDirWatch;

namespace Kasten {
class ByteArrayViewProfileLock;

class ByteArrayViewProfileFileInfo
{
public:
    ByteArrayViewProfileFileInfo(const QDateTime& lastModified, bool locked);
    ByteArrayViewProfileFileInfo(const ByteArrayViewProfileFileInfo&) = default;

    ~ByteArrayViewProfileFileInfo() = default;

    ByteArrayViewProfileFileInfo& operator=(const ByteArrayViewProfileFileInfo&) = default;

public:
    const QDateTime& lastModified() const;
    bool isLocked() const;

    void setLastModified(const QDateTime& lastModified);
    void setLocked(bool isLocked);

private:
    QDateTime mLastModified;
    bool mLocked;
//     bool mUserOwned;
};

inline ByteArrayViewProfileFileInfo::ByteArrayViewProfileFileInfo(const QDateTime& lastModified, bool locked)
    : mLastModified(lastModified)
    , mLocked(locked)
{}

inline const QDateTime& ByteArrayViewProfileFileInfo::lastModified() const { return mLastModified; }
inline bool ByteArrayViewProfileFileInfo::isLocked()                 const { return mLocked; }

inline void ByteArrayViewProfileFileInfo::setLastModified(const QDateTime& lastModified)  { mLastModified = lastModified; }
inline void ByteArrayViewProfileFileInfo::setLocked(bool isLocked) { mLocked = isLocked; }


using ByteArrayViewProfileFileInfoLookup = std::unordered_map<ByteArrayViewProfile::Id, ByteArrayViewProfileFileInfo>;

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
    OKTETAKASTENGUI_NO_EXPORT QString filePathOfViewProfile(const ByteArrayViewProfile::Id& viewProfileId) const;
    // Returns the viewProfile as loaded from the file with the given absoluteFilePath.
    // If the loading fails the viewProfile has no id set.
    OKTETAKASTENGUI_NO_EXPORT ByteArrayViewProfile loadViewProfile(const QString& absoluteFilePath) const;
    OKTETAKASTENGUI_NO_EXPORT void saveViewProfile(const ByteArrayViewProfile& viewProfile) const;
    OKTETAKASTENGUI_NO_EXPORT void removeViewProfile(const ByteArrayViewProfile::Id& viewProfileId);

private Q_SLOTS:
    OKTETAKASTENGUI_NO_EXPORT void onViewProfilesFolderChanged(const QString& path);
    OKTETAKASTENGUI_NO_EXPORT void onDefaultViewProfileChanged(const QString& path);

private:
    QVector<ByteArrayViewProfile> mViewProfiles;

    ByteArrayViewProfile::Id mDefaultViewProfileId;

    KDirWatch* mViewProfileFileWatcher;

    std::unordered_map<QString, ByteArrayViewProfileFileInfoLookup> mViewProfileFileInfoLookupPerFolder;
};

}

#endif
