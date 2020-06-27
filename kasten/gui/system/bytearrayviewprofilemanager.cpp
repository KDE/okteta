/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayviewprofilemanager.hpp"

// library
#include "bytearrayviewprofilelock.hpp"
#include <oktetakastengui.hpp>
// KF
#include <KConfigGroup>
#include <KConfig>
#include <KDirWatch>
// Qt
#include <QFileInfo>
#include <QDir>
#include <QUuid>

namespace Kasten {
QStringList viewProfileFileNameFilter() { return QStringList { QStringLiteral("*.obavp"), QStringLiteral("*.olock") }; }
inline QLatin1String viewProfileFileSuffix() { return QLatin1String(".obavp"); }
inline QLatin1String viewProfileDirSubPath() { return QLatin1String("/okteta/viewprofiles"); }
inline QLatin1String defaultViewProfileFileSubPath() { return QLatin1String("/okteta/defaultviewprofile"); }
static constexpr int DefaultNoOfBytesPerLine = 16;
static constexpr int DefaultNoOfBytesPerGroup = 4;
static constexpr int DefaultLayoutStyle = 0;
static constexpr int DefaultViewModus = 0;
static constexpr int DefaultVisibleByteArrayCodings = 3;
static constexpr int DefaultOffsetCoding = 0;
static constexpr int DefaultValueCoding = 0;
QString DefaultCharCoding() { return {}; }

static QVector<ByteArrayViewProfile::Id>
lockedViewProfileIds(const ByteArrayViewProfileFileInfoLookup& viewProfileFileInfoLookup)
{
    QVector<ByteArrayViewProfile::Id> result;

    ByteArrayViewProfileFileInfoLookup::ConstIterator end =
        viewProfileFileInfoLookup.constEnd();
    for (ByteArrayViewProfileFileInfoLookup::ConstIterator it =
             viewProfileFileInfoLookup.constBegin();
         it != end;
         ++it) {
        if (it.value().isLocked()) {
            result.append(it.key());
        }
    }

    return result;
}

static void
updateLockStatus(ByteArrayViewProfileFileInfoLookup& viewProfileFileInfoLookup,
                 const QVector<ByteArrayViewProfile::Id>& lockedViewProfileIds,
                 const QVector<ByteArrayViewProfile::Id>& unlockedViewProfileIds)
{
    if (lockedViewProfileIds.isEmpty() && unlockedViewProfileIds.isEmpty()) {
        return;
    }

    ByteArrayViewProfileFileInfoLookup::Iterator end =
        viewProfileFileInfoLookup.end();
    for (ByteArrayViewProfileFileInfoLookup::Iterator it = viewProfileFileInfoLookup.begin();
         it != end;
         ++it) {
        bool isLocked;

        if (lockedViewProfileIds.contains(it.key())) {
            isLocked = true;
        } else if (unlockedViewProfileIds.contains(it.key())) {
            isLocked = false;
        } else {
            continue;
        }

        it.value().setLocked(isLocked);
    }
}

static QString
defaultViewProfileFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + defaultViewProfileFileSubPath();
}

static QString
viewProfileFilePath(const ByteArrayViewProfile::Id& viewProfileId)
{
    return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
           + viewProfileDirSubPath() + QLatin1Char('/') + viewProfileId + viewProfileFileSuffix();
}

static QString
viewProfileFileName(const ByteArrayViewProfile::Id& viewProfileId)
{
    return viewProfileId + viewProfileFileSuffix();
}

// TODO: add global lock
// TODO: make calls async
// TODO: only load view profiles on demand
ByteArrayViewProfileManager::ByteArrayViewProfileManager()
{
    mViewProfileFileWatcher = new KDirWatch(this);
    connect(mViewProfileFileWatcher, &KDirWatch::dirty,
            this, &ByteArrayViewProfileManager::onViewProfilesFolderChanged);

    // get all folder where viewProfiles could be stored
    const QStringList dataFolderPaths =
        QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);

    for (const QString& dataFolderPath : dataFolderPaths) {
        const QString viewProfileFolderPath = dataFolderPath + viewProfileDirSubPath();
        // watch folder for changes
        mViewProfileFileWatcher->addDir(viewProfileFolderPath, KDirWatch::WatchDirOnly);

        // read current files
        onViewProfilesFolderChanged(viewProfileFolderPath);
    }

    // default view profile
    // While there is no proper config syncing offer in the used frameworks, use a
    // single file with the id as content as workaround and watch for it changing
    auto* defaultViewProfileWatcher = new KDirWatch(this);
    connect(defaultViewProfileWatcher, &KDirWatch::created,
            this, &ByteArrayViewProfileManager::onDefaultViewProfileChanged);
    connect(defaultViewProfileWatcher, &KDirWatch::dirty,
            this, &ByteArrayViewProfileManager::onDefaultViewProfileChanged);
    const QString _defaultViewProfileFilePath = defaultViewProfileFilePath();

    defaultViewProfileWatcher->addFile(_defaultViewProfileFilePath);

    onDefaultViewProfileChanged(_defaultViewProfileFilePath);

    // report any problems with existing view profiles?
}

