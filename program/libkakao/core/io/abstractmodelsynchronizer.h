/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef ABSTRACTMODELSYNCHRONIZER_H
#define ABSTRACTMODELSYNCHRONIZER_H

// Qt
#include <QtCore/QObject>

class AbstractLoadJob;
class AbstractConnectJob;
class AbstractSyncToRemoteJob;
class AbstractSyncFromRemoteJob;
class AbstractSyncWithRemoteJob;
class KAbstractDocument;
class KUrl;

// TODO: better names? Active Translator? 
// synchronizers are created by factory functions (like plugins)

// TODO: should synchronizers set the document to readonly if remote is readonly? or who?
// TODO: should synchronizers offer to change writeflag at remote?

/**
possible actions:
1. synchronizer loads document and synchronizes until closing
   -> done by factory functions
2. synchronizer gets attached to a document new created or with other synchronizer
3. synchronizer used to export a model
*/
class AbstractModelSynchronizer : public QObject
{
  Q_OBJECT

  public:
    enum ConnectOption // TODO: better names
    {
      SyncLocalAndRemote = 0,
      ReplaceRemote = 1,
      ReplaceLocal = 2
    };
  protected:
    AbstractModelSynchronizer();
  public:
    virtual ~AbstractModelSynchronizer();

  public:
    KUrl url() const;

  public: // API to be implemented
    // TODO: makes this a job, too
    // TODO: filesystem synchronizer (or: to-passive-storage) does not need this: subclass or interface?
    virtual void startOffering( KAbstractDocument* document ) = 0;
    // TODO: once the synchronizer is attached to a document, this function should not be called
    // is there a way to ensure this?
    virtual AbstractLoadJob *startLoad( const KUrl &url ) = 0;
    /** */
    // TODO: not in constructor? cannot be called twice, each synchronizer is attached to its document
//     virtual KAbstractDocument *createWorkingCopy( const KUrl &originUrl, int *success ) const = 0;

    /** */
    // TODO: static? or by function? or another class? but 
//     virtual void copyTo( const KUrl &url, KAbstractDocument *document, int *success ) const = 0;

    /** overwrite remote with local (save) */
    virtual AbstractSyncToRemoteJob *startSyncToRemote() = 0;
    /** overwrite local with remote (reload) */
    virtual AbstractSyncFromRemoteJob *startSyncFromRemote() = 0;

    /** changes the  */ // TODO: better name for replace: overwrite?
    virtual AbstractSyncWithRemoteJob *startSyncWithRemote( const KUrl &url, AbstractModelSynchronizer::ConnectOption option ) = 0;

    virtual AbstractConnectJob *startConnect( KAbstractDocument *document,
                                              const KUrl &url, AbstractModelSynchronizer::ConnectOption option ) = 0;
//     virtual bool syncBiDirectly() = 0;
//     virtual bool canSyncBiDirectly() const = 0;
//     virtual bool deleteDocument();

    virtual KAbstractDocument *document() const = 0;

  Q_SIGNALS:
    void urlChanged( const KUrl &url );
    void dataPulled( int ) const;
    void dataPushed( int ) const;

  protected: // get
    void setUrl( const KUrl &url );

  protected:
    class Private;
    Private * const d;
};

#endif
