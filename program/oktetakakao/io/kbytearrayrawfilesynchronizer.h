/***************************************************************************
                          kbytearrayrawfilesynchronizer.h  -  description
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


#ifndef KBYTEARRAYRAWFILESYNCHRONIZER_H
#define KBYTEARRAYRAWFILESYNCHRONIZER_H


// kakao
#include <kabstractdocumentfilesystemsynchronizer.h>

class KByteArrayDocument;

class KByteArrayRawFileSynchronizer : public KAbstractDocumentFileSystemSynchronizer
{
    Q_OBJECT

  public:
    KByteArrayRawFileSynchronizer( KAbstractDocument *document, const KUrl &url,
                                   KAbstractDocumentSynchronizer::ConnectOption option );
    explicit KByteArrayRawFileSynchronizer( const KUrl &originUrl );

  public: // KAbstractDocumentSynchronizer API
    virtual KAbstractDocument *document() const;

  protected: // KAbstractDocumentFileSystemSynchronizer API
    virtual KAbstractDocument *loadFromFile( const QString &workFilePath );
    virtual bool reloadFromFile( const QString &workFilePath );
    virtual bool writeToFile( const QString &workFilePath );
    virtual bool syncWithFile( const QString &workFilePath,
                               KAbstractDocumentSynchronizer::ConnectOption option );

  protected Q_SLOTS:
    void onUrlChange( const KUrl &url );

  protected:
    KByteArrayDocument *mDocument;
};

#endif
