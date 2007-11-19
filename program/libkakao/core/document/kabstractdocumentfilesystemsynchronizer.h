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
    virtual bool syncToRemote();
    virtual bool syncFromRemote();
    virtual bool syncWithRemote( const KUrl &url, KAbstractDocumentSynchronizer::ConnectOption option );

  protected: // API to be implemented
    virtual KAbstractDocument *loadFromFile( const QString &workFilePath ) = 0;
    virtual bool reloadFromFile( const QString &workFilePath ) = 0;
    virtual bool writeToFile( const QString &workFilePath ) = 0;
    virtual bool syncWithFile( const QString &workFilePath,
                               KAbstractDocumentSynchronizer::ConnectOption option ) = 0;

  protected Q_SLOTS:
    void onFileDirty( const QString &filePath );
    void onFileCreated( const QString &filePath );
    void onFileDeleted( const QString &filePath );

  protected: // template functions, call from constructors
    KAbstractDocument *loadFromUrl( const KUrl &url );
    bool reloadFromUrl();
    bool writeToUrl();
    bool syncWithUrl( const KUrl &url, KAbstractDocumentSynchronizer::ConnectOption option );

  protected:
    class Private;
    Private * const d;
};

#endif
