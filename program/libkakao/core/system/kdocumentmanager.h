/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KDOCUMENTMANAGER_H
#define KDOCUMENTMANAGER_H

// lib
#include "kabstractdocument.h"
// Qt
#include <QtCore/QList>
#include <QtCore/QObject>

class KUrl;
class QStringList;

class KDocumentCreateManager; //TODO: temporary
class KDocumentSyncManager; //TODO: temporary
class ModelCodecManager; //TODO: temporary

class KDocumentManager : public QObject
{
  Q_OBJECT

  public:
    KDocumentManager();
    virtual ~KDocumentManager();

  public:
    void addDocument( KAbstractDocument *document );
    void closeDocument( KAbstractDocument* document );

    // TODO: what to do for documents not added?
    bool canClose( KAbstractDocument *document );
    bool canCloseAll();

  public: //TODO: see is this can be removed out of this api, hack for now to MainWindow::saveProperties
    QStringList urls() const;
    KAbstractDocument* documentOfUrl( const KUrl& url ) const;

  public:
    KDocumentCreateManager *createManager();
    KDocumentSyncManager *syncManager();
    ModelCodecManager *codecManager();

  Q_SIGNALS:
    // document got added
    void added( KAbstractDocument *document );
    /// document is about to be closed, cannot be stopped
    void closing( KAbstractDocument *document );
//     void closing( KCloseEvent *event );
// TODO: other than QObject event gets modified by observers, take care of unsetting a close cancel
// problem with a signal is that all(!) observers get notified, even if event is already cancelled
// better a visitor pattern?

//  protected:
//    virtual KAbstractDocument* createDocument();

  protected:
    QList<KAbstractDocument*> mList;

    // TODO: remove into own singleton
    KDocumentCreateManager *mCreateManager;
    KDocumentSyncManager *mSyncManager;
    ModelCodecManager *mCodecManager;
};

inline KDocumentCreateManager *KDocumentManager::createManager() { return mCreateManager; }
inline KDocumentSyncManager *KDocumentManager::syncManager() { return mSyncManager; }
inline ModelCodecManager *KDocumentManager::codecManager() { return mCodecManager; }

#endif