ByteArrayViewProfileManager::~ByteArrayViewProfileManager() = default;

int
ByteArrayViewProfileManager::viewProfilesCount() const
{
    return mViewProfiles.count();
}

QVector<ByteArrayViewProfile>
ByteArrayViewProfileManager::viewProfiles() const
{
    return mViewProfiles;
}

ByteArrayViewProfile
ByteArrayViewProfileManager::viewProfile(const ByteArrayViewProfile::Id& viewProfileId) const
{
    ByteArrayViewProfile result;

    for (const ByteArrayViewProfile& viewProfile : mViewProfiles) {
        if (viewProfile.id() == viewProfileId) {
            result = viewProfile;
            break;
        }
    }

    return result;
}

ByteArrayViewProfile::Id
ByteArrayViewProfileManager::defaultViewProfileId() const
{
    return mDefaultViewProfileId;
}

ByteArrayViewProfile
ByteArrayViewProfileManager::defaultViewProfile() const
{
    return viewProfile(mDefaultViewProfileId);
}

bool
ByteArrayViewProfileManager::isViewProfileLocked(const ByteArrayViewProfile::Id& viewProfileId) const
{
    bool result = false;

    // search in all folders for the info
    for (const ByteArrayViewProfileFileInfoLookup& viewProfileFileInfoLookup : mViewProfileFileInfoLookupPerFolder) {
        ByteArrayViewProfileFileInfoLookup::ConstIterator it =
            viewProfileFileInfoLookup.find(viewProfileId);
        if (it != viewProfileFileInfoLookup.constEnd()) {
            result = it->isLocked();
            break;
        }
    }

    return result;
}

void ByteArrayViewProfileManager::saveViewProfiles(QVector<ByteArrayViewProfile>& viewProfiles)
{
    // TODO: do not save if locked by someone else -> needs passing of our lock? or just registering our own and check?
    // create and set unique id
    std::for_each(viewProfiles.begin(), viewProfiles.end(), [this](ByteArrayViewProfile& viewProfile) {
        const ByteArrayViewProfile::Id viewProfileId = viewProfile.id();

        bool needsId = true;
        if (!viewProfileId.isEmpty()) {
            // already existing?
            auto hasViewProfileId = [&viewProfileId] (const ByteArrayViewProfile& existingProfile) {
                return (viewProfileId == existingProfile.id());
            };
            if (std::any_of(mViewProfiles.constBegin(), mViewProfiles.constEnd(), hasViewProfileId)) {
                needsId = false;
            }
        }

        // set new uuid for non-existing
        if (needsId) {
            viewProfile.setId(QUuid::createUuid().toString());
        }

        saveViewProfile(viewProfile);
    });
}

void
ByteArrayViewProfileManager::removeViewProfiles(const QVector<ByteArrayViewProfile::Id>& viewProfileIds)
{
    for (const ByteArrayViewProfile::Id& viewProfileId : viewProfileIds) {
        removeViewProfile(viewProfileId);
    }
}

void
ByteArrayViewProfileManager::setDefaultViewProfile(const ByteArrayViewProfile::Id& viewProfileId)
{
    QFile defaultViewProfileFile(defaultViewProfileFilePath());
    defaultViewProfileFile.open(QIODevice::WriteOnly);

    defaultViewProfileFile.write(viewProfileId.toUtf8());
    defaultViewProfileFile.close();
}

ByteArrayViewProfileLock
ByteArrayViewProfileManager::createLock(const ByteArrayViewProfile::Id& viewProfileId)
{
    const QString viewProfileFilePath = filePathOfViewProfile(viewProfileId);

    return ByteArrayViewProfileLock(viewProfileFilePath, viewProfileId);
}

