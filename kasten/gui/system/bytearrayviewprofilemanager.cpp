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

#include "bytearrayviewprofilemanager.h"

// library
#include "bytearrayviewprofilelock.h"
// KDE
#include <KGlobal>
#include <KStandardDirs>
#include <KConfigGroup>
#include <KConfig>
#include <KDirWatch>
#include <KLockFile>
// Qt
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QUuid>

#include <KDebug>

namespace Kasten2
{
static const QStringList viewProfileFileNameFilter =
    QStringList() << QLatin1String( "*.obavp" ) << QLatin1String( "*.olock" );
static const QString viewProfileFileSuffix = QLatin1String( ".obavp" );
static const QString viewProfileSubDir = QLatin1String( "okteta/viewprofiles" );
static const int DefaultNoOfBytesPerLine = 16;
static const int DefaultNoOfBytesPerGroup = 4;
static const int DefaultLayoutStyle = 0;
static const int DefaultViewModus = 0;
static const int DefaultVisibleByteArrayCodings = 3;
static const int DefaultValueCoding = 0;
static const QString DefaultCharCoding = QString();

static QList<ByteArrayViewProfile::Id>
viewProfileIds( const QList<ByteArrayViewProfile>& viewProfiles )
{
    QList<ByteArrayViewProfile::Id> result;
    result.reserve(viewProfiles.count());

    foreach( const ByteArrayViewProfile& viewProfile, viewProfiles )
        result.append( viewProfile.id() );

    return result;
}

static QList<ByteArrayViewProfile::Id>
lockedViewProfileIds( const ByteArrayViewProfileFileInfoLookup& viewProfileFileInfoLookup )
{
    QList<ByteArrayViewProfile::Id> result;

    ByteArrayViewProfileFileInfoLookup::ConstIterator end =
        viewProfileFileInfoLookup.constEnd();
    for( ByteArrayViewProfileFileInfoLookup::ConstIterator it =
                viewProfileFileInfoLookup.constBegin();
         it != end;
         ++it )
    {
        if( it.value().isLocked() )
            result.append( it.key() );
    }

    return result;
}

static void
updateLockStatus( ByteArrayViewProfileFileInfoLookup& viewProfileFileInfoLookup,
                  const QList<ByteArrayViewProfile::Id>& lockedViewProfileIds,
                  const QList<ByteArrayViewProfile::Id>& unlockedViewProfileIds )
{
    if( lockedViewProfileIds.isEmpty() && unlockedViewProfileIds.isEmpty() )
        return;

    ByteArrayViewProfileFileInfoLookup::Iterator end =
        viewProfileFileInfoLookup.end();
    for( ByteArrayViewProfileFileInfoLookup::Iterator it = viewProfileFileInfoLookup.begin();
         it != end;
         ++it )
    {
        bool isLocked;

        if( lockedViewProfileIds.contains(it.key()) )
            isLocked = true;
        else if( unlockedViewProfileIds.contains(it.key()) )
            isLocked = false;
        else
            continue;

        it.value().setLocked( isLocked );
    }
}

static QString
viewProfileFilePath( const ByteArrayViewProfile::Id& viewProfileId )
{
    return KGlobal::dirs()->saveLocation( "data", viewProfileSubDir ) + viewProfileId + viewProfileFileSuffix;
}

static QString
viewProfileFileName( const ByteArrayViewProfile::Id& viewProfileId )
{
    return viewProfileId + viewProfileFileSuffix;
}

// TODO: add global lock
// TODO: make calls async
// TODO: only load view profiles on demand
ByteArrayViewProfileManager::ByteArrayViewProfileManager()
{
    mViewProfileFileWatcher = new KDirWatch( this );
    connect( mViewProfileFileWatcher, SIGNAL(dirty(QString)),
             SLOT(onViewProfilesFolderChanged(QString)) );

    // get all folder where viewProfiles could be stored
    const KStandardDirs* const standardDirs = KGlobal::dirs();
    const QStringList dataFolderPaths = standardDirs->resourceDirs( "data" );

    foreach( const QString& dataFolderPath, dataFolderPaths )
    {
        const QString viewProfileFolderPath = dataFolderPath + viewProfileSubDir;
        // watch folder for changes
        mViewProfileFileWatcher->addDir( viewProfileFolderPath, KDirWatch::WatchDirOnly );
kDebug() << "adding Dir"<<viewProfileFolderPath;

        // read current files
        onViewProfilesFolderChanged( viewProfileFolderPath );
    }

    // tmp
    if( ! mViewProfiles.isEmpty() )
        mDefaultViewProfileId = mViewProfiles.at(0).id();

    // report any problems with existing view profiles?
}

int
ByteArrayViewProfileManager::viewProfilesCount() const
{
    return mViewProfiles.count();
}


QList<ByteArrayViewProfile>
ByteArrayViewProfileManager::viewProfiles() const
{
    return mViewProfiles;
}

ByteArrayViewProfile
ByteArrayViewProfileManager::viewProfile( const ByteArrayViewProfile::Id& viewProfileId ) const
{
    ByteArrayViewProfile result;

    foreach( const ByteArrayViewProfile& viewProfile, mViewProfiles )
    {
        if( viewProfile.id() == viewProfileId )
        {
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
    return viewProfile( mDefaultViewProfileId );
}

bool
ByteArrayViewProfileManager::isViewProfileLocked( const ByteArrayViewProfile::Id& viewProfileId ) const
{
    bool result = false;

    // search in all folders for the info
    foreach( const ByteArrayViewProfileFileInfoLookup& viewProfileFileInfoLookup, mViewProfileFileInfoLookupPerFolder )
    {
        ByteArrayViewProfileFileInfoLookup::ConstIterator it =
            viewProfileFileInfoLookup.find( viewProfileId );
        if( it != viewProfileFileInfoLookup.constEnd() )
        {
            result = it->isLocked();
            break;
        }
    }

    return result;
}

void ByteArrayViewProfileManager::saveViewProfiles( QList<ByteArrayViewProfile>& viewProfiles )
{
    //TODO: do not save if locked by someone else -> needs passing of our lock? or just registering our own and check?
    // create and set unique id
    for( QList<ByteArrayViewProfile>::Iterator it = viewProfiles.begin(); it != viewProfiles.end(); ++it )
    {
        ByteArrayViewProfile& viewProfile = *it;
        const ByteArrayViewProfile::Id viewProfileId = viewProfile.id();
        const ByteArrayViewProfile::Id oldViewProfileId = viewProfile.id();

        bool needsId = true;
        if( ! viewProfileId.isEmpty() )
        {
            // already existing?
            QList<ByteArrayViewProfile>::ConstIterator existingIt = mViewProfiles.constBegin();
            QList<ByteArrayViewProfile>::ConstIterator existingEnd = mViewProfiles.constEnd();
            for( ; existingIt != existingEnd; ++existingIt )
            {
                if( viewProfileId == existingIt->id() )
                {
                    needsId = false;
                    break;
                }
            }
        }

        // set new uuid for non-existing
        if( needsId )
            viewProfile.setId( QUuid::createUuid().toString() );

kDebug() << "going to save"<<viewProfile.viewProfileTitle()<<viewProfile.id()<<oldViewProfileId;
        saveViewProfile( viewProfile );
    }
}


void
ByteArrayViewProfileManager::removeViewProfiles( const QList<ByteArrayViewProfile::Id>& viewProfileIds )
{
    foreach( const ByteArrayViewProfile::Id& viewProfileId, viewProfileIds )
    {
        removeViewProfile( viewProfileId );
    }
}


void
ByteArrayViewProfileManager::setDefaultViewProfile( const ByteArrayViewProfile::Id& viewProfileId )
{
    // no change?
    if( mDefaultViewProfileId == viewProfileId )
        return;

    bool isExisting = false;
    foreach( const ByteArrayViewProfile& viewProfile, mViewProfiles )
    {
        if( viewProfile.id() == viewProfileId )
        {
            isExisting = true;
            break;
        }
    }

    if( isExisting )
    {
        mDefaultViewProfileId = viewProfileId;
        emit defaultViewProfileChanged( mDefaultViewProfileId );
    }
}

ByteArrayViewProfileLock
ByteArrayViewProfileManager::createLock( const ByteArrayViewProfile::Id& viewProfileId )
{
    const QString viewProfileFilePath = filePathOfViewProfile( viewProfileId );

    return ByteArrayViewProfileLock( viewProfileFilePath, viewProfileId );
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
ByteArrayViewProfileManager::loadViewProfile( const QString& absoluteFilePath ) const
{
kDebug() << "Loading" << QFileInfo(absoluteFilePath).baseName() << absoluteFilePath;
    ByteArrayViewProfile result;

    KConfig configFile( absoluteFilePath, KConfig::SimpleConfig );
    // TODO: version check

    result.setId( QFileInfo(absoluteFilePath).baseName() );

    KConfigGroup generalConfigGroup = configFile.group( "General" );
    result.setViewProfileTitle( generalConfigGroup.readEntry("Title") );

    KConfigGroup layoutConfigGroup = configFile.group( "Layout" );
    result.setNoOfBytesPerLine( layoutConfigGroup.readEntry("NoOfBytesPerLine", DefaultNoOfBytesPerLine) );
    result.setNoOfGroupedBytes( layoutConfigGroup.readEntry("NoOfBytesPerGroup", DefaultNoOfBytesPerGroup) );
    result.setLayoutStyle( layoutConfigGroup.readEntry("LayoutStyle", DefaultLayoutStyle) );

    KConfigGroup displayConfigGroup = configFile.group( "Display" );
    result.setOffsetColumnVisible( displayConfigGroup.readEntry("OffsetColumnVisible", true) );
    result.setViewModus( displayConfigGroup.readEntry("ViewModus", DefaultViewModus) );
    result.setVisibleByteArrayCodings( displayConfigGroup.readEntry("VisibleByteArrayCodings", DefaultVisibleByteArrayCodings) );

    KConfigGroup interpretationConfigGroup = configFile.group( "Interpretation" );
 
    KConfigGroup valuesConfigGroup = interpretationConfigGroup.group( "Values" );
    result.setValueCoding( valuesConfigGroup.readEntry("Coding", DefaultValueCoding) );

    KConfigGroup charsConfigGroup = interpretationConfigGroup.group( "Chars" );
    result.setCharCoding( charsConfigGroup.readEntry("Coding", DefaultCharCoding) );
    result.setShowsNonprinting( charsConfigGroup.readEntry("NonprintingShown", false) );
    result.setSubstituteChar( charsConfigGroup.readEntry("SubstituteChar", ".").at(0) );
    result.setUndefinedChar( charsConfigGroup.readEntry("UndefinedChar", "?").at(0) );

    return result;
}

void
ByteArrayViewProfileManager::saveViewProfile( const ByteArrayViewProfile& viewProfile ) const
{
kDebug() << "------------------ Saving"<<viewProfile.id();
{
    const QString fileName = viewProfileFilePath( viewProfile.id() );
    KConfig configFile( fileName, KConfig::SimpleConfig );

    KConfigGroup generalConfigGroup = configFile.group( "General" );
    generalConfigGroup.writeEntry( "Title", viewProfile.viewProfileTitle() );

    KConfigGroup layoutConfigGroup = configFile.group( "Layout" );
    layoutConfigGroup.writeEntry( "NoOfBytesPerLine", viewProfile.noOfBytesPerLine() );
    layoutConfigGroup.writeEntry( "NoOfBytesPerGroup", viewProfile.noOfGroupedBytes() );
    layoutConfigGroup.writeEntry( "LayoutStyle", viewProfile.layoutStyle() );

    KConfigGroup displayConfigGroup = configFile.group( "Display" );
    displayConfigGroup.writeEntry( "OffsetColumnVisible", viewProfile.offsetColumnVisible() );
    displayConfigGroup.writeEntry( "ViewModus", viewProfile.viewModus() );
    displayConfigGroup.writeEntry( "VisibleByteArrayCodings", viewProfile.visibleByteArrayCodings() );

    KConfigGroup interpretationConfigGroup = configFile.group( "Interpretation" );

    KConfigGroup valuesConfigGroup = interpretationConfigGroup.group( "Values" );
    valuesConfigGroup.writeEntry( "Coding", viewProfile.valueCoding() );

    KConfigGroup charsConfigGroup = interpretationConfigGroup.group( "Chars" );
    charsConfigGroup.writeEntry( "Coding", viewProfile.charCodingName() );
    charsConfigGroup.writeEntry( "NonprintingShown", viewProfile.showsNonprinting() );
    charsConfigGroup.writeEntry( "SubstituteChar", QString(viewProfile.substituteChar()) );
    charsConfigGroup.writeEntry( "UndefinedChar", QString(viewProfile.undefinedChar()) );
}
kDebug() << "------------------ Saved"<<viewProfile.id();
}

void
ByteArrayViewProfileManager::removeViewProfile( const ByteArrayViewProfile::Id& viewProfileId )
{
    const QString filePath = filePathOfViewProfile( viewProfileId );
kDebug() << "------------------ Removing"<<viewProfileId<<filePath;
    if( ! filePath.isEmpty() )
        QFile::remove( filePath );
}

QString
ByteArrayViewProfileManager::filePathOfViewProfile( const ByteArrayViewProfile::Id& viewProfileId ) const
{
    QString result;

    for( QHash<QString, ByteArrayViewProfileFileInfoLookup>::ConstIterator foldersIt =
             mViewProfileFileInfoLookupPerFolder.constBegin();
         foldersIt != mViewProfileFileInfoLookupPerFolder.constEnd() && result.isEmpty();
         ++foldersIt )
    {
        const ByteArrayViewProfileFileInfoLookup& fileInfoList = foldersIt.value();
        for( ByteArrayViewProfileFileInfoLookup::ConstIterator folderIt = fileInfoList.constBegin();
             folderIt != fileInfoList.constEnd();
             ++folderIt )
        {
            if( folderIt.key() == viewProfileId )
            {
                result = foldersIt.key() + QLatin1Char('/') + viewProfileFileName( viewProfileId );
                break;
            }
        }
    }

    return result;
}

void
ByteArrayViewProfileManager::onViewProfilesFolderChanged( const QString& viewProfileFolderPath )
{
kDebug() << "looking into folder"<< viewProfileFolderPath;
    ByteArrayViewProfileFileInfoLookup& viewProfileFileInfoLookup =
        mViewProfileFileInfoLookupPerFolder[viewProfileFolderPath];

    // TODO: reparse for new, removed and changed files
    // assume all are removed and unlocked in the beginning
    QList<ByteArrayViewProfile::Id> removedViewProfileIds = viewProfileFileInfoLookup.keys();
    QList<ByteArrayViewProfile> newViewProfiles;
    QList<ByteArrayViewProfile> changedViewProfiles;

    QList<ByteArrayViewProfile::Id> newUnlockedViewProfileIds = lockedViewProfileIds(viewProfileFileInfoLookup);
    QList<ByteArrayViewProfile::Id> newLockedViewProfileIds;
kDebug() << "old profiles:" << removedViewProfileIds;
kDebug() << "locked profiles:" << newUnlockedViewProfileIds;
    // iterate all files in folder
    const QFileInfoList viewProfileFileInfoList =
        QDir( viewProfileFolderPath ).entryInfoList( viewProfileFileNameFilter, QDir::Files );

    foreach( const QFileInfo& viewProfileFileInfo, viewProfileFileInfoList )
    {
       // a lock file ?
       if( viewProfileFileInfo.suffix() == QLatin1String("olock") )
       {
           const ByteArrayViewProfile::Id lockedViewProfileId = viewProfileFileInfo.baseName();
           // if not in old locks, is a new lock
           if( ! newUnlockedViewProfileIds.removeOne(lockedViewProfileId) )
                newLockedViewProfileIds.append( lockedViewProfileId );
           continue;
       }

       // not a viewprofile file ?
       if( viewProfileFileInfo.suffix() != QLatin1String("obavp") )
            continue;

       // all other files assumed to be viewProfile files
       // TODO: check validness
        const QDateTime fileInfoLastModified = viewProfileFileInfo.lastModified();
        const ByteArrayViewProfile::Id viewProfileId = viewProfileFileInfo.baseName();
        ByteArrayViewProfileFileInfoLookup::Iterator infoIt =
            viewProfileFileInfoLookup.find( viewProfileId );
        const bool isKnown = ( infoIt != viewProfileFileInfoLookup.end() );
        // is known?
        if( isKnown )
        {
            removedViewProfileIds.removeOne( viewProfileId );

            // check timestamp
            if( fileInfoLastModified == infoIt->lastModified() )
                continue;

            // update timestamp
            infoIt->setLastModified( fileInfoLastModified );
        }
        else
        {
            ByteArrayViewProfileFileInfo info( fileInfoLastModified, false );
            viewProfileFileInfoLookup.insert( viewProfileId, info );
        }

kDebug() <<"going to load"<<viewProfileId;
        // load file
        // TODO: check validness
        const ByteArrayViewProfile viewProfile = loadViewProfile( viewProfileFileInfo.absoluteFilePath() );
        if( isKnown )
        {
            QList<ByteArrayViewProfile>::Iterator it = mViewProfiles.begin();
            QList<ByteArrayViewProfile>::Iterator end = mViewProfiles.end();
            for( ; it != end; ++it )
            {
                if( it->id() == viewProfileId )
                {
                    *it = viewProfile;
                    break;
                }
            }
        }
        else
            newViewProfiles.append( viewProfile );
        changedViewProfiles.append( viewProfile );
    }

    // remove all removed viewprofiles
    {
        QList<ByteArrayViewProfile>::Iterator it = mViewProfiles.begin();
        while( it != mViewProfiles.end() )
        {
            // contained in removed?
            if( removedViewProfileIds.contains(it->id()) )
                it = mViewProfiles.erase(it);
            else
                ++it;
        }
    }
    foreach( const ByteArrayViewProfile::Id& viewProfileId, removedViewProfileIds )
    {
        viewProfileFileInfoLookup.remove( viewProfileId );
        if( viewProfileId == mDefaultViewProfileId )
            mDefaultViewProfileId.clear();
        // TODO: how to select new one?
    }

    // add new viewprofiles
    mViewProfiles.append( newViewProfiles );
    // update lock info
    updateLockStatus( viewProfileFileInfoLookup, newLockedViewProfileIds, newUnlockedViewProfileIds );

kDebug() << "new profiles:" << viewProfileFileInfoLookup.keys();

    // signal changes
    if( ! changedViewProfiles.isEmpty() )
        emit viewProfilesChanged( changedViewProfiles );
kDebug() << "changed profiles" << viewProfileIds(changedViewProfiles);
    if( ! removedViewProfileIds.isEmpty() )
        emit viewProfilesRemoved( removedViewProfileIds );
kDebug() << "removed profiles" << removedViewProfileIds;

    if( ! newUnlockedViewProfileIds.isEmpty() )
        emit viewProfilesUnlocked( newUnlockedViewProfileIds );
kDebug() << "unlocked profiles" << newUnlockedViewProfileIds;
    if( ! newLockedViewProfileIds.isEmpty() )
        emit viewProfilesLocked( newLockedViewProfileIds );
kDebug() << "locked profiles" << newLockedViewProfileIds;
}


ByteArrayViewProfileManager::~ByteArrayViewProfileManager()
{
}

}
