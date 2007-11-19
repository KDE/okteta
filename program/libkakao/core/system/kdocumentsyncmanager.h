/***************************************************************************
                          kdocumentsyncmanager.h  -  description
                             -------------------
    begin                : Wed Nov 14 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KDOCUMENTSYNCMANAGER_H
#define KDOCUMENTSYNCMANAGER_H

// Qt
#include <QtCore/QObject>

class QString;
class QWidget;
class KUrl;
class KAbstractDocument;
class KAbstractDocumentSynchronizerFactory;
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
// TODO: better name
    bool setSynchronizer( KAbstractDocument *document );
    bool canClose( KAbstractDocument *document );

  public:
    bool hasSynchronizerForLocal( const QString &mimeType );

  public:
    void setDocumentSynchronizerFactory( KAbstractDocumentSynchronizerFactory *synchronizerFactory );
    void setWidget( QWidget *widget );

  Q_SIGNALS:
    void urlUsed( const KUrl &url );

  protected:
    // unless there is a singleton
    KDocumentManager *mManager;
    // used for dialogs, TODO: create (or use?) global instance for this
    QWidget *mWidget;

    // temporary hack: hard coded factories for byte arrays
    KAbstractDocumentSynchronizerFactory *mSynchronizerFactory;
};

#endif
