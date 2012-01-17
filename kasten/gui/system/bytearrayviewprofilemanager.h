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

#ifndef BYTEARRAYVIEWPROFILEMANAGER_H
#define BYTEARRAYVIEWPROFILEMANAGER_H

// lib
#include "bytearrayviewprofile.h"
// Qt
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QDateTime>
#include <QtCore/QHash>

class KDirWatch;


namespace Kasten2
{
class ByteArrayViewProfileLock;


class ByteArrayViewProfileFileInfo
{
public:
    ByteArrayViewProfileFileInfo( const QDateTime& lastModified, bool  )
      : mLastModified( lastModified )
      , mLocked( false )
    {}

    const QDateTime& lastModified() const { return mLastModified; }
    bool isLocked()                 const { return mLocked; }

    void setLastModified( const QDateTime& lastModified )  { mLastModified = lastModified; }
    void setLocked( bool isLocked ) { mLocked = isLocked; }

private:
    QDateTime mLastModified;
    bool mLocked;
//     bool mUserOwned;
};

typedef QHash<ByteArrayViewProfile::Id, ByteArrayViewProfileFileInfo> ByteArrayViewProfileFileInfoLookup;


class OKTETAKASTENGUI_EXPORT ByteArrayViewProfileManager : public QObject
{
  Q_OBJECT

  public:
    ByteArrayViewProfileManager();
    virtual ~ByteArrayViewProfileManager();

  public:
    QList<ByteArrayViewProfile> viewProfiles() const;
    int viewProfilesCount() const;
    ByteArrayViewProfile viewProfile( const ByteArrayViewProfile::Id& id ) const;
    ByteArrayViewProfile::Id defaultViewProfileId() const;
    ByteArrayViewProfile defaultViewProfile() const;
    bool isViewProfileLocked( const ByteArrayViewProfile::Id& id ) const;

  public:
    void saveViewProfiles( QList<ByteArrayViewProfile>& viewProfiles );
    void removeViewProfiles( const QList<ByteArrayViewProfile::Id>& viewProfileIds );
    void setDefaultViewProfile( const ByteArrayViewProfile::Id& viewProfileId );

    ByteArrayViewProfileLock createLock( const ByteArrayViewProfile::Id& viewProfileId );

  Q_SIGNALS:
    void viewProfilesChanged( const QList<Kasten2::ByteArrayViewProfile>& viewProfiles );
    void viewProfilesRemoved( const QList<Kasten2::ByteArrayViewProfile::Id>& viewProfileIds );
    void defaultViewProfileChanged( const Kasten2::ByteArrayViewProfile::Id& viewProfileId );
    void viewProfilesLocked( const QList<Kasten2::ByteArrayViewProfile::Id>& viewProfileIds );
    void viewProfilesUnlocked( const QList<Kasten2::ByteArrayViewProfile::Id>& viewProfileIds );

  private:
    QString filePathOfViewProfile( const ByteArrayViewProfile::Id& viewProfileId ) const;
    ByteArrayViewProfile loadViewProfile( const QString& fileName ) const;
    void saveViewProfile( const ByteArrayViewProfile& viewProfile ) const;
    void removeViewProfile( const ByteArrayViewProfile::Id& viewProfileId );

  private Q_SLOTS:
    void onViewProfilesFolderChanged( const QString& path );

  private:
    QList<ByteArrayViewProfile> mViewProfiles;

    ByteArrayViewProfile::Id mDefaultViewProfileId;

    KDirWatch* mViewProfileFileWatcher;

    QHash<QString, ByteArrayViewProfileFileInfoLookup> mViewProfileFileInfoLookupPerFolder;
};

}

#endif
