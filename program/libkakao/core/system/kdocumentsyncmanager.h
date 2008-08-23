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

#ifndef KDOCUMENTSYNCMANAGER_H
#define KDOCUMENTSYNCMANAGER_H

// Qt
#include <QtCore/QObject>

class QString;
class QWidget;
class KUrl;
class KAbstractDocument;
class AbstractModelSynchronizerFactory;
class KDocumentManager;

class KDocumentSyncManager : public QObject
{
  Q_OBJECT

  public:
    explicit KDocumentSyncManager( KDocumentManager *manager );
    virtual ~KDocumentSyncManager();

  public:
    void load( const KUrl &url );
    void load();
    void connectTo();
// TODO: better name
    bool setSynchronizer( KAbstractDocument *document );
    bool canClose( KAbstractDocument *document );
    void startOffering( KAbstractDocument* document );
    void finishOffering( KAbstractDocument* document );

  public:
    bool hasSynchronizerForLocal( const QString &mimeType ) const;
    KUrl urlOf( KAbstractDocument *document ) const;

  public:
    void setDocumentSynchronizerFactory( AbstractModelSynchronizerFactory* synchronizerFactory );
    void setDocumentLiveSynchronizerFactory( AbstractModelSynchronizerFactory* synchronizerFactory );
    void setWidget( QWidget *widget );

  Q_SIGNALS:
    void urlUsed( const KUrl &url );

  protected Q_SLOTS:
    void onDocumentLoaded( KAbstractDocument *document );

  protected:
    // unless there is a singleton
    KDocumentManager *mManager;
    // used for dialogs, TODO: create (or use?) global instance for this
    QWidget *mWidget;

    // temporary hack: hard coded factories for byte arrays
    AbstractModelSynchronizerFactory* mSynchronizerFactory;
    AbstractModelSynchronizerFactory* mLiveSynchronizerFactory;
};

#endif
