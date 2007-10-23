/***************************************************************************
                          kdocumentsynchronizer.h  -  description
                             -------------------
    begin                : Sun Sep 2 2007
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


#ifndef KDOCUMENTSYNCHRONIZER_H
#define KDOCUMENTSYNCHRONIZER_H

// Qt
#include <QtCore/QObject>

class QWidget;
class KUrl;
class KAbstractDocument;
class KDocumentAbstractFactory;
class KDocumentManager;


// this is just a simple basic synchronizer specialised for traditional filesystems
// versioning systems, databases, other data sources/sinks have not been modelled yet
// and thus taken into account

// all file operations are treated as processes and return false, if they are cancelled or fail
// if a document does not support the storable interface the process is treated as cancelled
// TODO: think about if there is a semantic difference between cancel (by human) or failed (by machine)
class KDocumentSynchronizer : public QObject
{
  Q_OBJECT

  public:
    explicit KDocumentSynchronizer( KDocumentManager *manager );
    virtual ~KDocumentSynchronizer();

  public:
    void setDocumentFactory( KDocumentAbstractFactory *factory );
    void setWidget( QWidget *widget );

  public:
    /** overwrite local with remote */
    bool reload( KAbstractDocument *document );
    /** overwrite remote with local */
    bool save( KAbstractDocument *document );
    /** store to different location/name and use that furtheron */
    bool saveAs( KAbstractDocument *document );

    // well, we want this both, no?
    bool saveElsewhere( KAbstractDocument *document );
    bool saveCopy( KAbstractDocument *document );
//     bool del( KAbstractDocument *document );

    /** ensures that the document can be closed without unwanted data loosage
    */
    bool canClose( KAbstractDocument *document );

  protected:
    // unless there is a singleton
    KDocumentManager *mManager;
    // used for dialogs, TODO: create (or use?) global instance for this
    QWidget *mWidget;

    // temporary hack: hard coded factory for byte arrays
    KDocumentAbstractFactory *mFactory;
};

#endif