/*
   bool
   ByteArrayViewProfileManager::lockViewProfile( const Kasten::ByteArrayViewProfile::Id& viewProfileId )
   {
    bool isSuccessfull;

    const QString viewProfileFilePath = filePathOfViewProfile( viewProfileId );

    // viewProfile known
    if( not viewProfileFilePath.isEmpty() )
    {
        const QString lockFilePath = viewProfileFileLockPath( viewProfileFilePath );
        KLockFile::Ptr lock = new KLockFile( lockFilePath );
        const KLockFile::LockResult lockResult =
            lock->lock(KLockFile::NoBlockFlag | KLockFile::ForceFlag);
        isSuccessfull = (lockResult == KLockFile::LockOK );
    }
    // if found
    // try to create lock file
    return isSuccessfull;
   }
*/
// bool
// ByteArrayViewProfileManager::unlockViewProfile( const Kasten::ByteArrayViewProfile::Id& viewProfileId )
// {
//     const QString filePath = filePathOfViewProfile( viewProfileId );
//
//     if( filePath.isEmpty() )
//     return false;
// }

ByteArrayViewProfile
ByteArrayViewProfileManager::loadViewProfile(const QString& absoluteFilePath) const
{
    ByteArrayViewProfile result;

    KConfig configFile(absoluteFilePath, KConfig::SimpleConfig);

    // check version
    KConfigGroup formatConfigGroup = configFile.group("OBAVP");
    const QString formatVersion = formatConfigGroup.readEntry("Version");
    if (!formatVersion.startsWith(QLatin1String("1."))) {
        return result;
    }

    result.setId(QFileInfo(absoluteFilePath).baseName());

    KConfigGroup generalConfigGroup = configFile.group("General");
    result.setViewProfileTitle(generalConfigGroup.readEntry("Title"));

    KConfigGroup layoutConfigGroup = configFile.group("Layout");
    result.setNoOfBytesPerLine(layoutConfigGroup.readEntry("NoOfBytesPerLine", DefaultNoOfBytesPerLine));
    result.setNoOfGroupedBytes(layoutConfigGroup.readEntry("NoOfBytesPerGroup", DefaultNoOfBytesPerGroup));
    result.setLayoutStyle(layoutConfigGroup.readEntry("LayoutStyle", DefaultLayoutStyle));

    KConfigGroup displayConfigGroup = configFile.group("Display");
    result.setOffsetColumnVisible(displayConfigGroup.readEntry("OffsetColumnVisible", true));
    result.setOffsetCoding(displayConfigGroup.readEntry("OffsetCoding", DefaultOffsetCoding));
    result.setViewModus(displayConfigGroup.readEntry("ViewModus", DefaultViewModus));
    result.setVisibleByteArrayCodings(displayConfigGroup.readEntry("VisibleByteArrayCodings", DefaultVisibleByteArrayCodings));

    KConfigGroup interpretationConfigGroup = configFile.group("Interpretation");

    KConfigGroup valuesConfigGroup = interpretationConfigGroup.group("Values");
    result.setValueCoding(valuesConfigGroup.readEntry("Coding", DefaultValueCoding));

    KConfigGroup charsConfigGroup = interpretationConfigGroup.group("Chars");
    result.setCharCoding(charsConfigGroup.readEntry("Coding", DefaultCharCoding()));
    result.setShowsNonprinting(charsConfigGroup.readEntry("NonprintingShown", false));
    result.setSubstituteChar(charsConfigGroup.readEntry("SubstituteChar", ".").at(0));
    result.setUndefinedChar(charsConfigGroup.readEntry("UndefinedChar", "?").at(0));

    return result;
}

