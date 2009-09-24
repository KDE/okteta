/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef ABSTRACTFILESYSTEMSYNCWITHREMOTEJOB_P_H
#define ABSTRACTFILESYSTEMSYNCWITHREMOTEJOB_P_H

// library
#include "abstractfilesystemsyncwithremotejob.h"
#include <abstractsyncwithremotejob_p.h>
// KDE
#include <KTemporaryFile>
#include <KUrl>
// Qt
#include <QtCore/QTimer>


namespace Kasten
{

class KASTENCORE_EXPORT AbstractFileSystemSyncWithRemoteJobPrivate : public AbstractSyncWithRemoteJobPrivate
{
  public:
    AbstractFileSystemSyncWithRemoteJobPrivate( AbstractFileSystemSyncWithRemoteJob* parent,
                                                AbstractModelFileSystemSynchronizer* synchronizer,
                                                const KUrl& url, AbstractModelSynchronizer::ConnectOption option );

    virtual ~AbstractFileSystemSyncWithRemoteJobPrivate();

  public: // KJob API
    void start();

  protected:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    const QString& workFilePath() const;
    QWidget* widget() const;

  protected:
    void completeSync( bool success );

  protected: // slots
    void syncWithRemote();

  protected:
    Q_DECLARE_PUBLIC( AbstractFileSystemSyncWithRemoteJob )

  protected:
    AbstractModelFileSystemSynchronizer* const mSynchronizer;
    const KUrl mUrl;
    const AbstractModelSynchronizer::ConnectOption mOption;
    KTemporaryFile* mTemporaryFile;
    QString mWorkFilePath;
};


inline AbstractFileSystemSyncWithRemoteJobPrivate::AbstractFileSystemSyncWithRemoteJobPrivate( AbstractFileSystemSyncWithRemoteJob* parent,
    AbstractModelFileSystemSynchronizer* synchronizer,
    const KUrl& url, AbstractModelSynchronizer::ConnectOption option )
  : AbstractSyncWithRemoteJobPrivate( parent ),
    mSynchronizer( synchronizer ),
    mUrl( url ),
    mOption( option ),
    mTemporaryFile( 0 )
{}

inline AbstractFileSystemSyncWithRemoteJobPrivate::~AbstractFileSystemSyncWithRemoteJobPrivate() {}

inline const QString& AbstractFileSystemSyncWithRemoteJobPrivate::workFilePath() const { return mWorkFilePath; }
// TODO: setup a notification system
inline QWidget* AbstractFileSystemSyncWithRemoteJobPrivate::widget()      const { return 0; }
inline AbstractModelFileSystemSynchronizer* AbstractFileSystemSyncWithRemoteJobPrivate::synchronizer() const
{
    return mSynchronizer;
}

inline void AbstractFileSystemSyncWithRemoteJobPrivate::start()
{
    Q_Q( AbstractFileSystemSyncWithRemoteJob );

    QTimer::singleShot( 0, q, SLOT(syncWithRemote()) );
}

}

#endif
