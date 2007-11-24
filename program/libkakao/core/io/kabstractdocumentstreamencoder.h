/***************************************************************************
                          kabstractdocumentstreamencoder.h  -  description
                             -------------------
    begin                : Thu Nov 22 2007
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


#ifndef KABSTRACTDOCUMENTSTREAMENCODER_H
#define KABSTRACTDOCUMENTSTREAMENCODER_H

// Qt
#include <QtCore/QObject>
#include <QtCore/QString>

class KAbstractDocument;
class KAbstractDocumentSelection;
class QIODevice;

// TODO: General synchronizer would load matching encoder and decoder
// manually defined by desktopfile
class KAbstractDocumentStreamEncoder : public QObject
{
    Q_OBJECT

  protected:
    KAbstractDocumentStreamEncoder( const QString &remoteTypeName, const QString &remoteMimeType );
  public:
    virtual ~KAbstractDocumentStreamEncoder();

  public: // API to be implemented
    virtual bool encodeToStream( QIODevice *device, const KAbstractDocument *document ) = 0;
    virtual bool encodeToStream( QIODevice *device, const KAbstractDocumentSelection *selection ) = 0;

  public:
    QString remoteTypeName() const;
    QString remoteMimeType() const;

  protected:
    const QString mRemoteTypeName;
    const QString mRemoteMimeType;
};

#endif