void
ByteArrayViewProfileManager::saveViewProfile(const ByteArrayViewProfile& viewProfile) const
{
    const QString fileName = viewProfileFilePath(viewProfile.id());
    KConfig configFile(fileName, KConfig::SimpleConfig);

    KConfigGroup formatConfigGroup = configFile.group("OBAVP");
    formatConfigGroup.writeEntry("Version", "1.1");

    KConfigGroup generalConfigGroup = configFile.group("General");
    generalConfigGroup.writeEntry("Title", viewProfile.viewProfileTitle());

    KConfigGroup layoutConfigGroup = configFile.group("Layout");
    layoutConfigGroup.writeEntry("NoOfBytesPerLine", viewProfile.noOfBytesPerLine());
    layoutConfigGroup.writeEntry("NoOfBytesPerGroup", viewProfile.noOfGroupedBytes());
    layoutConfigGroup.writeEntry("LayoutStyle", viewProfile.layoutStyle());

    KConfigGroup displayConfigGroup = configFile.group("Display");
    displayConfigGroup.writeEntry("OffsetColumnVisible", viewProfile.offsetColumnVisible());
    displayConfigGroup.writeEntry("OffsetCoding", viewProfile.offsetCoding());
    displayConfigGroup.writeEntry("ViewModus", viewProfile.viewModus());
    displayConfigGroup.writeEntry("VisibleByteArrayCodings", viewProfile.visibleByteArrayCodings());

    KConfigGroup interpretationConfigGroup = configFile.group("Interpretation");

    KConfigGroup valuesConfigGroup = interpretationConfigGroup.group("Values");
    valuesConfigGroup.writeEntry("Coding", viewProfile.valueCoding());

    KConfigGroup charsConfigGroup = interpretationConfigGroup.group("Chars");
    charsConfigGroup.writeEntry("Coding", viewProfile.charCodingName());
    charsConfigGroup.writeEntry("NonprintingShown", viewProfile.showsNonprinting());
    charsConfigGroup.writeEntry("SubstituteChar", QString(viewProfile.substituteChar()));
    charsConfigGroup.writeEntry("UndefinedChar", QString(viewProfile.undefinedChar()));
}

void
ByteArrayViewProfileManager::removeViewProfile(const ByteArrayViewProfile::Id& viewProfileId)
{
    const QString filePath = filePathOfViewProfile(viewProfileId);
    if (!filePath.isEmpty()) {
        QFile::remove(filePath);
    }
}

QString
ByteArrayViewProfileManager::filePathOfViewProfile(const ByteArrayViewProfile::Id& viewProfileId) const
{
    QString result;

    for (QHash<QString, ByteArrayViewProfileFileInfoLookup>::ConstIterator foldersIt =
             mViewProfileFileInfoLookupPerFolder.constBegin();
         foldersIt != mViewProfileFileInfoLookupPerFolder.constEnd() && result.isEmpty();
         ++foldersIt) {
        const ByteArrayViewProfileFileInfoLookup& fileInfoList = foldersIt.value();
        for (ByteArrayViewProfileFileInfoLookup::ConstIterator folderIt = fileInfoList.constBegin();
             folderIt != fileInfoList.constEnd();
             ++folderIt) {
            if (folderIt.key() == viewProfileId) {
                result = foldersIt.key() + QLatin1Char('/') + viewProfileFileName(viewProfileId);
                break;
            }
        }
    }

    return result;
}

