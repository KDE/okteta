/***************************************************************************
                          kdocumentmanager.h  -  description
                             -------------------
    begin                : Fri Jun 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KDOCUMENTMANAGER_H
#define KDOCUMENTMANAGER_H

// lib
#include "kabstractdocument.h"
// Qt
#include <QtCore/QList>
#include <QtCore/QObject>

class QWidget;
class KUrl;

class KDocumentCreator; //TODO: temporary
class KDocumentSynchronizer; //TODO: temporary

class KDocumentManager : public QObject
{
  Q_OBJECT

  public:
    KDocumentManager();
    virtual ~KDocumentManager();

  public:
    void addDocument( KAbstractDocument *document );
    void closeDocument( KAbstractDocument* document );

    bool canClose( KAbstractDocument *document );
    bool canCloseAll();

  public:
    KDocumentCreator *creator();
    KDocumentSynchronizer *synchronizer();

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
    KDocumentCreator *mCreator;
    KDocumentSynchronizer *mSynchronizer;
};

inline KDocumentCreator *KDocumentManager::creator() { return mCreator; }
inline KDocumentSynchronizer *KDocumentManager::synchronizer() { return mSynchronizer; }

#endif
