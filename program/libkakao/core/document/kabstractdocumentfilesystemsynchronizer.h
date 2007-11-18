/***************************************************************************
                          kabstractdocumentfilesystemsynchronizer.h  -  description
                             -------------------
    begin                : Mon Nov 12 2007
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


#ifndef KABSTRACTDOCUMENTFILESYSTEMSYNCHRONIZER_H
#define KABSTRACTDOCUMENTFILESYSTEMSYNCHRONIZER_H

// lib
#include "kabstractdocumentsynchronizer.h"

class KAbstractDocumentFileSystemSynchronizer : public KAbstractDocumentSynchronizer
{
  Q_OBJECT

  public:
    KAbstractDocumentFileSystemSynchronizer();
    virtual ~KAbstractDocumentFileSystemSynchronizer();

  public: // KAbstractDocumentSynchronizer API
    virtual bool synchToRemote();
    virtual bool synchFromRemote();
    virtual bool synchWithRemote( const KUrl &url, KAbstractDocumentSynchronizer::ConnectOption option );

  protected: // API to be implemented
    virtual KAbstractDocument *loadFromFile( const QString &localFilePath ) = 0;
    virtual bool reloadFromFile( const QString &localFilePath ) = 0;
    virtual bool writeToFile( const QString &localFilePath ) = 0;
    virtual bool synchWithFile( const QString &localFilePath,
                                KAbstractDocumentSynchronizer::ConnectOption option ) = 0;

  protected Q_SLOTS:
    void onFileDirty( const QString &fileName );
    void onFileCreated( const QString &fileName );
    void onFileDeleted( const QString &fileName );

  protected: // template functions, call from constructors
    KAbstractDocument *loadFromUrl( const KUrl &url );
    bool reloadFromUrl();
    bool writeToUrl();
    bool synchWithUrl( const KUrl &url, KAbstractDocumentSynchronizer::ConnectOption option );

  protected:
    class Private;
    Private * const d;
};

#endif