void
ByteArrayViewProfileManager::onViewProfilesFolderChanged(const QString& viewProfileFolderPath)
{
    ByteArrayViewProfileFileInfoLookup& viewProfileFileInfoLookup =
        mViewProfileFileInfoLookupPerFolder[viewProfileFolderPath];

    // TODO: reparse for new, removed and changed files
    // assume all are removed and unlocked in the beginning
    QVector<ByteArrayViewProfile::Id> removedViewProfileIds = viewProfileFileInfoLookup.keys().toVector();
    QVector<ByteArrayViewProfile> newViewProfiles;
    QVector<ByteArrayViewProfile> changedViewProfiles;

    QVector<ByteArrayViewProfile::Id> newUnlockedViewProfileIds = lockedViewProfileIds(viewProfileFileInfoLookup);
    QVector<ByteArrayViewProfile::Id> newLockedViewProfileIds;
    // iterate all files in folder
    const QFileInfoList viewProfileFileInfoList =
        QDir(viewProfileFolderPath).entryInfoList(viewProfileFileNameFilter(), QDir::Files);

    for (const QFileInfo& viewProfileFileInfo : viewProfileFileInfoList) {
        // a lock file ?
        if (viewProfileFileInfo.suffix() == QLatin1String("olock")) {
            const ByteArrayViewProfile::Id lockedViewProfileId = viewProfileFileInfo.baseName();
            // if not in old locks, is a new lock
            if (!newUnlockedViewProfileIds.removeOne(lockedViewProfileId)) {
                newLockedViewProfileIds.append(lockedViewProfileId);
            }
            continue;
        }

        // not a viewprofile file ?
        if (viewProfileFileInfo.suffix() != QLatin1String("obavp")) {
            continue;
        }

        // all other files assumed to be viewProfile files
        const ByteArrayViewProfile::Id viewProfileId = viewProfileFileInfo.baseName();
        // load file
        const ByteArrayViewProfile viewProfile = loadViewProfile(viewProfileFileInfo.absoluteFilePath());
        // loading failed? Treat as not existing
        if (viewProfile.id().isEmpty()) {
            continue;
        }

        const ByteArrayViewProfileFileInfoLookup::Iterator infoIt =
            viewProfileFileInfoLookup.find(viewProfileId);
        const bool isKnown = (infoIt != viewProfileFileInfoLookup.end());
        const QDateTime fileInfoLastModified = viewProfileFileInfo.lastModified();
        // is known?
        if (isKnown) {
            removedViewProfileIds.removeOne(viewProfileId);

            // check timestamp
            if (fileInfoLastModified == infoIt->lastModified()) {
                continue;
            }

            // update timestamp
            infoIt->setLastModified(fileInfoLastModified);
        } else {
            ByteArrayViewProfileFileInfo info(fileInfoLastModified, false);
            viewProfileFileInfoLookup.insert(viewProfileId, info);
        }

        if (isKnown) {
            auto it = std::find_if(mViewProfiles.begin(), mViewProfiles.end(),
                                   [&viewProfileId](const ByteArrayViewProfile& existingProfile) {
                return (existingProfile.id() == viewProfileId);
            });
            if (it != mViewProfiles.end()) {
                *it = viewProfile;
            }
        } else {
            newViewProfiles.append(viewProfile);
        }
        changedViewProfiles.append(viewProfile);
    }

    // remove all removed viewprofiles
    {
        auto isProfileToRemove = [&removedViewProfileIds](const ByteArrayViewProfile& profile) {
            return removedViewProfileIds.contains(profile.id());
        };
        mViewProfiles.erase(std::remove_if(mViewProfiles.begin(), mViewProfiles.end(), isProfileToRemove),
                            mViewProfiles.end());
    }

    for (const ByteArrayViewProfile::Id& viewProfileId : qAsConst(removedViewProfileIds)) {
        viewProfileFileInfoLookup.remove(viewProfileId);
        if (viewProfileId == mDefaultViewProfileId) {
            mDefaultViewProfileId.clear();
        }
        // TODO: how to select new one?
    }

    // add new viewprofiles
    mViewProfiles.append(newViewProfiles);
    // if there was no default viewprofile before, set default to first
    const bool isDefaultViewProfileChanged = (mDefaultViewProfileId.isEmpty() && !mViewProfiles.isEmpty());
    if (isDefaultViewProfileChanged) {
        mDefaultViewProfileId = mViewProfiles.at(0).id();
    }

    // update lock info
    updateLockStatus(viewProfileFileInfoLookup, newLockedViewProfileIds, newUnlockedViewProfileIds);

    // signal changes
    if (!changedViewProfiles.isEmpty()) {
        emit viewProfilesChanged(changedViewProfiles);
    }
    if (!removedViewProfileIds.isEmpty()) {
        emit viewProfilesRemoved(removedViewProfileIds);
    }

    if (!newUnlockedViewProfileIds.isEmpty()) {
        emit viewProfilesUnlocked(newUnlockedViewProfileIds);
    }
    if (!newLockedViewProfileIds.isEmpty()) {
        emit viewProfilesLocked(newLockedViewProfileIds);
    }
    if (isDefaultViewProfileChanged) {
        emit defaultViewProfileChanged(mDefaultViewProfileId);
    }
}

void ByteArrayViewProfileManager::onDefaultViewProfileChanged(const QString& path)
{
    QFile defaultViewProfileFile(path);
    if (!defaultViewProfileFile.open(QIODevice::ReadOnly)) {
        qCDebug(LOG_KASTEN_OKTETA_GUI) << "Failed to open view profiles file " << path;
        return;
    }

    const QByteArray fileContent = defaultViewProfileFile.readAll();
    const QString viewProfileId = QString::fromUtf8(fileContent);
    defaultViewProfileFile.close();

    // no id set?
    if (viewProfileId.isEmpty()) {
        return;
    }

    // no change?
    if (mDefaultViewProfileId == viewProfileId) {
        return;
    }

    bool isExisting = false;
    for (const ByteArrayViewProfile& viewProfile : qAsConst(mViewProfiles)) {
        if (viewProfile.id() == viewProfileId) {
            isExisting = true;
            break;
        }
    }

    if (isExisting) {
        mDefaultViewProfileId = viewProfileId;
        emit defaultViewProfileChanged(mDefaultViewProfileId);
    }
}

}
